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
    private boolean abort = false;
    private WriteRequest currentWriteRequest;
    private int writeAcknowledgeCount = 0;
    private int writeCount = 0;
    public int closedClientSockets = 0;

    private List<Node> serverNodes = new LinkedList<>();
    private List<Node> serverSetupNodes = new LinkedList<>();
    private HashMap<Integer, String> serverDirs = new HashMap<>();
    private HashMap<Integer, S2CMessages> clientMessengers = new HashMap<>();
    private HashMap<Integer, S2SMessages> serverMessengers = new HashMap<>();
    private HashMap<String, Integer> abortAcks = new HashMap<>();
    public PriorityQueue<WriteRequest> clientWriteRequests = new PriorityQueue<>(new WriteRequestComparator());

    // Constructor
    public Server(int serverId) {
        id = serverId;

        serverDirs.put(0, "S0");
        serverDirs.put(1, "S1");
        serverDirs.put(2, "S2");
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
     * Grab list of servers (for server-to-server connections) from config file
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

    public synchronized void updateAbortAcks(String message) {
        if(abortAcks.containsKey(message)) {
            abortAcks.put(message, abortAcks.get(message) + 1);
        } else {
            abortAcks.put(message, 1);
        }
    }
    public synchronized void decideAbort(WriteRequest writeRequest) {
        if(currentWriteRequest.message.equals(writeRequest.message)) {
            writeRequest.abort = abort;
            // Send abort message to other 2 servers
            for(HashMap.Entry<Integer, S2SMessages> entry : serverMessengers.entrySet()) {
                S2SMessages serverMessenger = entry.getValue();
                serverMessenger.sendAbortMsg(writeRequest);
            }

            updateAbortAcks(writeRequest.message);
        }
    }

    /**
     * Abort message for request from other servers.
     * @param writeRequest Request with write information
     */
    public synchronized void processAbortMsgs(WriteRequest writeRequest) {
        abort = abort || writeRequest.abort;
        updateAbortAcks(writeRequest.message);
    }

    /**
     * Send write acknowledgements to other servers to get write permissions.
     * @param writeRequest Request with write information
     */
    public synchronized void writeRequest(WriteRequest writeRequest) {
        // Sending write acknowledgement to other 2 servers
        for(HashMap.Entry<Integer, S2SMessages> entry : serverMessengers.entrySet()) {
            writeAcknowledgeCount++;
            S2SMessages serverMessenger = entry.getValue();
            serverMessenger.sendWriteAcknowledge(writeRequest);
        }

        if(writeAcknowledgeCount == 0) {
            writeToFile(writeRequest);
        }
    }

    /**
     * Write acknowledgements from other servers.
     * @param writeRequest Request with write information
     */
    public synchronized void writeAcknowledge(WriteRequest writeRequest) {
        writeAcknowledgeCount--;
        // System.out.println(writeAcknowledgeCount);
        if(writeAcknowledgeCount == 0 && currentWriteRequest != null) {
            writeToFile(writeRequest);
        }
    }
    
    /**
     * Write to file.
     * @param writeRequest Request with write information
     */
    public synchronized void writeToFile(WriteRequest writeRequest) {
        if(currentWriteRequest.done) {
            return;
        }

        try {
            String filePath = "./" + serverDirs.get(id) + "/" + writeRequest.fileName;
            BufferedWriter writer = new BufferedWriter(new FileWriter(filePath, true));
            writer.append(writeRequest.message + "\n");
            writer.close();

            currentWriteRequest.success = true;
            currentWriteRequest.done = true;

            System.out.println("Server " + id + ": Successfully wrote Client " + writeRequest.clientId + "'s request to file " + writeRequest.fileName);
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * Resets variables for next request.
     */
    public void reset() {
        // writeAcknowledgeCount = 0;
        // abortAcks = 0;
        abort = false;
    }

    /**
	 * Starting up server.
	 */
    public void start() {
        setServerNodes();
        setServerSetupNodes();

        openServerConnection();
        Scanner blocker = new Scanner(System.in);
        blocker.nextLine();
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
                        currentWriteRequest = clientWriteRequests.poll();
                        // System.out.println("> Got the next request!");
                        abort = abort || new Random().nextDouble() <= 0.2; // abort happens 20% of the time;
                        System.out.println("Server " + id + ": Abort Status? ==> " + abort);
                        decideAbort(currentWriteRequest);

                        try {
                            Thread.sleep(2000);
                        } catch (InterruptedException ex) {
                            printException(ex);
                        }

                        while(abortAcks.get(currentWriteRequest.message) < 3) {
                            try {
                                Thread.sleep(1);
                            } catch (InterruptedException ex) {
                                printException(ex);
                            }
                        }

                        if(abort) {
                            currentWriteRequest.success = false;
                            currentWriteRequest.done = true;
                            reset();
                            continue;
                        }

                        // System.out.println("> Passed Abort Stage!");
                        writeRequest(currentWriteRequest);
                        while(!currentWriteRequest.done) {
                            try {
                                Thread.sleep(1);
                            } catch (InterruptedException ex) {
                                printException(ex);
                            }
                        }
                        System.out.println("Write Count: " + ++writeCount);
                        reset();
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