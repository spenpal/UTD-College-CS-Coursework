public class ExampleEnum
{
    public static void main(String[] args)
    {
        // How to create an enum literal
        printColor(Color.RED);

        // How to create an enum variable
        Color color = Color.GREEN;
        printColor(color);

        // You can even print the string representation of an enum value directly
        System.out.println(Color.BLUE.toString());
    }

    public static void printColor(Color color)
    {
        // Using the == operator
        if (color == Color.RED) {
            System.out.println("Your color is red!");
        }
        else if (color == Color.GREEN) {
            System.out.println("Your color is green!");
        }
        else if (color == Color.BLUE) {
            System.out.println("Your color is blue!");
        }

        // Using a switch statement
        switch (color) {
        case RED:
            System.out.println("Your color is red!");
            break;
        case GREEN:
            System.out.println("Your color is green!");
            break;
        case BLUE:
            System.out.println("Your color is blue!");
            break;
        }
    }

    public static enum Color
    {
        RED,
        GREEN,
        BLUE;
    }
}
