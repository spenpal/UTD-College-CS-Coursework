import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ServerMessages {
    private Socket clientSocket;
    private Server server;
    private int clientId, serverId;
    private BufferedReader in;
    private PrintWriter out;

    public ServerMessages(Socket clientSocket, int serverId, Server server) {
        this.clientSocket = clientSocket;
        this.serverId = serverId;
        this.server = server;

        try {
            in = new BufferedReader(new InputStreamReader(this.clientSocket.getInputStream()));
            out = new PrintWriter(this.clientSocket.getOutputStream(), true);

            out.println("GET_CLIENT_ID");
            // System.out.println("GET_CLIENT_ID request sent");
            clientId = Integer.parseInt(in.readLine());
            // System.out.println("GET_CLIENT_ID request response received with ID: " + clientId);
        }
        catch (Exception ex) {
            printException(ex);
        }

        Thread clientRequests = new Thread() {
            public void run() {
                while(clientMessage() != 0) { }
            }
        };

        clientRequests.setDaemon(true);
        clientRequests.start();
    }

    public Socket getClient() {
        return clientSocket;
    }

    public int getClientId() {
        return clientId;
    }

    public int clientMessage() {
        try {
            String clientRequest = in.readLine();

            switch (clientRequest) {
                case "GET_SERVER_ID":
                    out.println(serverId);
                    break;
                case "ENQUIRE":
                    enquire();
                    break;
                case "WRITE":
                    System.out.println("Server " + serverId + ": Recieved WRITE request from Client " + clientId);
                    String fileName = in.readLine();
                    String clientTimestamp = in.readLine();
                    write(fileName, clientId, clientTimestamp);
                    break;
            }
        } catch (Exception ex) {
            printException(ex);
            return 0; 
        }
        return 1;
    }

    public synchronized void write(String fileName, int clientId, String clientTimestamp) {
        if (server.writeToFile(fileName, clientId, clientTimestamp)) {
            System.out.println("Server " + serverId + ": Successful write to file " + fileName + " [request from Client " + clientId + "]");
            out.println("WRITE_ACK");
            out.println(fileName);
        }
    }

    public synchronized void enquire() {
        String hostedFiles = server.getHostedFiles();
        System.out.println("Server " + serverId + ": Successful enquire of files [request from Client " + clientId + "]");
        out.println("ENQUIRE_ACK");
        out.println(hostedFiles);
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
}
