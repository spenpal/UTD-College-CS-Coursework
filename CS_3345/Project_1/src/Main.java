/**
 *
 * Name: Sanjeev Penupala
 * Class: CS 3345
 * Section: 001
 * Semester: Fall 2019
 * Project #1: "Use the 'Sieve of Eratosthenes' algorithm to compute all prime numbers up until number N"
 */

import java.util.*;
import java.lang.*;
import static java.lang.System.out;

public class Main
{
    public static void main(String[] args)
    {
        boolean valid = false; // Truth Variable For Valid Input
        Scanner input = new Scanner(System.in);
        out.print("Enter a number :: ");

        while(!valid) // Repeats If There Is No Valid Input For Size
        {
            try
            {
                int n = input.nextInt();

                boolean prime[] = new boolean[n];
                Arrays.fill(prime, true); // Fill All Values In The Array With True

                // Algorithm To Cross Out Non-Prime Numbers
                for(int i = 2; i < Math.sqrt(n); i++)
                {
                    if(prime[i])
                    {
                        for(int j = i*i; j < n; j += i)
                        {
                            if(prime[j])
                            {
                                prime[j] = false;
                            }
                        }
                    }
                }

                // Print Out Prime Numbers
                for(int num = 2; num < prime.length; num++)
                {
                    if(prime[num])
                    {
                        out.print(num + " ");
                    }
                }

                valid = true; // Change Truth Value of Input
            }
            catch(Exception e)
            {
                out.print("Re-enter a proper integer :: ");
                input.nextLine();
            }
        }

        out.println();
    }
}