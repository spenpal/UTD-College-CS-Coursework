/**
 * Name: Sanjeev Penupala
 * Class: CS 3345
 * Section: 001
 * Semester: Fall 2019
 * Project #5: "To implement in Java several variations of the in-place QuickSort algorithm, each with a different choice of pivot"
 */

import java.io.File;
import java.io.PrintWriter;
import java.time.Duration;
import java.util.*;

public class Main
{
    public static void main(String[] args)
    {
        if (args.length!=4)
        {
            System.out.print("Error Incorrect Arguments:" + Arrays.toString(args));
            System.exit(0);
        }
        try
        {
            int size = Integer.parseInt(args[0]);

            File output_file = new File(args[1]);
            PrintWriter report = new PrintWriter(output_file);

            output_file = new File(args[2]);
            PrintWriter unsorted = new PrintWriter(output_file);

            output_file = new File(args[3]);
            PrintWriter sorted = new PrintWriter(output_file);

            // Print the unsorted List to "unsorted.txt" and create clones of the unsortedList for each QuickSort method
            ArrayList<Integer> unsortedList = QuickSorter.generateRandomList(size);
            ArrayList<Integer> unsortedList2 = (ArrayList<Integer>) unsortedList.clone();
            ArrayList<Integer> unsortedList3 = (ArrayList<Integer>) unsortedList.clone();
            ArrayList<Integer> unsortedList4 = (ArrayList<Integer>) unsortedList.clone();

            for(int i = 0; i < unsortedList.size(); i++)
            {
                unsorted.println(unsortedList.get(i));
            }
            unsorted.close();

            /* Testing Java's Library Sorting Algorithms
            Object list[] = unsortedList.toArray();
            long startTime = System.nanoTime();
            Arrays.sort(list);
            System.out.println("Arrays Sort Timing: " + Duration.ofNanos(System.nanoTime()-startTime));

            List newList = (List) unsortedList.clone();
            long startTime2 = System.nanoTime();
            Collections.sort(newList);
            System.out.println("Collections Sort Timing: " + Duration.ofNanos(System.nanoTime()-startTime2));
            */

            // Print Reports Of Different QuickSort Strategies to "report.txt"
            // If All Times Are VERY Similar, Then That Means That Insertion Sort Was Used
            report.println("Array Size = " + size);
            report.println("FIRST_ELEMENT : " + QuickSorter.timedQuickSort(unsortedList, QuickSorter.PivotStrategy.FIRST_ELEMENT));
            report.println("RANDOM_ELEMENT : " + QuickSorter.timedQuickSort(unsortedList2, QuickSorter.PivotStrategy.RANDOM_ELEMENT));
            report.println("MEDIAN_OF_THREE_RANDOM_ELEMENTS : " + QuickSorter.timedQuickSort(unsortedList3, QuickSorter.PivotStrategy.MEDIAN_OF_THREE_RANDOM_ELEMENTS));
            report.println("MEDIAN_OF_THREE_ELEMENTS : " + QuickSorter.timedQuickSort(unsortedList4, QuickSorter.PivotStrategy.MEDIAN_OF_THREE_ELEMENTS));
            report.close();

            // Printed Sorted List to "sorted.txt"
            // System.out.println(Arrays.toString(unsortedList.toArray()));
            List sortedList = unsortedList;
            for(int i = 0; i < sortedList.size(); i++)
            {
                sorted.println(sortedList.get(i));
            }
            sorted.close();
        }
        catch (Exception e)
        {
            System.out.println("Exception: " + e.getMessage());
        }
    }
}
