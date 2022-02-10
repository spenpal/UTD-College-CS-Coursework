import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;

public class P1 {

	// Instance Variables
	private Socket clientSocket;
    private BufferedReader in;
    private PrintWriter out;
	private String address;
	private int port;
	private int bytes = 75;
	
	// Constructor
	public P1(String address, int port) {
		this.address = address;
		this.port = port;
	}

	/**
	 * Starts up the socket connection with process P2 as a client.
	 * 
	 * @throws IOException
	 */
	public void start() throws IOException {
		System.out.println("Starting client socket...");
        clientSocket = new Socket(address, port);
		System.out.println("P1 has been accepted!");

		in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        out = new PrintWriter(clientSocket.getOutputStream(), true);
    }

	/**
	 * Sends data from file F1 to process P2, 75 bytes at a time.
	 * 
	 * @throws Exception
	 */
	public void send() throws Exception {
		byte[] b = new byte[bytes];
		InputStream is = new FileInputStream("./D1/F1");

		System.out.println("Sending file F1 data over...");
		while (is.read(b) != -1) {
			String data = new String(b, StandardCharsets.UTF_8); // convert read bytes to string
			out.println(data); // send data over
		}
		System.out.println("All data sent!");

		out.println("$Over"); // ending message for no more data to send
		is.close();
	}

	/**
	 * While process P2 is sending file F2 data over,
	 * "append" it to file "F1"
	 * 
	 * To give the illusion file F1 is being edited natively, create a new file F1'
	 * Add all data from file F1 to file F1'
	 * Append file F2 data to file F1'
	 * 
	 * @throws Exception
	 */
	public void recv() throws Exception {
		BufferedWriter bw = new BufferedWriter(new FileWriter("./D1/F1'", true)); // create a new file for F1
        BufferedReader br = new BufferedReader(new FileReader("./D1/F1"));
        String data = "";
		
		// Add all data from file F1 to F1'
		while ((data = br.readLine()) != null)
            bw.write(data);

		// "Append" data from file F2 to F1', as it comes through
		System.out.println("Recieving file F2 data...");
        while(!(data = in.readLine()).equals("$Over"))
            bw.write(data);
		System.out.println("All data recieved!");

        bw.close();
        br.close();
	}

	/**
	 * Closes all file streams and socket connection.
	 * 
	 * @throws IOException
	 */
	public void stop() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
    }

	public static void main(String args[]) throws Exception {
		P1 client = new P1("localhost", 2222);
		client.start();
		client.send();
		client.recv();

		// Replace old F1 file with new one
		// Path new_F1_path = Paths.get("./D1/F1'");
        // Path og_F1_path = Paths.get("./D1/F1");
        // Files.move(new_F1_path, og_F1_path, StandardCopyOption.REPLACE_EXISTING);
	}
}