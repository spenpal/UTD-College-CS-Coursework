import java.io.Serializable;

public class WriteRequest implements Serializable {
    String fileName, message;
    long timestamp;
    int clientId, serverId;
    boolean success = false;
    boolean done = false;
    boolean abort = false;
    // int abortAcks = 2;
    // int writeAcknowledgeCount = 0;

    public WriteRequest(String fileName, int clientId, long clientTimestamp) {
        this.fileName = fileName;
        this.clientId = clientId;
        this.timestamp = clientTimestamp;
        message = clientId + ", " + clientTimestamp;
    }

    public void setServerId(int serverId) {
        this.serverId = serverId;
    }

    public String toString() {
        return "File: " + fileName + "\nClient: " + clientId + "\nTimestamp: " + timestamp +
                "\nMessage: " + message + "\nServer: " + serverId + "\nSuccess: " + success + "\nDone: " + done + 
                "\nAddress: " + Integer.toHexString(System.identityHashCode(this));
    }
}