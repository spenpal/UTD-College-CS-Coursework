import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientMessages {
    private Socket clientSocket;
    private int clientId, serverId;
    private BufferedReader in;
    private PrintWriter out;

    public ClientMessages(Socket clientSocket) {
        this.clientSocket = clientSocket;

        try {
            in = new BufferedReader(new InputStreamReader(this.clientSocket.getInputStream()));
            out = new PrintWriter(this.clientSocket.getOutputStream(), true);

            out.println("GET_SERVER_ID");
            serverId = Integer.parseInt(in.readLine());
        }
        catch (Exception ex) {
            printException(ex);
        }
    }


    public synchronized String write(String fileName) {
        System.out.println("Sending write request from Client " + clientId + " to server with Server " + serverId);
        out.println("WRITE");
        out.println(fileName);
        out.println(System.currentTimeMillis());
        return serverMessage();
    }

    public synchronized String enquire() {
        System.out.println("Sending enquire request from Client " + clientId + " to server with Server " + serverId);
        out.println("ENQUIRE");
        return serverMessage();
    }

    public String serverMessage() {
        try {
            return in.readLine();
        } catch (Exception ex) {
            printException(ex);
        }
        return "";
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
