import java.io.*;
import java.net.Socket;

public class S2SMessages {
    private Socket otherServerSocket;
    private Server server;
    private int otherServerId, serverId;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private WriteRequest writeRequest;

    public S2SMessages(Socket otherServerSocket, int serverId, int otherServerId, Server server) {
        this.otherServerSocket = otherServerSocket;
        this.serverId = serverId;
        this.server = server;
        this.otherServerId = otherServerId;

        try {
            out = new ObjectOutputStream(this.otherServerSocket.getOutputStream());
            in = new ObjectInputStream(this.otherServerSocket.getInputStream());
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

    public S2SMessages(Socket otherServerSocket, int serverId, Server server) {
        this.otherServerSocket = otherServerSocket;
        this.serverId = serverId;
        this.server = server;

        try {
            out = new ObjectOutputStream(this.otherServerSocket.getOutputStream());
            in = new ObjectInputStream(this.otherServerSocket.getInputStream());

            out.writeObject("GET_SERVER_ID"); out.flush();
            System.out.println("Server " + serverId + ": New Server Connection! GET_SERVER_ID request sent");
            String otherServerId = in.readObject().toString();
            this.otherServerId = Integer.parseInt(otherServerId);
            System.out.println("Server " + serverId + ": Server ID received - " + this.otherServerId);
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

    public Socket getOtherServer() {
        return otherServerSocket;
    }

    public int getOtherServerId() {
        return otherServerId;
    }

    /**
     * This method is used to handle server messages from servers.
     * @param in2 The input stream from the server.
     * @param out2 The output stream to the server.
     * @return 1 if the server is still connected, 0 if the server is disconnected.
     */
    public int serverMessage(ObjectInputStream in2, ObjectOutputStream out2) {
        try {
            String serverRequest = in2.readObject().toString();

            switch (serverRequest) {
                case "GET_SERVER_ID":
                    out2.writeObject(serverId); out.flush();
                    break;
                case "ABORT":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Server " + serverId + ": Received ABORT message from Server " + otherServerId + " for file " + writeRequest.fileName);
                    // System.out.println("Server " + serverId + ": Aborting Client " + writeRequest.clientId + "'s request to file " + writeRequest.fileName + "...");
                    server.processAbortMsgs(writeRequest);
                    break;
                case "WRITE_ACK":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Server " + serverId + ": Received write acknowledgement from Server " + otherServerId + " for file " + writeRequest.fileName);
                    server.writeAcknowledge(writeRequest);
                    break;
            }
        } catch (Exception ex) {
            printException(ex);
            return 0;
        }
        return 1;
    }
    
    /**
     * This method is used to send an ABORT to the other server.
     * @param request The aborted request to be sent.
     */
    public synchronized void sendAbortMsg(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending abort message for request to Server " + otherServerId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("ABORT"); out.flush();
            out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a WRITE acknowledgement to the other server.
     * @param request The request to be sent.
     */
    public synchronized void sendWriteAcknowledge(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending write acknowledgement for request to Server " + otherServerId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("WRITE_ACK"); out.flush();
            out.writeObject(writeRequest); out.flush();
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
			otherServerSocket.close();
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
        System.err.println("\n");
    }
}