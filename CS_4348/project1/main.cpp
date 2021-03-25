// Project:      Project 1
// Filename:     main.cpp
// Date:         3/25/21
// Author:       Sanjeev Penupala, Gautam Sapre
// Course:       CS 4348.006
// 
// Description:
// 
//      We are given two giant numbers n1 and n2 (each say 200 digits). We need to find the product of these
//      two numbers n3 = n1 * n2. Implement a solution to this using multithreading.
//
// ----------------------------------------------------------------

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Global Variables
vector<string> results;

// IMPORTS //
string add(string str1, string str2) // find the sum of two integer strings
{
    // Grab length of the strings
    string addition = "";
    int n1 = str1.length();
    int n2 = str2.length();
    
    // Reverse both strings
    reverse(str1.begin(), str1.end());
    reverse(str2.begin(), str2.end());
    
    // Compute sum of current digits and carry
    int carry = 0;
    for (int i = 0; i < n1; i++)
    {
        int t = ((str1[i] - '0') + (str2[i] - '0') + carry);
        addition.push_back(t % 10 + '0');
        carry = t / 10; // calculate the carry for next calculation
    }
  
    // Add remaining digits of larger number
    for (int i = n1; i < n2; i++)
    {
        int t = ((str2[i] - '0') + carry);
        addition.push_back(t % 10 + '0');
        carry = t / 10;
    }
  
    // Add remaining carry
    if (carry)
        addition.push_back(carry + '0');
  
    // Reverse the result string
    reverse(addition.begin(), addition.end());
  
    return addition;
}

void multiply(string num1, string num2) // Multiplies str1 and str2, and prints result.
{
    int len1 = num1.size();
    int len2 = num2.size();
    if (len1 == 0 || len2 == 0)
        return results.push_back("0");

    // Create result vector
    vector<int> result(len1 + len2, 0);

    // Indexes to find positions in the result vector
    int index1 = 0;
    int index2 = 0;

    // Go from right to left in num1
    for (int i = len1 - 1; i >= 0; i--)
    {
        int carry = 0;
        int n1 = num1[i] - '0';

        // To shift position to left after every multiplication of a digit in num2
        index2 = 0;       
        for (int j = len2 - 1; j >= 0; j--)
        {
            int n2 = num2[j] - '0'; // Take current digit of second number

            // Multiply with current digit of first number and add result to previously stored result at current position. 
            int temp = ((n1 * n2) + result[index1 + index2] + carry);

            carry = temp / 10;                      // Carry for next iteration
            result[index1 + index2] = temp % 10;    // Store result
            index2++;
        }
  
        // Store carry in next cell
        if (carry > 0)
            result[index1 + index2] += carry;
            
        index1++;
    }
  
    // ignore '0's from the right
    int k = result.size() - 1;
    while (k >= 0 && result[k] == 0)
        k--;

    // If all were '0's - means either both or one of num1 or num2 were '0'
    if (k == -1)
        return results.push_back("0");
        
    // generate the result string
    string s = "";
    while (k >= 0)
        s += to_string(result[k--]);
         
    return results.push_back(s);
}

// Divide a string among n threads
vector<string> divideString(string str, int n)
{
    int str_size = str.size();

    vector<string> parts; // push parts in reverse order
  
    // If length of string is smaller than number of threads
    if (str_size < n)
        return divideString(str, str_size);

    else if (str_size % n != 0) // Not evenly divisible
    {
        int part_size = (str_size / n) + 1;
        int parts_index = 0;
        for (int i = str_size; i > 0; i -= part_size)
        {
            int start_index = max(i - part_size, 0), end_index = i;
            string partial_num = start_index != 0 ? str.substr(start_index, part_size) : str.substr(start_index, end_index);

            for(int z = 0; z < parts_index * part_size; z++)
                partial_num += "0";

            parts.push_back(partial_num);
            parts_index++;
        }
    }
    else // Evenly divisible
    {
        int part_size = str_size / n;
        int parts_index = 0;
        for (int i = str_size; i > 0; i -= part_size)
        {
            int start_index = (i - part_size);
            string partial_num = str.substr(start_index, part_size);

            for(int z = 0; z < parts_index * part_size; z++)
                partial_num += "0";

            parts.push_back(partial_num);
            parts_index++;
        }
    }
    
    return parts;
}

// Driver code
int main()
{
    // Take input from "input.txt"
    vector<string> input;
    string line = "";
    ifstream ifile ("input.txt");

    if (ifile.is_open())
    {
        while (getline(ifile, line))
            input.push_back(line);

        ifile.close();
    }

    int number1[input[1].size()];
    int number2[input[2].size()];

    // Put inputs into an unsigned integer array
    for (int g = 0; g < input[1].size(); g++)
        number1[g] = (int)input[1].at(g) - 48;

    for (int g = 0; g < input[2].size(); g++)
        number2[g] = (int)input[2].at(g) - 48;
    
    // Initialize variables
    int thread_count = stoi(input[0]);
    string str1 = input[1].size() > input[2].size() ? input[1] : input[2]; // bigger or equal length string
    string str2 = input[1].size() > input[2].size() ? input[2] : input[1]; // lesser or equal length string
    
    // Handle negative numbers
    if ((str1.at(0) == '-' || str2.at(0) == '-') && (str1.at(0) != '-' || str2.at(0) != '-' ))
        cout << "-";
    if (str1.at(0) == '-' && str2.at(0) != '-')
        str1 = str1.substr(1);
    else if(str1.at(0) != '-' && str2.at(0) == '-')
        str2 = str2.substr(1);
    else if(str1.at(0) == '-' && str2.at(0) == '-')
    {
        str1 = str1.substr(1);
        str2 = str2.substr(1);
    }

    vector<thread> threads;
    vector<string> parts = divideString(str1, thread_count); // split biggest string into n thread parts
    
    // Create all the threads and store in a vector
    for(auto part: parts)
        threads.push_back(thread(multiply, str2, part));
        
    // execute all threads
    for (auto &th : threads)
        th.join();

    // Add up all numbers in results vector
    string sum = "";
    for (auto result : results)
    {
        // Choose str2 to be the larger string
        if (result.length() > sum.length())
            sum = add(sum, result);
        else
            sum = add(result, sum);
    }
    
    // product Array is created and stores partial sums
    int z;
    int productArray[sum.size()];
    for (z = 0; z < sum.size(); z++)
        productArray[z] = (int)sum.at(z) - 48 ;
    
    // print long number in output file
    ofstream ofile;
    ofile.open("output.txt");
    if (ofile.is_open())
    {
        for (z = 0; z<sum.size(); z++)
            ofile<<productArray[z];

        ofile << "\n";
        ofile.close();
    }

    return 0;
}