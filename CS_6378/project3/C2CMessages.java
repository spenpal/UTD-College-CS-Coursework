import java.io.*;
import java.net.Socket;

public class C2CMessages {
    private Socket otherClientSocket;
    private Client client;
    private int otherClientId, clientId;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private WriteRequest writeRequest;

    public C2CMessages(Socket otherClientSocket, int clientId, int otherClientId, Client client) {
        this.otherClientSocket = otherClientSocket;
        this.clientId = clientId;
        this.otherClientId = otherClientId;
        this.client = client;

        try {
            out = new ObjectOutputStream(this.otherClientSocket.getOutputStream());
            in = new ObjectInputStream(this.otherClientSocket.getInputStream());
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

    public C2CMessages(Socket otherClientSocket, int clientId, Client client) {
        this.otherClientSocket = otherClientSocket;
        this.clientId = clientId;
        this.client = client;

        try {
            out = new ObjectOutputStream(this.otherClientSocket.getOutputStream());
            in = new ObjectInputStream(this.otherClientSocket.getInputStream());

            out.writeObject("GET_CLIENT_ID"); out.flush();
            System.out.println("Client " + clientId + ": New Client Connection! GET_CLIENT_ID request sent");
            String otherClientId = in.readObject().toString();
            this.otherClientId = Integer.parseInt(otherClientId);
            System.out.println("Client " + clientId + ": Client ID received - " + this.otherClientId);
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

    public Socket getOtherClient() {
        return otherClientSocket;
    }

    public int getOtherClientId() {
        return otherClientId;
    }

    /**
     * This method is used to handle client messages from clients.
     * @param in2 The input stream from the client.
     * @param out2 The output stream to the client.
     * @return 1 if the client is still connected, 0 if the client is disconnected.
     */
    public int clientMessage(ObjectInputStream in2, ObjectOutputStream out2) {
        try {
            String clientRequest = in2.readObject().toString();

            switch (clientRequest) {
                case "GET_CLIENT_ID":
                    out2.writeObject(clientId); out.flush();
                    break;
                case "REQUEST":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received request from Client " + otherClientId + " with message <" + writeRequest.message + "> for file " + writeRequest.fileName);
                    client.processRequest(writeRequest);
                    break;
                case "REPLY":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received reply from Client " + otherClientId + " for file " + writeRequest.fileName);
                    client.processReply(writeRequest);
                    break;
                case "RELEASE":
                    // writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received release from Client " + otherClientId + " for file " + writeRequest.fileName);
                    client.processRelease();
                    client.currentWriteRequest.done = true;
                    break;
                case "FAILED":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received FAILED from Client " + otherClientId + " for file " + writeRequest.fileName);
                    client.processFailed(writeRequest);
                    break;
                case "INQUIRE":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received INQUIRE from Client " + otherClientId + " for file " + writeRequest.fileName);
                    client.processInquire(writeRequest, otherClientId);
                    break;
                case "YIELD":
                    writeRequest = (WriteRequest) in2.readObject();
                    System.out.println("Client " + clientId + ": Received YIELD from Client " + otherClientId + " for file " + writeRequest.fileName);
                    client.processYield(writeRequest);
                    break;
            }
        } catch (Exception ex) {
            printException(ex);
            return 0;
        }
        return 1;
    }

    /**
     * This method is used to send a WRITE request to the other server.
     * @param request The request to be sent.
     */
    public synchronized void request(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending WRITE request to Client " + otherClientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("REQUEST"); out.flush();
            out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a REPLY to the other server.
     * @param request The request to be sent.
     */
    public synchronized void reply(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending reply for request to Client " + writeRequest.clientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("REPLY"); out.flush();
            out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a REPLY to the other server.
     * @param request The request to be sent.
     */
    public synchronized void release(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending release message to Client " + otherClientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("RELEASE"); out.flush();
            // out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a FAILED to the other server.
     * @param request The request to be sent.
     */
    public synchronized void failed(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending FAILED message to Client " + otherClientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("FAILED"); out.flush();
            out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a INQUIRE to the other server.
     * @param request The request to be sent.
     */
    public synchronized void inquire(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending INQUIRE message to Client " + otherClientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("INQUIRE"); out.flush();
            out.writeObject(writeRequest); out.flush();
        } catch (IOException ex) {
            printException(ex);
        }
    }

    /**
     * This method is used to send a YIELD to the other server.
     * @param request The request to be sent.
     */
    public synchronized void yield(WriteRequest writeRequest) {
        System.out.println("Client " + clientId + ": Sending YIELD message to Client " + otherClientId + " for file " + writeRequest.fileName);
        try {
            out.writeObject("YIELD"); out.flush();
            out.writeObject(writeRequest); out.flush();
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
        System.err.println("\n");
    }
}