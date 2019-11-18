Name: Sanjeev Penupala
NetID: sxp170022
Project: #5

Files:
- README.txt
- QuickSorter.java
- Main.java

Details:
- IDE: IntelliJ
- Version: JDK 13

Sample Output:
====================================================================================================
For n = 15 elements...
// Insertion Sort
Array Size = 15
FIRST_ELEMENT : PT0.0000541S
RANDOM_ELEMENT : PT0.0000082S
MEDIAN_OF_THREE_RANDOM_ELEMENTS : PT0.0000095S
MEDIAN_OF_THREE_ELEMENTS : PT0.0000146S

// Arrays.sort()
Arrays Sort Timing: PT0.0002923S

// Collections.sort()
Collections Sort Timing: PT0.0000599S
---------------------------------------------------
- Observations:
 : For n = 15, it uses "Insertion Sort". The timings are not consistent each time my Insertion Sort is ran, but is the fastest sorting algorithm, in this scenario
 : If we try to sort an "array" version of our list, using Arrays.sort(), we can see it takes a significant more time to sort out
 : As for Collections.sort() on a list, it is comparable faster than Arrays.sort(), but not faster than Insertion Sort
====================================================================================================
For n = 250,000 elements...
Array Size = 250000
FIRST_ELEMENT : PT0.1328225S
RANDOM_ELEMENT : PT0.1910331S
MEDIAN_OF_THREE_RANDOM_ELEMENTS : PT0.2325724S
MEDIAN_OF_THREE_ELEMENTS : PT0.1438609S

// Arrays.sort()
Arrays Sort Timing: PT0.1432989S

// Collections.sort()
Collections Sort Timing: PT0.1081677S
---------------------------------------------------
- Observations:
 : For n = 250,000, it uses "Quick Sort". We can see that FIRST_ELEMENT is the fastest for this list of elements.
    Maybe the first element was close to the median of the list, which makes Quick Sort faster to sort a balanced pivot list
 : If we try to sort an "array" version of our list, using Arrays.sort(), we can see it takes about the same time as MEDIAN_OF_THREE_ELEMENTS
 : As for Collections.sort() on a list, it is comparable faster than Arrays.sort() and faster than the QuickSort algorithm
====================================================================================================
For n = 500,000 elements...
// Quick Sort
Array Size = 500000
FIRST_ELEMENT : PT0.419788S
RANDOM_ELEMENT : PT0.1988904S
MEDIAN_OF_THREE_RANDOM_ELEMENTS : PT0.3594526S
MEDIAN_OF_THREE_ELEMENTS : PT0.1899896S

// Arrays.sort()
Arrays Sort Timing: PT0.3278579S

// Collections.sort()
Collections Sort Timing: PT0.1900309S
---------------------------------------------------
- Observations:
 : For n = 500,000, it uses "Quick Sort". We can see that MEDIAN_OF_THREE_ELEMENTS is the fastest for this list of elements
    Maybe the median of the low, mid, and high was close to the true middle of the list, which makes Quick Sort faster to sort a balanced pivot list
 : If we try to sort an "array" version of our list, using Arrays.sort(), we can see it takes a significant more time to sort out
 : As for Collections.sort() on a list, it is comparable faster than Arrays.sort() and is comparably on par with the QuickSort algorithm

