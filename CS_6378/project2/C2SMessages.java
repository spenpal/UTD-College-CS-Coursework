import java.io.*;
import java.net.Socket;

public class C2SMessages {
    private Socket clientSocket;
    private Client client;
    private int clientId, serverId;
    private ObjectInputStream in;
    private ObjectOutputStream out;

    public C2SMessages(Socket clientSocket, int clientId, int serverId, Client client) {
        this.clientSocket = clientSocket;
        this.clientId = clientId;
        this.serverId = serverId;
        this.client = client;

        try {
            out = new ObjectOutputStream(this.clientSocket.getOutputStream());
            in = new ObjectInputStream(this.clientSocket.getInputStream());
        }
        catch (Exception ex) {
            printException(ex);
        }

        Thread serverRequests = new Thread() {
            public void run() {
                while(serverMessage(in, out) != 0) { }
            }
        };

        serverRequests.start();
    }

    /**
     * This method is used to recieve messages from the server.
     * @param in2 The input stream from the server.
     * @param out2 The output stream to the server.
     * @return 1 if the client is still connected, 0 if the client is disconnected.
     */
    public int serverMessage(ObjectInputStream in2, ObjectOutputStream out2) {
        try {
            String serverMessage = in2.readObject().toString();

            switch (serverMessage) {
                case "GET_CLIENT_ID":
                    out2.writeObject(clientId);
                    break;
                case "ENQUIRE_ACK":
                    System.out.println("Client " + clientId + ": Recieved ENQUIRE acknowledge from Server " + serverId);
                    String hostedFiles = in2.readObject().toString();
                    System.out.println("Client " + clientId + ": Hosted Files - " + hostedFiles);
                    client.setHostedFiles(hostedFiles);
                    break;
                case "WRITE_ACK":
                    System.out.println("Client " + clientId + ": Recieved WRITE acknowledge from Server " + serverId);
                    // client.numOfWriteAcks++;
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
     * @param fileName The name of the file to be written to.
     */
    public synchronized void write(String fileName) {
        System.out.println("Client " + clientId + ": Sending WRITE request to Server " + serverId + " for file " + fileName);
        
        try {
            out.writeObject("WRITE");
            out.writeObject(fileName);
            out.writeObject(System.currentTimeMillis());
        } catch (IOException e) {
            printException(e);
        }
    }

    /**
     * This method is used to send enquire requests to the server.
     */
    public synchronized void enquire() {
        System.out.println("Client " + clientId + ": Sending ENQUIRE request to Server " + serverId);

        try {
            out.writeObject("ENQUIRE");
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send close requests to the server, and close the streams.
     */
    public void close() {
        try {
            out.writeObject("CLOSE");
            // serverRequests.interrupt();
            out.close();
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
