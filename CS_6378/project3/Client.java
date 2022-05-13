import java.net.*;
import java.util.*;
import java.io.*;

class WriteRequestComparator implements Comparator<WriteRequest> {
    @Override
    public int compare(WriteRequest wr1, WriteRequest wr2) {
        if(wr1.timestamp - wr2.timestamp == 0) {
            return wr1.clientId - wr2.clientId;
        }
        return (int) (wr1.timestamp - wr2.timestamp);
    }
}

public class Client {
    private ServerSocket clientSetupSocket;
    private int id, port;
    private int numOfWriteRequests = 3, minimumDelay = 2000;
    private boolean voted = false;
    private boolean usingCS = false;
    private boolean failed = false, yieldSent = false;
    private boolean requestsSent = false;

    private String[] hostedFiles;
    private List<Node> serverNodes = new LinkedList<>();
    private List<Node> clientNodes = new LinkedList<>();
    private int[] quorumIds;

    HashMap<Integer, C2CMessages> clientMessengers = new HashMap<>();
    HashMap<Integer, C2SMessages> serverMessengers = new HashMap<>();
    PriorityQueue<WriteRequest> clientWriteRequests = new PriorityQueue<>(new WriteRequestComparator());
    PriorityQueue<WriteRequest> inquireRequests = new PriorityQueue<>(new WriteRequestComparator());

    private int outstandingReplyCount = 0;
    WriteRequest currentWriteRequest;

    public Client(int id) {
        this.id = id;
    }

    public void setQuorum() {
        quorumIds = new int[] {id, (id + 1) % 5, (id + 2) % 5};
    }

    public int compare(WriteRequest wr1, WriteRequest wr2) {
        if(wr1.timestamp - wr2.timestamp == 0) {
            return wr1.clientId - wr2.clientId;
        }
        return (int) (wr1.timestamp - wr2.timestamp);
    }

    public boolean inQuorum(int id) {
        for(int quorumId : quorumIds) {
            if(quorumId == id) {
                return true;
            }
        }

        return false;
    }

    /**
     * Grab list of clients (for client-to-client connections) from config file
     */
    public void setClientNodes() {
        try {
            BufferedReader br = new BufferedReader(new FileReader("clientInfos"));
            String line;
            while ((line = br.readLine()) != null) {
                String[] clientInfo = line.split(",");
                Node clientNode = new Node(Integer.parseInt(clientInfo[0]), clientInfo[1], Integer.parseInt(clientInfo[2]));
                clientNodes.add(clientNode);
            }
            br.close();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * Grab list of servers (for client-to-server connections) from config file
     */
    public void setServerNodes() {
        try {
            BufferedReader br = new BufferedReader(new FileReader("serverInfos"));
            String line;
            while ((line = br.readLine()) != null) {
                String[] serverInfo = line.split(",");
                Node serverNode = new Node(Integer.parseInt(serverInfo[0]), serverInfo[1], Integer.parseInt(serverInfo[2]));
                serverNodes.add(serverNode);
            }
            br.close();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * Listen for other client connection requests
     */
    public void openClientConnection() {
        try {
            port = clientNodes.get(id).port;
            clientSetupSocket = new ServerSocket(port);
        } catch (IOException ex) {
			printException(ex);
		}

        Client client = this;
        Thread currentClient = new Thread() {
            public void run() {
                while(clientMessengers.size() < clientNodes.size() - 1) {
                    try {
                        Socket otherClientSocket = clientSetupSocket.accept();
                        C2CMessages clientConnection = new C2CMessages(otherClientSocket, id, client);
                        clientMessengers.put(clientConnection.getOtherClientId(), clientConnection);
                    }
                    catch(IOException ex) {
                        printException(ex);
                    }
                }
            }
        };

        // currentServer.setDaemon(true);
        currentClient.start();
    }

    public void setupClientConnections() {
        while(clientMessengers.size() < clientNodes.size() - 1) {
            try {
                for(int i = id + 1; i < clientNodes.size(); i++) {
                    Socket otherClientSocket = new Socket(clientNodes.get(i).ip, clientNodes.get(i).port);
                    C2CMessages clientConnection = new C2CMessages(otherClientSocket, id, i, this);
                    clientMessengers.put(i, clientConnection);
                }
            } catch (IOException ex) {}
        }
    }

    /**
     * Connect to servers
     */
    public void setupServerConnections() {
        for (Node serverNode : serverNodes) {
            try {
                Socket clientSocket = new Socket(serverNode.ip, serverNode.port);
                C2SMessages serverMessenger = new C2SMessages(clientSocket, id, serverNode.id, this);
                serverMessengers.put(serverNode.id, serverMessenger);
            } catch (IOException ex) {
                printException(ex);
            }
        }
    }

    /**
     * Set list of hosted files
     */
    public void setHostedFiles(String hostedFiles) {
        this.hostedFiles = hostedFiles.split(",");
    }

    /**
     * Send enquire request to servers
     */
    public void sendEnquireRequest() {
        serverMessengers.get(0).enquire();
    }

    /**
     * Choose random file to send write request to servers, with some delay in between
     */
    public void generateWriteRequests() {
        Thread writeRequests = new Thread() {
            public void run() {
                try {
                    for (int i = 0; i < numOfWriteRequests; i++) {
                        System.out.println("Client " + id + ": Generating WRITE request #" + (i + 1) + "...");
                        Random r = new Random();
                        String fileName = hostedFiles[r.nextInt(hostedFiles.length)];
                        long timestamp = System.currentTimeMillis();
                        WriteRequest writeRequest = new WriteRequest(fileName, id, timestamp);
                        clientWriteRequests.add(writeRequest);

                        double randFraction = Math.random() * 0;
                        int delay = (int) Math.floor(randFraction) + minimumDelay;
                        System.out.println("SYSTEM: REQUEST thread will delay for " + delay + " milliseconds");
                        Thread.sleep(minimumDelay);
                    }
                }
                catch (Exception ex) {
                    printException(ex);
                }
            }
        };

        // writeRequests.setDaemon(true);
        writeRequests.start();
    }

    /**
     * Check requests queue for write requests and works with them appropriately
     */
    public synchronized void checkRequestQueue() {
        currentWriteRequest = clientWriteRequests.poll();
        
        // if your own request is at the top
        if (currentWriteRequest.clientId == id) {
            doWriteRequest(currentWriteRequest);
        }
        else { 
            // reply to other client's request
            this.voted = true;
            C2CMessages clientMessenger = clientMessengers.get(currentWriteRequest.clientId);
            clientMessenger.reply(currentWriteRequest);
        }
    }

    /**
     * Sends requests to other clients in quorum to get CS permissions.
     * @param writeRequest Request with write information
     */
    public synchronized void doWriteRequest(WriteRequest writeRequest) {
        this.voted = true;
        if(requestsSent) {
            if (outstandingReplyCount == 0) {
                enterCS(writeRequest);
            }
            return;
        }
        
        System.out.println("Client " + id + ": Creating WRITE request with message <" + writeRequest.message + "> requesting CS access for file " + writeRequest.fileName);

        for(int quorumId : quorumIds) {
            if(quorumId != id) {
                outstandingReplyCount++;
                C2CMessages clientMessenger = clientMessengers.get(quorumId);
                clientMessenger.request(writeRequest);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException ex) {
                    printException(ex);
                }
            }
        }

        requestsSent = true;
    }

    /**
     * Process a request from another client.
     * @param writeRequest Request with write information.
     */
    public void processRequest(WriteRequest writeRequest) {
        clientWriteRequests.add(writeRequest);

        // Peform Inquire or Failed Messages
        if(currentWriteRequest != null) {
            int compareValue = compare(writeRequest, currentWriteRequest);
            if(compareValue > 0 || usingCS) {
                // Send Failed Message
                if(writeRequest.clientId != id) {
                    C2CMessages clientMessenger = clientMessengers.get(writeRequest.clientId);
                    clientMessenger.failed(currentWriteRequest);
                }
                else {
                    processFailed(currentWriteRequest);
                }
            } 
            else {
                // Send Inquire Message
                if(currentWriteRequest.clientId != id) {
                    C2CMessages clientMessenger = clientMessengers.get(currentWriteRequest.clientId);
                    clientMessenger.inquire(writeRequest);
                }
                else {
                    processInquire(writeRequest);
                }
            }
        }
    }

    /**
     * Process a reply from another client.
     * @param replyingClientId id of client replying
     * @param writeRequest Request with write information.
     */
    public synchronized void processReply(WriteRequest writeRequest) {
        outstandingReplyCount--;
        if (outstandingReplyCount == 0 && currentWriteRequest.clientId == id) {
            enterCS(writeRequest);
        }
    }

    /**
     * Enter critical section.
     * @param writeRequest Request with write information
     */
    public synchronized void enterCS(WriteRequest writeRequest) {
        System.out.println("Client " + id + ": Entering critical section for file " + writeRequest.fileName);
        this.usingCS = true;
        // System.out.println("Client " + id + ": Sending write request to all servers...");

        try {
            for(HashMap.Entry<Integer, C2SMessages> entry : serverMessengers.entrySet()) {
                C2SMessages serverMessenger = entry.getValue();
                serverMessenger.write(writeRequest);
            }
        }
        catch (Exception ex) {
            printException(ex);
        }
    }

    /**
     * Release critical section.
     * @param writeRequest Request with write information
     */
    public synchronized void releaseCS(WriteRequest writeRequest) {
        // System.out.println("Client " + id + ": Successfully wrote and updated all replicas of file " + writeRequest.fileName);
        System.out.println("Client " + id + ": Releasing critical section for file " + writeRequest.fileName);

        usingCS = false;
        voted = false;
        failed = false;
        yieldSent = false;
        requestsSent = false;
        inquireRequests = new PriorityQueue<>(new WriteRequestComparator());

        for(int quorumId : quorumIds) {
            if(quorumId != id) {
                C2CMessages clientMessenger = clientMessengers.get(quorumId);
                clientMessenger.release(currentWriteRequest);
            }
        }
    }

    /**
     * Process a release from another client.
     */
    public void processRelease() {
        voted = false;
        // failed = false;
        yieldSent = false;
        inquireRequests = new PriorityQueue<>(new WriteRequestComparator());
    }

    /**
     * Process a failed from another client.
     */
    public void processFailed(WriteRequest writeRequest) {
        failed = true;

        handleInquireMessages();

        // if(clientWriteRequests.size() > 0) {
        //     voted = true;
        //     WriteRequest newRequest = clientWriteRequests.poll();
        //     clientWriteRequests.add(currentWriteRequest);
        //     currentWriteRequest = newRequest;
        //     C2CMessages clientMessenger = clientMessengers.get(currentWriteRequest.clientId);
        //     clientMessenger.reply(currentWriteRequest);
        // }
    }

    public void handleInquireMessages() {
		for(WriteRequest inqReq: inquireRequests){
			processInquire(inqReq);
		}
		inquireRequests = new PriorityQueue<>(new WriteRequestComparator());
	}

    /**
     * Process inquires from array.
     */
    public void processInquire(WriteRequest writeRequest) {
        processInquire(writeRequest, -1);
    }

    /**
     * Process a inquire from another client.
     */
    public void processInquire(WriteRequest writeRequest, int otherClientId) {
        if(((failed) || (yieldSent && !voted)) && !usingCS) {
            // if writeRequest receieved is not in quorum, send reply message
            if(!inQuorum(writeRequest.clientId)) {
                voted = true;
                clientWriteRequests.add(currentWriteRequest);
                currentWriteRequest = clientWriteRequests.poll();
                C2CMessages clientMessenger = clientMessengers.get(writeRequest.clientId);
                clientMessenger.reply(writeRequest);
            }
            else {
                yieldSent = true;
                outstandingReplyCount++;
                C2CMessages clientMessenger = clientMessengers.get(writeRequest.clientId);
                clientMessenger.yield(writeRequest);
            }
        }
        else if (otherClientId != -1) {
            System.out.println("Client " + id + ": Adding Client's " + otherClientId + " inquire request to inquire queue");
            inquireRequests.add(writeRequest);
        }
        else {
            inquireRequests.add(writeRequest);
        }
    }

    /**
     * Process a yield from another client.
     */
    public void processYield(WriteRequest writeRequest) {
        if(!clientWriteRequests.isEmpty()) {
            voted = true;
            outstandingReplyCount--;
            clientWriteRequests.add(currentWriteRequest);
            currentWriteRequest = clientWriteRequests.poll();
            C2CMessages clientMessenger = clientMessengers.get(currentWriteRequest.clientId);
            clientMessenger.reply(currentWriteRequest);
        }
    }

    /**
     * Detect if deadlock is occuring in the system.
     */
    public boolean detectDeadlock() {
        int oldReplyCount = outstandingReplyCount;
        try {
            Thread.sleep(10000);
        } catch (InterruptedException ex) {
            printException(ex);
        }
        if (oldReplyCount == outstandingReplyCount) {
            System.out.println("Client " + id + ": Deadlock detected!");
            return true;
        }

        return false;
    }
    /**
     * Start program
     */
    public void start() {
        setQuorum();

        setClientNodes();
        openClientConnection();
        Scanner blocker = new Scanner(System.in);
        blocker.nextLine();
        setupClientConnections();

        setServerNodes();
        setupServerConnections();
        
        Thread currentClientNode = new Thread() {
            public void run() {
                boolean sixSecTimer = false;

                while(true) {
                    while(clientWriteRequests.size() == 0) {
                        try {
                            // System.out.println(clientWriteRequests.size());
                            Thread.sleep(1);
                        }
                        catch (Exception ex) {
                            printException(ex);
                        }
                    }

                    // Sleep for 6 seconds to ensure deadlock occurs
                    if(!sixSecTimer) {
                        try {
                            Thread.sleep(6000);
                        } catch (InterruptedException ex) {
                            printException(ex);
                        }
                        sixSecTimer = true;
                    }

                    // System.out.println(voted);
                    while(voted || usingCS) {
                        try {
                            Thread.sleep(1);
                        }
                        catch (Exception ex) {
                            printException(ex);
                        }
                    }

                    checkRequestQueue();

                    // System.out.println(currentWriteRequest);
                    while(!currentWriteRequest.done) {
                        try {
                            Thread.sleep(1);
                        } catch (InterruptedException ex) {
                            printException(ex);
                        }
                    }
                    if(currentWriteRequest.clientId == id) {
                        releaseCS(currentWriteRequest);
                    }
                }
            }
        };
        currentClientNode.start();

        sendEnquireRequest();
        blocker.nextLine();
        generateWriteRequests();
        blocker.close();

        Thread deadlockHandler = new Thread() {
            public void run() {
                if(!detectDeadlock()) {
                    try {
                        Thread.sleep(1);
                    }
                    catch (Exception ex) {
                        printException(ex);
                    }
                }

                while(true) {
                    while(clientWriteRequests.size() == 0) {
                        try {
                            Thread.sleep(1);
                        }
                        catch (Exception ex) {
                            printException(ex);
                        }
                    }
    
                    WriteRequest nextWriteRequest = clientWriteRequests.peek();
    
                    // Handle FAILED messages
                    if(compare(nextWriteRequest, currentWriteRequest) > 0) {
                        C2CMessages clientMessenger = clientMessengers.get(nextWriteRequest.clientId);
                        clientMessenger.failed(nextWriteRequest);
                    }
                    else {
                        // Handle INQUIRE messages
                        C2CMessages clientMessenger = clientMessengers.get(currentWriteRequest.clientId);
                        clientMessenger.inquire(nextWriteRequest);
                    }
                }
            }
        };
        // deadlockHandler.start();
    }

    /**
     * Close all connections
     */
    public void stopConnections() {
        for (C2SMessages serverMessenger : serverMessengers.values()) {
            serverMessenger.close();
        }
        System.exit(0);
    }

    private void printException(Exception ex) {
		System.out.println("Oops! Something went wrong. Here's the stack trace:");

		StackTraceElement[] e = ex.getStackTrace();
        System.err.println("\nERROR = " + ex + 
                           "\nFILE = " + e[0].getFileName() + 
                           "\nCLASS =" + e[0].getClassName() + 
                           "\nMETHOD = " + e[0].getMethodName() + 
                           "\nLINE = " + e[0].getLineNumber());

		System.err.println("FULL TRACE = ");
		ex.printStackTrace(System.err);
	}

    public static void main(String[] args) {

        if (args.length != 1) {
            System.out.println("ERROR: Incorrect Usage. Try \"java Client <client-id>\"");
            System.exit(0);
        }
        int clientId = -1;
        try {
            clientId = Integer.parseInt(args[0]);
            if (clientId < 0 || clientId > 4) {
                System.out.println("ERROR: Invalid Client ID. Enter a Client ID between 0-4.");
                System.exit(0);
            }
        }
        catch (Exception ex) {
            System.out.println("ERROR: Invalid Client ID. Enter a Client ID between 0-4.");
            System.exit(0);
        }

        System.out.println("Starting client...");
        Client client = new Client(clientId);
        client.start();
    }
}