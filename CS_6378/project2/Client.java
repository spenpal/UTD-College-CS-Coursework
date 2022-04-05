import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Client {
    private int id, port, numOfWriteRequests = 5;
    private String hostedFiles;
    private List<Node> clientNodes = new LinkedList<>();
    private List<Node> serverNodes = new LinkedList<>();

    HashMap<Integer, ClientMessages> serverMessengers = new HashMap<>();

    private int logicalClock = 0;
    int minimumDelay = 5000;

    public Client(int id) {
        this.id = id;
    }
    
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

    public void setupServerConnection() {
        for (Node serverNode : serverNodes) {
            try {
                Socket clientSocket = new Socket(serverNode.ip, serverNode.port);
                ClientMessages serverMessenger = new ClientMessages(clientSocket);
                serverMessengers.put(serverNode.id, serverMessenger);
            } catch (IOException ex) {
                printException(ex);
            }
        }
    }

    public String getHostedFiles() {
        return serverMessengers.get(0).enquire();
    }

    public void generateWriteRequests() {
        Thread writeRequests = new Thread() {
            public void run() {
                try {
                    for (int i = 0; i < numOfWriteRequests; i++) {
                        System.out.println("Generating WRITE request...");
                        Random r = new Random();
                        String fileName = "f" + r.nextInt(hostedFiles.length());
                        sendWriteRequest(fileName);

                        double randFraction = Math.random() * 1000;
                        int delay = (int) Math.floor(randFraction) + minimumDelay;
                        System.out.println("The AUTO REQUEST THREAD thread will sleep for " + delay + " seconds");
                        Thread.sleep(delay);
                    }
                }
                catch (Exception ex) {
                    printException(ex);
                }
            }
        };

        writeRequests.setDaemon(true);
        writeRequests.start();
    }

    public String sendWriteRequest(String fileName) {
        Random r = new Random();
        int serverId = r.nextInt(serverNodes.size());
        String serverMessage = serverMessengers.get(serverId).write(fileName);
        return serverMessage;
    }

    public void start() {
        setClientNodes();
        setServerNodes();
        setupServerConnection();
        hostedFiles = getHostedFiles();

        generateWriteRequests();
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
            System.out.println("ERROR: Invalid Server ID. Enter a Client ID between 0-2.");
            System.exit(0);
        }


        System.out.println("Starting client...");
        Client client = new Client(clientId);
        client.start();
    }
}