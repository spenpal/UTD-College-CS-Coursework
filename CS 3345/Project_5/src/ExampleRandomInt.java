import java.util.Random;

public class ExampleRandomInt
{ 
    public static final int COUNT = 100;

    public static void main(String[] args)
    {
        /* According to the JavaDoc, the no-args constructor of Random "sets the seed of the random number generator to
         * a value very likely to be distinct from any other invocation of this constructor."
         */
        Random random = new Random();

        // OPTION 1:  use the nextInt() method
        for (int i = 0; i < COUNT; ++i)
        {
            int num = random.nextInt();
            System.out.println(num);
        }

        // OPTION 2:  use the ints(long) method
        random.ints(COUNT).forEach( num -> System.out.println(num) );
        int min = 1, max = 10;
		for (int i = 0; i < 10; i++)
        {
            System.out.println(rand(min, max));
        }
    }

 /**
	* Generates a pseudo-random integer in the range [min, max]
	* @param min : the starting value of the range (inclusive)
	* @param max : the ending value of the range (inclusive)
     * @return 
	*/
    public static int rand(int min, int max)
	{
		if (min > max || (max - min + 1 > Integer.MAX_VALUE))
		{
			throw new IllegalArgumentException("Invalid range");
		}

		return new Random().nextInt(max - min + 1) + min;
	}

}
