import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

public class Server {

    // Instance Variables
    private ServerSocket serverSocket;
    private String hostedFiles = "";
    private int id, port;

    private List<Node> serverNodes = new LinkedList<>();
    private HashMap<Integer, String> serverDirs = new HashMap<>();
    private HashMap<Integer, ServerMessages> clientMessengers = new HashMap<>();

    // Constructor
    public Server(int serverId) {
        id = serverId;

        serverDirs.put(0, "S0");
        serverDirs.put(1, "S1");
        serverDirs.put(2, "S2");
    }

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
	 * Starts up the server socket connection and awaits for process P1 to connect.
	 */
    public void start() {
        setServerNodes();

        try {
            port = serverNodes.get(id).port;
            serverSocket = new ServerSocket(port);
            System.out.println("Server " + id + " running on " + port);
        } catch (IOException ex) {
			printException(ex);
		}

        Server currentServer = this;
        Thread currentServerNode = new Thread() {
            public void run() {
                while(true) {
                    try {
                        Socket clientSocket = serverSocket.accept();
                        ServerMessages serverConnection = new ServerMessages(clientSocket, id, currentServer);
                        clientMessengers.put(serverConnection.getClientId(), serverConnection);
                    }
                    catch(IOException ex) { 
                        printException(ex);
                    }
                }
            }
        };

        currentServerNode.setDaemon(true);
        currentServerNode.start();
    }

    /*write to file and send ack*/
    public synchronized boolean writeToFile(String fileName, int clientId, String clientTimestamp) {
        try {
            String filePath = "./" + serverDirs.get(id) + "/" + fileName;
            BufferedWriter writer = new BufferedWriter(new FileWriter(filePath, true));
            writer.append(clientId + "," + clientTimestamp + "\n");
            writer.close();
            return true;
        } catch (IOException ex) {
            printException(ex);
        }
        return false;
    }

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
	 * Closes socket connection.
	 */
    public void stop() {
        try {
            serverSocket.close();
		} catch (IOException ex) {
			printException(ex);
		}
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

        // System.out.println("Booted server " + args[0] + " successfully!");
    }
}