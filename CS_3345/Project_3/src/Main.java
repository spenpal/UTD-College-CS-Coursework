import java.io.File;
import java.io.PrintWriter;
import java.util.*;
import java.util.Scanner;

public class Main
{
    public static void main(String[] args)
    {
        if (args.length!=2)
        {
            System.out.print("Error Incorrect Arguments:" + Arrays.toString(args));
            System.exit(0);
        }

        try
        {
            File input_file = new File(args[0]);
            Scanner in = new Scanner(input_file);

            File output_file = new File(args[1]);
            PrintWriter out;
            out = new PrintWriter(output_file);

            String operation = "";
            int id; // Element Of A Node
            boolean result; // Tells If The Operation Was Successful

            LazyBinarySearchTree tree = new LazyBinarySearchTree();

            while (in.hasNextLine())
            {
                Scanner chopper = new Scanner(in.nextLine());
                chopper.useDelimiter(":"); // Don't Read Colons In Input File
                operation = chopper.next();

                switch(operation)
                {
                    case "Insert": try
                    {
                        id = chopper.nextInt();
                        result = tree.insert(id);
                        // result = Insert the item into the BST and get true or false
                        out.println(result ? "True" : "False");
                    }
                    catch (Exception e)
                    {
                        out.println("Error in insert: IllegalArgumentException raised");
                    }
                        break;
                    case "Delete": try
                    {
                        id = chopper.nextInt();
                        result = tree.delete(id);
                        //result = Lazy delete the item into the BST and get true or false
                        out.println(result ? "True" : "False");
                    }
                    catch (Exception e)
                    {
                        out.println("Error in delete: IllegalArgumentException raised");
                    }
                        break;
                    case "FindMin":
                        int min = tree.findMin();
                        out.println(min);
                        break;
                    case "FindMax":
                        int max = tree.findMax();
                        out.println(max);
                        break;
                    case "Contains": try
                    {
                        id = chopper.nextInt();
                        result = tree.contains(id);
                        //result = See if ID contains in BST and get true or false
                        out.println(result ? "True" : "False");
                    }
                    catch (Exception e)
                    {
                        out.println("Error in contains: IllegalArgumentException raised");
                    }
                        break;
                    case "PrintTree":
                        out.println(tree.toString());
                        break;
                    case "Height":
                        out.println(tree.height());
                        break;
                    case "Size":
                        out.println(tree.size());
                        break;
                    default:
                        out.println("Error In Line: " + operation);
                }
            }

            in.close();
            out.close();
        }
        catch(Exception e)
        {
            System.out.println("Exception: " + e.getMessage());
        }
    }
}