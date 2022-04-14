import java.io.*;
import java.net.Socket;

public class S2CMessages {
    private Socket clientSocket;
    private Server server;
    private int clientId, serverId;
    private ObjectInputStream in;
    private ObjectOutputStream out;

    public S2CMessages(Socket clientSocket, int serverId, Server server) {
        this.clientSocket = clientSocket;
        this.serverId = serverId;
        this.server = server;

        try {
            out = new ObjectOutputStream(this.clientSocket.getOutputStream());
            in = new ObjectInputStream(this.clientSocket.getInputStream());

            out.writeObject("GET_CLIENT_ID"); out.flush();
            System.out.println("Server " + serverId + ": New Client Connection! GET_CLIENT_ID request sent");
            String clientId = in.readObject().toString();
            this.clientId = Integer.parseInt(clientId);
            System.out.println("Server " + serverId + ": Client ID received - " + this.clientId);
        }
        catch (Exception ex) {
            printException(ex);
        }

        Thread clientRequests = new Thread() {
            public void run() {
                while(clientMessage(in, out) != 0) { }
            }
        };

        clientRequests.start();
    }

    public Socket getClient() {
        return clientSocket;
    }

    public int getClientId() {
        return clientId;
    }

    /**
     * This method is used to handle client requests.
     * @param in2 The input stream from the client.
     * @param out2 The output stream to the client.
     * @return 1 if the client is still connected, 0 if the client is disconnected.
     */
    public int clientMessage(ObjectInputStream in2, ObjectOutputStream out2) {
        try {
            String clientRequest = in2.readObject().toString();

            switch (clientRequest) {
                case "ENQUIRE":
                    enquire();
                    break;
                case "WRITE":
                    System.out.println("Server " + serverId + ": Recieved WRITE request from Client " + clientId);
                    String fileName = in2.readObject().toString();
                    long clientTimestamp = Long.valueOf(in2.readObject().toString());
                    write(fileName, clientId, clientTimestamp);
                    break;
            }
        } catch (Exception ex) {
            printException(ex);
            return 0;
        }
        return 1;
    }

    /**
     * This method is used to send write requests to the server.
     * @param fileName The name of the file to be written.
     * @param clientId The ID of the client.
     * @param clientTimestamp The timestamp of the client.
     */
    public synchronized void write(String fileName, int clientId, long clientTimestamp) {
        WriteRequest writeRequest = new WriteRequest(fileName, clientId, serverId, clientTimestamp);
        server.clientWriteRequests.add(writeRequest);
        while(!writeRequest.success) {
            try {
                Thread.sleep(1);
            } catch (InterruptedException ex) {
                printException(ex);
            }
        }

        System.out.println("Server " + serverId + ": Sending WRITE acknowledge to Client " + clientId);
        try {
            out.writeObject("WRITE_ACK"); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send enquire requests to the server.
     */
    public synchronized void enquire() {
        String hostedFiles = server.getHostedFiles();
        System.out.println("Server " + serverId + ": Successful enquire of files [request from Client " + clientId + "]");

        try {
            out.writeObject("ENQUIRE_ACK"); out.flush();
            out.writeObject(hostedFiles); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to close the streams.
     */
    public void close() {
        try {
            out.close(); out.flush();
            in.close();
			clientSocket.close();
        } catch (IOException e) {
            printException(e);
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
}
