import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;

public class P2 {

    // Instance Variables
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private BufferedReader in;
    private PrintWriter out;
    private int port;
    private int bytes = 100;

    // Constructor
    public P2(int port) {
        this.port = port;
    }

    /**
	 * Starts up the server socket connection and awaits for process P1 to connect.
	 */
    public void start() {
        try {
            System.out.println("Starting server socket...");
            serverSocket = new ServerSocket(port);
            clientSocket = serverSocket.accept();
            System.out.println("P1 has been accepted!");
    
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            out = new PrintWriter(clientSocket.getOutputStream(), true);
        } catch (IOException ex) {
			printException(ex);
		}
    }

    /**
	 * Sends data from file F2 to process P1, 100 bytes at a time.
	 */
    public void send() {
        try {
            byte[] b = new byte[bytes];
            InputStream is = new FileInputStream("./D1/F2");
    
            System.out.println("Sending file F2 data over...");
            while (is.read(b) != -1) {
                String data = new String(b, StandardCharsets.UTF_8); // convert read bytes to string
                out.println(data); // send data over
            }
            System.out.println("All data sent!");
    
            out.println("$Over"); // ending message for no more data to send
            is.close();
        } catch (IOException ex) {
			printException(ex);
		}
	}

    /**
     * While process P1 is sending file F1 data over,
	 * "insert" it into file "F2"
     * 
     * To give the illusion file F2 is being edited natively, create a new file F2'
	 * Add file F1 data to file F2'
	 * Append file F2 data to file F2'
     */
	public void recv() {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter("./D1/F2'", true)); // create a new file for F2
            BufferedReader br = new BufferedReader(new FileReader("./D1/F2"));
            String data = "";
            
            // "Insert" data from file F1 to F2', as it comes through
            System.out.println("Recieving file F1 data...");
            while(!(data = in.readLine()).equals("$Over"))
                bw.write(data);
            System.out.println("All data recieved!");
            
            // Add all data from file F2 to F2'
            while ((data = br.readLine()) != null)
                bw.write(data);
            
            bw.close();
            br.close();
        } catch (IOException ex) {
			printException(ex);
		}
	}

    /**
	 * Closes all file streams and socket connections.
	 */
    public void stop(){
        try {
			in.close();
        	out.close();
			clientSocket.close();
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
        P2 server = new P2(2222);
        server.start();
        server.recv();
        server.send();

        // Replace old F2 file with new one
        Path new_F2_path = Paths.get("./D1/F2'");
        Path og_F2_path = Paths.get("./D1/F2");
        try {
            Files.move(new_F2_path, og_F2_path, StandardCopyOption.REPLACE_EXISTING);
        } catch (IOException e) {
            System.out.println("Oops! Cannot rename/replace new files for some reason.");
        }
    }
}