import java.net.*;
import java.io.*;
import java.util.*;

class WriteRequestComparator implements Comparator<WriteRequest> {
    @Override
    public int compare(WriteRequest wr1, WriteRequest wr2) {
        return (int) (wr1.timestamp - wr2.timestamp);
    }
}

public class Server {

    // Instance Variables
    private ServerSocket serverSocket;
    private ServerSocket serverSetupSocket;
    private String hostedFiles = "";
    private int id, port;

    public int closedClientSockets = 0;

    private List<Node> serverNodes = new LinkedList<>();
    private List<Node> serverSetupNodes = new LinkedList<>();
    private HashMap<Integer, String> serverDirs = new HashMap<>();
    private HashMap<Integer, S2CMessages> clientMessengers = new HashMap<>();
    private HashMap<Integer, S2SMessages> serverMessengers = new HashMap<>();
    private HashMap<Integer, Boolean> serverPermissionRequired = new HashMap<>();
    private HashMap<Integer, WriteRequest> deferredReplyList = new HashMap<>();
    public PriorityQueue<WriteRequest> clientWriteRequests = new PriorityQueue<>(new WriteRequestComparator());

    private int outstandingReplyCount = 0;
    private boolean requestedCS = false;
    private boolean usingCS = false;
    private String requestedFileForCS = "";
    private int writeAcknowledgeCount;

    private WriteRequest myWriteRequest;

    // Constructor
    public Server(int serverId) {
        id = serverId;

        serverDirs.put(0, "S0");
        serverDirs.put(1, "S1");
        serverDirs.put(2, "S2");
    }

    /**
     * Grab list of servers (for client connections) from config file
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
     * Grab list of servers (for server connections) from config file
     */
    public void setServerSetupNodes() {
        try {
            BufferedReader br = new BufferedReader(new FileReader("serverSetupInfos"));
            String line;
            while ((line = br.readLine()) != null) {
                String[] serverSetupInfo = line.split(",");
                Node serverSetupNode = new Node(Integer.parseInt(serverSetupInfo[0]), serverSetupInfo[1], Integer.parseInt(serverSetupInfo[2]));
                serverSetupNodes.add(serverSetupNode);
            }
            br.close();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * Connect to servers
     */
    public void setServerConnections() {
        while(serverMessengers.size() < serverSetupNodes.size() - 1) {
            try {
                for(int i = id + 1; i < serverSetupNodes.size(); i++) {
                    Socket otherServerSocket = new Socket(serverSetupNodes.get(i).ip, serverSetupNodes.get(i).port);
                    S2SMessages serverConnection = new S2SMessages(otherServerSocket, id, i, this);
                    serverMessengers.put(i, serverConnection);
                    serverPermissionRequired.put(i, true);
                }
            } catch (IOException ex) {}
        }
    }

    /**
     * Listen for other server connection requests
     */
    public void openServerConnection() {
        try {
            port = serverSetupNodes.get(id).port;
            serverSetupSocket = new ServerSocket(port);
        } catch (IOException ex) {
			printException(ex);
		}

        Server server = this;
        Thread currentServer = new Thread() {
            public void run() {
                while(serverMessengers.size() < serverNodes.size() - 1) {
                    try {
                        Socket otherServerSocket = serverSetupSocket.accept();
                        S2SMessages serverConnection = new S2SMessages(otherServerSocket, id, server);
                        serverMessengers.put(serverConnection.getOtherServerId(), serverConnection);
                        serverPermissionRequired.put(serverConnection.getOtherServerId(), true);
                    }
                    catch(IOException ex) { 
                        printException(ex);
                    }
                }
            }
        };

        // currentServer.setDaemon(true);
        currentServer.start();
    }

    /**
     * Get hosted files
     */
    public synchronized String getHostedFiles() {
        if(hostedFiles.isEmpty()) {
            String folderPath = "./" + serverDirs.get(id) + "/";
            File folder = new File(folderPath);
            File[] listOfFiles = folder.listFiles();

            for(File file : listOfFiles) {
                hostedFiles += file.getName() + ",";
            }
            
            hostedFiles = hostedFiles.substring(0, hostedFiles.length() - 1);
        }
        return hostedFiles;
    }

    
    /**
     * Send request to other servers to get CS permissions.
     * @param writeRequest Request with write information
     */
    public synchronized void writeRequest(WriteRequest writeRequest) {
        if(!(this.requestedCS || this.usingCS)) {
            myWriteRequest = writeRequest;
            requestedFileForCS = myWriteRequest.fileName;
            requestedCS = true;
            System.out.println("Server " + id + ": Creating WRITE request with message <" + myWriteRequest.message + "> requesting CS access for file " + myWriteRequest.fileName);

            for(HashMap.Entry<Integer, Boolean> entry : serverPermissionRequired.entrySet()) {
                S2SMessages serverMessenger = serverMessengers.get(entry.getKey());
                boolean permissionNeeded = entry.getValue();
                if(permissionNeeded) {
                    outstandingReplyCount = outstandingReplyCount + 1;
                    serverMessenger.request(myWriteRequest);
                }
            }

            if(outstandingReplyCount == 0) {
                enterCS(myWriteRequest);
            }
        }
        else {
            System.out.println("Server " + id + ": Currently in CS or already requested for CS");
        }
    }

    /**
     * Process a request from another server.
     * @param writeRequest Request with write information.
     */
    public synchronized void processRequest(WriteRequest writeRequest) {
        if(writeRequest.fileName.equals(requestedFileForCS)) {
            if (usingCS || requestedCS) {
                if ((myWriteRequest.timestamp < writeRequest.timestamp) 
                        || (myWriteRequest.timestamp == writeRequest.timestamp) && (myWriteRequest.clientId < writeRequest.clientId)) {
                    System.out.println("Server " + id + ": Deferring reply for request from Server " + writeRequest.serverId + " for file " + writeRequest.fileName);
                    serverPermissionRequired.replace(writeRequest.serverId, true);
                    deferredReplyList.put(writeRequest.serverId, writeRequest);
                }
            }
            else {
                System.out.println("Server " + id + ": Sending reply for request from Server " + writeRequest.serverId + " for file " + writeRequest.fileName);
                serverPermissionRequired.replace(writeRequest.serverId, true);
                S2SMessages serverMessenger = serverMessengers.get(writeRequest.serverId);
                serverMessenger.reply(writeRequest);
            }
        }
        else {
            serverPermissionRequired.replace(writeRequest.serverId, true);
            S2SMessages serverMessenger = serverMessengers.get(writeRequest.serverId);
            serverMessenger.reply(writeRequest);
        }
    }

    /**
     * Process a reply from another server.
     * @param replyingServerId Id of server replying
     * @param writeRequest Request with write information.
     */
    public synchronized void processReply(int replyingServerId, WriteRequest writeRequest) {
        String fileName = writeRequest.fileName;
        if(fileName.equals(requestedFileForCS)) {
            // System.out.println("Server " + id + ": Recieved reply for from Server " + replyingServerId + " for file " + fileName);
            serverPermissionRequired.replace(replyingServerId, false);
            outstandingReplyCount--;
            if (outstandingReplyCount == 0) {
                enterCS(myWriteRequest);
            }
        }
        else {
            System.out.println("Server " + id + ": Recieved reply for from Server " + replyingServerId + ", but for wrong requested file " + fileName);
        }
    }

    /**
     * Enter critical section.
     * @param writeRequest Request with write information
     */
    public void enterCS(WriteRequest writeRequest) {
        System.out.println("Server " + id + ": Entering critical section for file " + writeRequest.fileName);
        usingCS = true;
        requestedCS = false;

        try {
            writeAcknowledgeCount = serverMessengers.size();
            
            writeToFile(writeRequest);
            System.out.println("Server " + id + ": Finished writing on file " + writeRequest.fileName);
            for(HashMap.Entry<Integer, S2SMessages> entry : serverMessengers.entrySet()) {
                S2SMessages serverMessenger = entry.getValue();
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
    public void releaseCS(WriteRequest writeRequest) {
        System.out.println("Server " + id + ": Successfully wrote and updated all replicas of file " + writeRequest.fileName);
        System.out.println("Server " + id + ": Releasing critical section for file " + writeRequest.fileName);
        this.usingCS = false;
        this.requestedCS = false;
        for (HashMap.Entry<Integer, WriteRequest> entry : deferredReplyList.entrySet()) {
            S2SMessages serverMessenger = serverMessengers.get(entry.getKey());
            serverMessenger.reply(entry.getValue());
        }
        requestedFileForCS = "";
        deferredReplyList.clear();
        writeRequest.success = true;
    }
    
    /**
     * Write to file.
     * @param writeRequest Request with write information
     */
    public synchronized boolean writeToFile(WriteRequest writeRequest) {
        try {
            String filePath = "./" + serverDirs.get(id) + "/" + writeRequest.fileName;
            BufferedWriter writer = new BufferedWriter(new FileWriter(filePath, true));
            writer.append(writeRequest.message + "\n");
            writer.close();

            if (writeRequest.serverId != id) {
                serverMessengers.get(writeRequest.serverId).sendWriteAcknowledge(writeRequest);
            }
            return true;
        } catch (IOException ex) {
            printException(ex);
        }
        return false;
    }

    /**
     * Write acknowledgements from other servers.
     * @param writeRequest Request with write information
     */
    public synchronized void writeAcknowledge(WriteRequest writeRequest) {
        if(writeRequest.fileName.equals(requestedFileForCS)) {
            writeAcknowledgeCount--;
            // System.out.println(writeAcknowledgeCount);
            if(writeAcknowledgeCount == 0) {
                System.out.println("Server " + id + ": CS completed for file " + writeRequest.fileName);
                releaseCS(myWriteRequest);
            }
        }
    }

    /**
	 * Starting up server.
	 */
    public void start() {
        setServerNodes();
        setServerSetupNodes();

        openServerConnection();
        Scanner blocker = new Scanner(System.in);
        blocker.hasNext();
        setServerConnections();
        blocker.close();

        try {
            port = serverNodes.get(id).port;
            serverSocket = new ServerSocket(port);
            System.out.println("Server " + id + " running on " + port);
        } catch (IOException ex) {
			printException(ex);
		}

        Server server = this;
        Thread currentServerNode = new Thread() {
            public void run() {
                int clientConnections = 0;
                while(clientConnections < 5) {
                    try {
                        Socket clientSocket = serverSocket.accept();
                        S2CMessages serverConnection = new S2CMessages(clientSocket, id, server);
                        clientMessengers.put(serverConnection.getClientId(), serverConnection);
                        clientConnections++;
                    }
                    catch(IOException ex) { 
                        printException(ex);
                    }
                }

                while(true) {
                    if(clientWriteRequests.size() > 0) {
                        System.out.println("We got a write request!");
                        WriteRequest writeRequest = clientWriteRequests.poll();
                        writeRequest(writeRequest);
                        while(!writeRequest.success) {
                            try {
                                Thread.sleep(1);
                            } catch (InterruptedException ex) {
                                printException(ex);
                            }
                        }
                    }
                    else {
                        try {
                            Thread.sleep(1);
                        } catch (InterruptedException ex) {
                            printException(ex);
                        }
                    }
                }
            }
        };

        currentServerNode.start();
    }

    /**
	 * Closes socket connections.
	 */
    public void stopConnections() {
        while (closedClientSockets < 5) {
            try {
                Thread.sleep(1000);
            } catch (Exception ex) {
                printException(ex);
            }
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
            System.out.println("ERROR: Incorrect Usage. Try \"java Server <server-id>\"");
            System.exit(0);
        }
        int serverId = -1;
        try {
            serverId = Integer.parseInt(args[0]);
            if (serverId < 0 || serverId > 2) {
                System.out.println("ERROR: Invalid Server ID. Enter a Server ID between 0-2.");
                System.exit(0);
            }
        }
        catch (Exception ex) {
            System.out.println("ERROR: Invalid Server ID. Enter a Server ID between 0-2.");
            System.exit(0);
        }
        
        System.out.println("Starting server...");
        Server server = new Server(serverId);
        server.start();
    }
}