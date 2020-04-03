import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class MyItem implements IDedObject
{
    private int itemID;
    private int itemPrice;
    private String s = "";
    private List<Integer> itemDescription = new LinkedList<Integer>();

    MyItem()
    {
        itemID = 0;
        itemPrice = 0;
        itemDescription.clear();
    }

    MyItem(int itemID, int itemPrice)
    {
        this.itemID = itemID;
        this.itemPrice = itemPrice;
        itemDescription.clear();
    }

    MyItem(int itemID, int itemPrice, List<Integer> itemDescription)
    {
        this.itemID = itemID;
        this.itemPrice = itemPrice;
        importList(itemDescription);
    }

    public void importList(List<Integer> x)
    {
        for(int i = 0; i < x.size(); i++)
        {
            itemDescription.add(x.get(i));
        }
    }

    @Override
    public int getID()
    {
        return itemID;
    }

    public int getItemPrice()
    {
        return itemPrice;
    }

    public List<Integer> getItemDescription()
    {
        return itemDescription;
    }


    @Override
    public String printID()
    {
        String output = "";

        // Print out itemID
        output += getID() + " ";

        // Print out itemPrice
        output += getItemPrice() + " ";

        //Print Out Item Descriptions
        for(int i = 0; i < getItemDescription().size(); i++)
        {
            output += getItemDescription().get(i);


            if(i != getItemDescription().size() - 1)
            {
                output += " ";
            }
        }

        return output;
    }
}
