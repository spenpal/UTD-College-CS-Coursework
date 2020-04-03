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

            String operation;
            boolean result; // Tells If The Operation Was Successful
            String type = in.nextLine(); // Type Of The Object

            // If Not Integer Or String, Then Quit Program With Appropriate Message
            if(type.equals("Integer"))
            {
                RedBlackTree<Integer> tree = new RedBlackTree();
                Integer element; // Element Of A Node

                // Read Through The File
                while (in.hasNextLine())
                {
                    Scanner chopper = new Scanner(in.nextLine());
                    chopper.useDelimiter(":"); // Don't Read Colons In Input File
                    operation = chopper.next();

                    switch(operation)
                    {
                        case "Insert": try
                        {
                            element = chopper.nextInt();
                            result = tree.insert(element);
                            //result = Insert the item into the BST and get true or false
                            out.println(result ? "True" : "False");
                        }
                        catch (Exception e)
                        {
                            out.println("Error In Line: " + operation);
                        }
                            break;
                        case "Contains": try
                        {
                            element = chopper.nextInt();
                            result = tree.contains(element);
                            //result = See if ID contains in BST and get true or false
                            out.println(result ? "True" : "False");
                        }
                        catch (Exception e)
                        {
                            out.println("Error In Line: " + operation);
                        }
                            break;
                        case "PrintTree":
                            out.println(tree.toString());
                            break;
                        default:
                            out.println("Error In Line: " + operation);
                    }
                }
            }
            else if(type.equals("String"))
            {
                RedBlackTree<String> tree = new RedBlackTree();
                String element; // Element Of A Node

                // Read Through The File
                while (in.hasNextLine())
                {
                    Scanner chopper = new Scanner(in.nextLine());
                    chopper.useDelimiter(":"); // Don't Read Colons In Input File
                    operation = chopper.next();

                    switch(operation)
                    {
                        case "Insert": try
                        {
                            element = chopper.next();
                            result = tree.insert(element);
                            //result = Insert the item into the BST and get true or false
                            out.println(result ? "True" : "False");
                        }
                        catch (Exception e)
                        {
                            out.println("Error in insert: IllegalArgumentException raised");
                        }
                            break;
                        case "Contains": try
                        {
                            element = chopper.next();
                            result = tree.contains(element);
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
                        default:
                            out.println("Error In Line: " + operation);
                    }
                }
            }
            else
            {
                System.out.println("Only works for objects Integers and Strings");
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
