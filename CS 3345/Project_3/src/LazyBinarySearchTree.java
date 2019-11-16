import com.sun.source.tree.Tree;

public class LazyBinarySearchTree
{
    /**
     * TreeNode is a node in the BST data structure that stores the key (number) and pointers to its left and right childs
     */
    class TreeNode
    {
        // Instance Variables;
        int key;
        TreeNode leftChild;
        TreeNode rightChild;
        boolean deleted;
        int depth; // Distance Of Node From The Root

        // Constructor
        TreeNode(int k, int d)
        {
            key = k;
            leftChild = null;
            rightChild = null;
            deleted = false;
            depth = d;
        }
    }

    // Instance Variables
    private TreeNode root;
    private int size; // # Of Nodes In The Tree
    private final int  MIN_KEY  = 1; // Lowest Possible Value For Keys Inserted
    private final int MAX_KEY  = 99; // Highest Possible Value For Keys Inserted
    private int height; // Height Of The Tree

    LazyBinarySearchTree()
    {
        root = null;
        size = 0;
        height = 0;
    }

    /**
     * insert() should insert a new element to a leaf node.
     * The valid set of keys is all integers in the range [1,99]
     * If the new element would be a duplicate of a non-deleted element already in the tree, then insert should do nothing
     * However, if the new element is not a duplicate of a non-deleted element, but is a duplicate of a deleted element,
     * then insert should “un-delete” the deleted element in-place rather than physically inserting a new copy of the element
     * The return value of insert should indicate whether insert logically (as opposed to physically) inserted a new element.
     *
     * @param   key Number Being Inserted Into BST
     * @return      Boolean value whether the insertion was successfully or not
     */
    public boolean insert(int key) throws IllegalArgumentException
    {
        if(key >= MIN_KEY && key <= MAX_KEY)
        {
            TreeNode current = root;

            // If The Tree Is Empty, Insert At Root
            if(current == null)
            {
                root = new TreeNode(key, 0);
                size++;
                return true;
            }

            int depthCounter = 0; // Initialize Count To Depth Of Node From Root

            // Traverse Through Tree To Find Correct Insertion
            while(key != current.key)
            {
                // Key Is On Left-Side
                if(key < current.key)
                {
                    // If No Childs On Left...
                    if(current.leftChild == null)
                    {
                        if(++depthCounter > height)
                        {
                            height = depthCounter; // Update The New Height Of Tree, If New Node Has Greater Depth
                        }

                        current.leftChild = new TreeNode(key, depthCounter);
                        size++;
                        return true;
                    }

                    current = current.leftChild;
                    depthCounter++;
                }
                else if(key > current.key) // Key Is On Right-Side
                {
                    if(current.rightChild == null)
                    {
                        if(++depthCounter > height)
                        {
                            height = depthCounter;
                        }

                        current.rightChild = new TreeNode(key, depthCounter);
                        size++;
                        return true;
                    }

                    current = current.rightChild;
                    depthCounter++;
                }
            }
            // If Node Is Deleted, "Un-Delete" It
            if(current.deleted)
            {
                current.deleted = false;
                return true;
            }
        }
        else
        {
            throw new IllegalArgumentException("Key Is Not Range");
        }

        return false;
    }

    /**
     * delete() should not physically remove an element from the tree
     * Rather, it should mark the specified element as logically deleted
     * If the specified element is not in the tree or is already marked as deleted, then delete should do nothing
     * The return value of delete should indicate whether delete logically deleted an element.
     *
     * @param   key Number Being Lazily Deleted From The BST
     * @return      Boolean value whether the deletion was successfully or not
     * @throws IllegalArgumentException
     */
    public boolean delete(int key) throws IllegalArgumentException
    {
        if(key >= MIN_KEY && key <= MAX_KEY)
        {
            TreeNode current = root;

            while(current != null && key != current.key)
            {
                if(key < current.key) // Key Is On Left-Side
                {
                    current = current.leftChild;
                }
                else if(key > current.key) // Key Is On Left-Side
                {
                    current = current.rightChild;
                }
            }

            if(current != null && !current.deleted) // Lazy Delete
            {
                current.deleted = true;
                return true;
            }
        }
        else
        {
            throw new IllegalArgumentException("Key Is Not Range");
        }

        return false;
    }

    /**
     * findMin() should return the value of the minimum non-deleted element, or -1 if none exists.
     * @return      Minimum Of The BST Tree
     */
    public int findMin()
    {
        if(root == null) // If Tree Is Empty
        {
            return -1;
        }

        TreeNode current = root;
        return findMin(current);
    }

    /**
     * Helper Function Of findMin()
     * @param x     Node At Given Location
     * @return      Minimum Of The BST Tree
     */
    public int findMin(TreeNode x)
    {
        if (x == null || x.deleted)
        {
            return Integer.MAX_VALUE;
        }

        int parent = x.key;
        int left = findMin(x.leftChild);
        int right = findMin(x.rightChild);

        if (left < parent)
        {
            parent = left;
        }
        if (right < parent)
        {
            parent = right;
        }

        return parent;
    }

    /**
     * findMax() should return the value of the maximum non-deleted element, or -1 if none exists
     * @return      The Maximum Of The BST Tree
     */
    public int findMax()
    {
        if(root == null) // If Tree Is Empty
        {
            return -1;
        }

        TreeNode current = root;
        return findMax(current);
    }

    /**
     * Helper Function For findMax()
     * @param x     Node At Given Location (Start At Root)
     * @return      Maximum Of The BST Tree
     */
    public int findMax(TreeNode x)
    {
        if (x == null || x.deleted)
        {
            return Integer.MIN_VALUE;
        }

        int parent = x.key;
        int left = findMax(x.leftChild);
        int right = findMax(x.rightChild);

        if (left > parent && !x.deleted)
        {
            parent = left;
        }
        if (right > parent && !x.deleted)
        {
            parent = right;
        }

        return parent;
    }

    /**
     * contains() should return whether the given element both exists in the tree and is non-deleted.
     *
     * @param key   Number To Be Found From The BST
     * @return      Boolean value whether the deletion was successfully or not
     * @throws IllegalArgumentException
     */
    public boolean contains(int key) throws IllegalArgumentException
    {
        if(key >= MIN_KEY && key <= MAX_KEY)
        {
            TreeNode current = root;

            while(current != null && key != current.key)
            {
                if(key < current.key)
                {
                    current = current.leftChild;
                }
                else if(key > current.key)
                {
                    current = current.rightChild;
                }
            }
            if(current != null && !current.deleted)
            {
                return true;
            }
        }
        else
        {
            throw new IllegalArgumentException("Key Is Not Range");
        }

        return false;
    }

    /**
     * toString() should perform an pre-order traversal of the tree and print the value of each element, including elements marked as deleted
     * However, elements that are marked as deleted should be preceded by a single asterisk
     * Every pair of adjacent elements should be separated by whitespace in the printing,
     * but no whitespace should occur between an asterisk and the element with which it is associated.
     *
     * @return Pre-Order Printing Of the BST Tree
     */
    public String toString()
    {
        String output = ""; // Output Builder Variable
        TreeNode current = root; // Pointer To Traverse Through The Tree
        output = print(current).trim();
        return output;
    }

    /**
     * Helper Method To Perform Recursive Pre-Order Traversal On The Tree
     * @param x     Node At A Given Location (Start With Root)
     * @return      Pre-Order Printing Of the BST Tree
     */
    public String print(TreeNode x)
    {
        if(x == null)
        {
            return "";
        }
        else
        {
            if(x.deleted)
            {
                return " *" + x.key + print(x.leftChild) + print(x.rightChild);
            }
            else
            {
                return " " + x.key + print(x.leftChild) + print(x.rightChild);
            }
        }
    }

    /**
     * height() should return the height of the tree, including “deleted” elements
     *
     * @return      Height Of The BST
     */
    public int height()
    {
        return height;
    }

    /**
     * size() should return the count of elements in the tree, including “deleted” ones
     *
     * @return      # Of Nodes In the BST
     */
    public int size()
    {
        return size;
    }
}