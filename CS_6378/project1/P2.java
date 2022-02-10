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
	 * 
	 * @throws IOException
	 */
    public void start() throws IOException {
        System.out.println("Starting server socket...");
        serverSocket = new ServerSocket(port);
        clientSocket = serverSocket.accept();
        System.out.println("P1 has been accepted!");

        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        out = new PrintWriter(clientSocket.getOutputStream(), true);
    }

    /**
	 * Sends data from file F2 to process P1, 100 bytes at a time.
	 * 
	 * @throws Exception
	 */
    public void send() throws Exception {
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
	}

    /**
     * While process P1 is sending file F1 data over,
	 * "insert" it into file "F2"
     * 
     * To give the illusion file F2 is being edited natively, create a new file F2'
	 * Add file F1 data to file F2'
	 * Append file F2 data to file F2'
     * 
     * @throws Exception
     */
	public void recv() throws Exception {
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
	}

    /**
	 * Closes all file streams and socket connections.
	 * 
	 * @throws IOException
	 */
    public void stop() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
        serverSocket.close();
    }

    public static void main(String[] args) throws Exception {
        P2 server = new P2(2222);
        server.start();
        server.recv();
        server.send();

        // Replace old F2 file with new one
        // Path new_F2_path = Paths.get("./D1/F2'");
        // Path og_F2_path = Paths.get("./D1/F2");
        // Files.move(new_F2_path, og_F2_path, StandardCopyOption.REPLACE_EXISTING);
    }
}
