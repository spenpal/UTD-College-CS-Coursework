import java.net.Socket;
import java.util.*;
import java.io.*;

public class Client {
    private int id, numOfWriteRequests = 5;
    public int numOfWriteAcks = 0;
    int minimumDelay = 1000;
    private String[] hostedFiles;
    private List<Node> serverNodes = new LinkedList<>();

    HashMap<Integer, C2SMessages> serverMessengers = new HashMap<>();

    public Client(int id) {
        this.id = id;
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
                C2SMessages serverMessenger = new C2SMessages(clientSocket, id, serverNode.id, this);
                serverMessengers.put(serverNode.id, serverMessenger);
            } catch (IOException ex) {
                printException(ex);
            }
        }
    }

    public void setHostedFiles(String hostedFiles) {
        this.hostedFiles = hostedFiles.split(",");
    }

    public void sendEnquireRequest() {
        serverMessengers.get(0).enquire();
    }

    public void generateWriteRequests() {
        Thread writeRequests = new Thread() {
            public void run() {
                try {
                    for (int i = 0; i < numOfWriteRequests; i++) {
                        System.out.println("Client " + id + ": Generating WRITE request #" + (i + 1) + "...");
                        Random r = new Random();
                        String fileName = "f" + r.nextInt(hostedFiles.length);
                        sendWriteRequest(fileName);

                        double randFraction = Math.random() * 1000;
                        int delay = (int) Math.floor(randFraction) + minimumDelay;
                        System.out.println("SYSTEM: REQUEST thread thread will delay for " + delay + " milliseconds");
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

    public void sendWriteRequest(String fileName) {
        Random r = new Random();
        int serverId = r.nextInt(serverNodes.size());
        serverMessengers.get(serverId).write(fileName);
    }

    public void start() {
        setServerNodes();
        setupServerConnection();

        sendEnquireRequest();
        Scanner blocker = new Scanner(System.in);
        blocker.hasNext();
        generateWriteRequests();
        blocker.close();

        // while (numOfWriteAcks < numOfWriteRequests) {
        //     try {
        //         Thread.sleep(1000);
        //     } catch (Exception ex) {
        //         printException(ex);
        //     }
        // }

        // stopConnections();
    }

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
            System.out.println("ERROR: Invalid Server ID. Enter a Client ID between 0-2.");
            System.exit(0);
        }

        System.out.println("Starting client...");
        Client client = new Client(clientId);
        client.start();
    }
}