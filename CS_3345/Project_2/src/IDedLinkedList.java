
public class IDedLinkedList<AnyType extends IDedObject>
{
    // Linked List Nodes
    class Node
    {
        AnyType data;
        Node next;

        Node (AnyType d, Node n)
        {
            data = d;
            next = n;
        }
    }

    // Instance Variables
    private Node head; // Pointer To The Head Of The List
    private int size; // Size Of The LinkedList

    IDedLinkedList()
    {
        head = null;
        size = 0;
    }

    // Empties the linked list
    public void makeEmpty()
    {
        head = null;
        size = 0;
    }

    /* Get the generic type to get the particular id and returns AnyType.
    Don’t remove the object from the list.
    Returns null if the list is empty or ID not found. */
    public AnyType findID(int ID)
    {
        Node current = head;

        while(current != null)
        {
            if(current.data.getID() == ID)
            {
                return current.data;
            }

            current = current.next;
        }

        return null;
    }

    // Insert at front of list or return false if that ID already exists
    public boolean insertAtFront(AnyType x)
    {
        // If ID exists, then do not insert at the front of the list
        if(findID(x.getID()) != null)
        {
            return false;
        }

        Node node = new Node(x, null);

        // If List Is Empty, Set First Node As Head
        if(head == null)
        {
            head = node;
            size++;
            return true;
        }

        node.next = head;
        head = node;
        size++;
        return true;
    }

    // Delete and return the record at the front of the list or return null if the list is empty
    public AnyType deleteFromFront()
    {
        if(head == null)
        {
            return null;
        }

        AnyType front = head.data;
        head = head.next;
        size--;
        return front;
    }

    // Find and delete the record with the given ID or returns null if it isn’t found
    public AnyType delete(int ID)
    {
        // Item Is Not Found
        if(head == null)
        {
            return null;
        }

        // If item to be deleted is the head...
        if(head.data.getID() == ID)
        {
            AnyType front = head.data;
            head = head.next;
            size--;
            return front;
        }

        Node current = head;
        Node prev;

        while(current != null)
        {
            if(current.data.getID() == ID)
            {
                prev = current.next;
                size--;
                return current.data;
            }

            prev = current;
            current = current.next;
        }

        return null;
    }

    // Return the sum of ids of all elements currently in the list. If list is empty return -1.
    public int printTotal()
    {
        int sum = 0;

        if(head == null)
        {
            return -1;
        }

        Node current = head;

        while(current != null)
        {
            sum += current.data.getID();
            current = current.next;
        }

        return sum;
    }
}
