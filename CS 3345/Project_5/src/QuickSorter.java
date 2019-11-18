/**
 * Name: Sanjeev Penupala
 * Class: CS 3345
 * Section: 001
 * Semester: Fall 2019
 * Project #5: "To implement in Java several variations of the in-place QuickSort algorithm, each with a different choice of pivot"
 */

import java.time.Duration;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class QuickSorter
{
    /*
     * This private constructor is optional, but it does help to prevent accidental client instantiation of QuickSorter
     * via the default constructor.  (defining any constructor prevents the compiler from creating a default constructor)
     * This particular anti-instantiation technique is exactly what {@link java.util.Collections} does.
     */
    private QuickSorter() { }

    public static <E extends Comparable<E>> Duration timedQuickSort(ArrayList<E> list, PivotStrategy strategy) throws NullPointerException
    {
        if(list == null || strategy == null || list.size() == 0)
        {
            throw new NullPointerException("Arguments cannot be empty or null!");
        }

        if(list.size() < 20)
        {
            return insertionSort(list);
        }
        else
        {
            switch (strategy)
            {
                case FIRST_ELEMENT:
                    return qsFirstElement(list, 0, list.size());
                case RANDOM_ELEMENT:
                    return qsRandomElement(list, 0, list.size());
                case MEDIAN_OF_THREE_RANDOM_ELEMENTS:
                    return qsMedianOfThreeRandomElements(list, 0, list.size());
                case MEDIAN_OF_THREE_ELEMENTS:
                    return qsMedianOfThreeElements(list, 0, list.size());
                default:
                    return null;
            }
        }
    }

    public static ArrayList<Integer> generateRandomList(int size) throws IllegalArgumentException
    {
        if (size < 0)
        {
            throw new IllegalArgumentException("Size must be a positive value!");
        }

        ArrayList<Integer> randomList = new ArrayList<Integer>();
        Random random = new Random();
        random.ints(size, 0, Integer.MAX_VALUE).forEach(randomList::add);
        return randomList;
    }

    public static enum PivotStrategy
    {
        FIRST_ELEMENT,
        RANDOM_ELEMENT,
        MEDIAN_OF_THREE_RANDOM_ELEMENTS,
        MEDIAN_OF_THREE_ELEMENTS
    }

    // Helper Methods
    private static <E extends Comparable<E>> Duration insertionSort(List<E> list)
    {
        long startTime = System.nanoTime();

        for (int i = 1; i < list.size(); i++)
        {
            E key = list.get(i);
            int j = i - 1;

            while (j >= 0 && list.get(j).compareTo(key) > 0)
            {
                list.set(j + 1, list.get(j));
                j = j - 1;
            }
            list.set(j + 1, key);
        }

        return Duration.ofNanos(System.nanoTime()-startTime);
    }

    private static <E extends Comparable<E>> Duration qsFirstElement(List<E> list, int low, int high)
    {
        long startTime = System.nanoTime();

        if (low < high)
        {
            int pi = partition(list, low, high, low); // Partitioning Index | Pivot Is Sorted Right Place

            // Recursively sort elements before partition and after partitioning index
            qsFirstElement(list, low, pi);
            qsFirstElement(list, pi+1, high);
        }

        return Duration.ofNanos(System.nanoTime()-startTime);
    }

    private static <E extends Comparable<E>> Duration qsRandomElement(List<E> list, int low, int high)
    {
        long startTime = System.nanoTime();

        if (low < high)
        {
            int random = rand(low, high);
            swap(list, low, random); // Swap Random Pivot To Beginning Of List

            int pi = partition(list, low, high, low); // Partitioning Index | Pivot Is Sorted Right Place

            // Recursively sort elements before partition and after partitioning index
            qsRandomElement(list, low, pi);
            qsRandomElement(list, pi + 1, high);
        }

        return Duration.ofNanos(System.nanoTime()-startTime);
    }

    private static <E extends Comparable<E>> Duration qsMedianOfThreeRandomElements(List<E> list, int low, int high)
    {
        long startTime = System.nanoTime();

        if (low < high)
        {
            swap(list, low, randM(low, high)); // Swap Random Pivot To Beginning Of List
            int pi = partition(list, low, high, low); // Partitioning Index | Pivot Is Sorted Right Place

            // Recursively sort elements before partition and after partitioning index
            qsMedianOfThreeRandomElements(list, low, pi);
            qsMedianOfThreeRandomElements(list, pi + 1, high);
        }

        return Duration.ofNanos(System.nanoTime()-startTime);
    }

    private static <E extends Comparable<E>> Duration qsMedianOfThreeElements(List<E> list, int low, int high)
    {
        long startTime = System.nanoTime();

        if (low < high)
        {
            swap(list, low, median(low, (low+high)/2, high)); // Swap Random Pivot To Beginning Of List
            int pi = partition(list, low, high, low); // Partitioning Index | Pivot Is Sorted Right Place

            // Recursively sort elements before partition and after partitioning index
            qsMedianOfThreeElements(list, low, pi);
            qsMedianOfThreeElements(list, pi + 1, high);
        }

        return Duration.ofNanos(System.nanoTime()-startTime);
    }

    private static <E extends Comparable<E>> int partition(List<E> list, int low, int high, int p)
    {
        E pivot = list.get(p);
        int i = low; // Index Of Lower Bound Of Unsorted List
        int j = high; // Index Of Higher Bound Of Unsorted List

        while(i < j)
        {
            do // Increment 'i' until you see a number bigger than pivot
            {
                i++;
            }
            while(i != list.size() && list.get(i).compareTo(pivot) <= 0);

            do // Decrement 'j' until you see a number smaller than pivot
            {
                j--;
            }
            while(j != -1 && list.get(j).compareTo(pivot) > 0);

            if(i < j)
            {
                if(j == p)
                {
                    p = i;
                }

                swap(list, i, j);
            }
        }

        // Swap arr[0] (Pivot) with arr[j] (Correct Pivot Location)
        swap(list, p, j);
        return j; // Return pivot location
    }

    // Swap two indices
    private static <E extends Comparable<E>> void swap(List<E> list, int l, int h)
    {
        if(h != -1 && l != list.size() && h != list.size() && l != -1)
        {
            E temp = list.get(l);
            list.set(l, list.get(h));
            list.set(h, temp);
        }
    }

    // Generate one random number
    public static int rand(int min, int max)
    {
        if (min > max || (max - min + 1 > Integer.MAX_VALUE))
        {
            throw new IllegalArgumentException("Invalid range");
        }

        return new Random().nextInt(max - min) + min;
    }

    // Generate random 3 numbers and return median
    public static int randM(int min, int max)
    {
        List<Integer> rands = new ArrayList();
        Random random = new Random();
        random.ints(3, min, max).forEach(rands::add);
        Collections.sort(rands);
        return rands.get(1);
    }

    // Return median of low, mid, and high
    public static int median(int low, int mid, int high)
    {
        List<Integer> med = new ArrayList();
        med.add(low);
        med.add(mid);
        med.add(high);
        Collections.sort(med);
        return med.get(1);
    }
}
