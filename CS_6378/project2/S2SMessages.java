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

        // clientRequests.setDaemon(true);
        serverRequests.start();
    }

    public S2SMessages(Socket otherServerSocket, int serverId, Server server) {
        this.otherServerSocket = otherServerSocket;
        this.serverId = serverId;
        this.server = server;

        try {
            out = new ObjectOutputStream(this.otherServerSocket.getOutputStream());
            in = new ObjectInputStream(this.otherServerSocket.getInputStream());

            out.writeObject("GET_SERVER_ID");
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

        // clientRequests.setDaemon(true);
        serverRequests.start();
    }

    public Socket getOtherServer() {
        return otherServerSocket;
    }

    public int getOtherServerId() {
        return otherServerId;
    }

    public int serverMessage(ObjectInputStream in2, ObjectOutputStream out2) {
        try {
            String serverRequest = in2.readObject().toString();

            switch (serverRequest) {
                case "GET_SERVER_ID":
                    out2.writeObject(serverId);
                    break;
                case "REQUEST":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Server " + serverId + ": Received request from Server " + otherServerId + " with message <" + writeRequest.message + "> for file " + writeRequest.fileName);
                    server.processRequest(writeRequest);
                    break;
                case "REPLY":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Server " + serverId + ": Received reply from Server " + otherServerId + " for file " + writeRequest.fileName);
                    server.processReply(otherServerId, writeRequest);
                    break;
                case "WRITE":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Server " + serverId + ": Received write instruction from Server " + otherServerId + " for file " + writeRequest.fileName);
                    server.writeToFile(writeRequest);
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

    public synchronized void write(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending WRITE instruction to Server " + otherServerId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("WRITE");
            out.writeObject(writeRequest);
        } catch (IOException ex) {
            printException(ex);
        }
    }

    public synchronized void request(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending WRITE request to Server " + otherServerId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("REQUEST");
            out.writeObject(writeRequest);
        } catch (IOException ex) {
            printException(ex);
        }
    }
    
    public synchronized void reply(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending reply for request to Server " + writeRequest.serverId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("REPLY");
            out.writeObject(writeRequest);
        } catch (IOException ex) {
            printException(ex);
        }
    }

    public synchronized void sendWriteAcknowledge(WriteRequest writeRequest) {
        System.out.println("Server " + serverId + ": Sending write acknowledgement for request to Server " + writeRequest.serverId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("WRITE_ACK");
            out.writeObject(writeRequest);
        } catch (IOException ex) {
            printException(ex);
        }
    }

    public void close() {
        try {
            out.close();
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