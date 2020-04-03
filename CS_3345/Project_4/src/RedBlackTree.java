import java.rmi.activation.ActivationGroup_Stub;
import java.util.StringJoiner;

public class RedBlackTree<E extends Comparable<E>>
{

    /**
     * Node Class For The RedBlackTree
     * @param <E>   Generic Object Type Of The Node
     */
    private static class Node<E extends Comparable<E>>
    {
        private E element;
        private Node<E> leftChild;
        private Node<E> rightChild;
        private Node<E> parent;
        private boolean color;

        public Node()
        {
            this(null);
        }

        public Node(E element)
        {
            this.element = element;
            leftChild = null;
            rightChild = null;
            parent = null;
            color = BLACK;
        }

        public String toString()
        {
            if(color == RED)
            {
                return "*" + element;
            }
            else
            {
                return "" + element;
            }
        }
    }

    // Instance Variables For RedBlackTree class
    private static final boolean BLACK = true;
    private static final boolean RED = false;
    private Node<E> root;
    private Node<E> emptyNode;

    public RedBlackTree()
    {
        emptyNode = new Node<E>();
        root = emptyNode;
    }

    /**
     * "Insert" the given element into the tree at the correct position, and then rebalance
     * the tree if necessary.
     * The correct position is defined by the properties of a binary search tree,
     * and the rebalancing procedure should enforce the properties of a red-black tree.
     * Regarding input validation, insert should immediately throw a NullPointerException with a
     * descriptive message, if the given element is null.
     * Alternatively, if the given element is a duplicate of an element already in the tree, then
     * insert should not insert the given element.
     * The return value should indicate whether the given element was inserted into the tree or not.
     * @param element   Given Number or String
     * @return          Truth Value, whether the insertion was successful or not
     * @throws          NullPointerException If A Null Element is passed along
     */
    public boolean insert(E element) throws NullPointerException
    {
        if(element == null)
        {
            throw new NullPointerException("Element Cannot Be Null");
        }

        Node<E> temp = emptyNode;
        Node<E> current = root;
        Node<E> newNode = new Node<E>(element);

        // If Tree Is Empty, Then Make New Node The Root Of The Tree
        if(root == emptyNode)
        {
            root = newNode;
            return true;
        }

        while(current != emptyNode)
        {
            temp = current;

            if((newNode.element).compareTo(current.element) < 0) // If Element Is Less Than Compared To Element, Traverse Left Sub-Tree
            {
                current = current.leftChild;
            }
            else if((newNode.element).compareTo(current.element) > 0) // If Element Is Less Than Compared To Element, Traverse Right Sub-Tree
            {
                current = current.rightChild;
            }
            else // If Element Is Equal To Compared To Element, Then It Is A Duplicate
            {
                return false;
            }
        }

        // Found A Place To Insert newNode
        newNode.parent = temp;
        if(newNode.element.compareTo(temp.element) < 0)
        {
            temp.leftChild = newNode;
        }
        else
        {
            temp.rightChild = newNode;
        }

        newNode.leftChild = emptyNode;
        newNode.rightChild = emptyNode;
        newNode.color = RED;

        balanceTree(newNode);
        return true;
    }

    /**
     * Return whether the tree contains any element that compares equal to the given object using
     * the compareTo method of the object.
     * However, if the given object is null, then contains should not throw an exception but rather
     * should return false.
     * @param object    Element To Be Seen in The Tree
     * @return          If Element Exists In The Tree
     */
    public boolean contains(Comparable<E> object)
    {
        if(object == null || contains(root, object) == emptyNode) // Using Helper Method
        {
            return false;
        }

        return true;
    }

    /**
     * Return a string representing the pre-order traversal of this tree.
     * The returned string should be the ordered concatenation of invoking the toString method of
     * each element in the traversal, where every two adjacent elements should be separated by a
     * single space character (“ ”).
     * If an element is located in a red node, then it should be preceded by a single asterisk
     * character (“*”) in the output string.
     * Otherwise, an element located in a black node should not be preceded by an asterisk
     *
     * @return          Pre-Order Traversal Of Elements In Tree
     */
    public String toString()
    {
        if(root == null)
        {
            return "";
        }
        else
        {
            return preOrderPrint(root, new StringJoiner(" ")).toString();
        }
    }

    //----------------------------------------------------------------------------------------------
    // Helper Methods
    private void balanceTree(Node<E> x) // Colors And Rotates Nodes To Make The Tree Properties Satisfactory
    {
        Node<E> current = x; // Newly Inserted Item

        while(current.parent.color == RED) // If Parent Node Is Black, Then Break
        {
            if(current.parent.equals(current.parent.parent.leftChild)) // If Parent Node Color Equals Uncle Node Color | Current On Right Sub-Tree
            {
                Node<E> uncle = current.parent.parent.rightChild; // Make Temp Node Equal To Uncle

                if(uncle.color == RED)
                {
                    current.parent.color = BLACK;
                    uncle.color = BLACK;
                    current.parent.parent.color = RED;
                    current = current.parent.parent; // Point To Grandfather Now
                }
                else
                {
                    if(current.equals(current.parent.rightChild)) // If Node Equals Sibling
                    {
                        // Rotate The Parent
                        current = current.parent;
                        leftRotation(current);
                    }

                    // Re-color before rotating to fit color property
                    current.parent.color = BLACK;
                    current.parent.parent.color = RED;

                    rightRotation(current.parent.parent); // Rotate The Grandfather
                }
            }
            else // Current On Left Sub-Tree
            {
                Node<E> uncle = current.parent.parent.leftChild; // Make Temp Node Equal To Uncle

                if(uncle.color == RED)
                {
                    current.parent.color = BLACK;
                    uncle.color = BLACK;
                    current.parent.parent.color = RED;
                    current = current.parent.parent; // Point To Grandfather Now
                }
                else
                {
                    if(current.equals(current.parent.leftChild)) // If Node Equals Sibling Or Itself
                    {
                        // Rotate The Parent
                        current = current.parent;
                        rightRotation(current);
                    }

                    // Re-color before rotating to fit color property
                    current.parent.color = BLACK;
                    current.parent.parent.color = RED;

                    leftRotation(current.parent.parent); // Rotate The Grandfather
                }
            }
        }

        root.color = BLACK; // To Make Sure The Root Is Always Black, If It Changes To Red
    }

    private void leftRotation(Node<E> x)
    {
        Node<E> current = x; // Item To Be Left Rotated
        Node<E> temp = current.rightChild;

        current.rightChild = temp.leftChild;

        if(!temp.leftChild.equals(emptyNode))
        {
            temp.leftChild.parent = current;
        }

        temp.parent = current.parent;

        if(current.parent.equals(emptyNode))
        {
            root = temp;
        }
        else if(current.equals(current.parent.leftChild))
        {
            current.parent.leftChild = temp;
        }
        else
        {
            current.parent.rightChild = temp;
        }

        temp.leftChild = current;
        current.parent = temp;
    }

    private void rightRotation(Node<E> x)
    {
        Node<E> current = x; // Item To Be Right Rotated
        Node<E> temp = current.leftChild;
        current.leftChild = temp.rightChild;

        if(!temp.rightChild.equals(emptyNode))
        {
            temp.rightChild.parent = current;
        }

        temp.parent = current.parent;

        if(current.parent.equals(emptyNode))
        {
            root = temp;
        }
        else if(current.equals(current.parent.rightChild))
        {
            current.parent.rightChild = temp;
        }
        else
        {
            current.parent.leftChild = temp;
        }

        temp.rightChild = current;
        current.parent = temp;
    }

    private Node<E> contains(Node<E> c, Comparable<E> object) // Traverse Through Tree To See If Element Exists
    {
        Node<E> current = c;

        while(current != emptyNode && !object.equals(current.element))
        {
            if(object.compareTo(current.element) < 0)
            {
                current = current.leftChild;
            }
            else
            {
                current = current.rightChild;
            }
        }

        return c;
    }

    private StringJoiner preOrderPrint(Node<E> c, StringJoiner s)
    {
        Node<E> current = c;

        if(current != null && current.element != null)
        {
            s.add(current.toString());
            preOrderPrint(current.leftChild, s);
            preOrderPrint(current.rightChild, s);
        }

        return s;
    }
}
