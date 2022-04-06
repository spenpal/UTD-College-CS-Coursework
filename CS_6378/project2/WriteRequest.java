import java.io.Serializable;

public class WriteRequest implements Serializable {
    String fileName, message;
    long timestamp;
    int clientId;
    int serverId;
    boolean success = false;

    public WriteRequest(String fileName, int clientId, int serverId, long clientTimestamp) {
        this.fileName = fileName;
        this.clientId = clientId;
        this.serverId = serverId;
        this.timestamp = clientTimestamp;
        message = clientId + ", " + clientTimestamp;
    }
}