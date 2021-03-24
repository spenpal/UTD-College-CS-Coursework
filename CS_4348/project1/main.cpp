// C++ program to multiply two numbers represented
// as strings.
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Global Variables
vector<int> results;

// Multiplies str1 and str2, and prints result.
void multiply(string num1, string num2)
{
    int len1 = num1.size();
    int len2 = num2.size();
    if (len1 == 0 || len2 == 0)
    {
        return results.push_back(0);
    }
        
    // will keep the result number in vector
    // in reverse order
    vector<int> result(len1 + len2, 0);
  
    // Below two indexes are used to find positions
    // in result.
    int i_n1 = 0;
    int i_n2 = 0;
      
    // Go from right to left in num1
    for (int i = len1 - 1; i >= 0; i--)
    {
        int carry = 0;
        int n1 = num1[i] - '0';
  
        // To shift position to left after every
        // multiplication of a digit in num2
        i_n2 = 0;
          
        // Go from right to left in num2             
        for (int j=len2-1; j>=0; j--)
        {
            // Take current digit of second number
            int n2 = num2[j] - '0';
  
            // Multiply with current digit of first number
            // and add result to previously stored result
            // at current position. 
            int sum = (n1 * n2) + result[i_n1 + i_n2] + carry;
  
            // Carry for next iteration
            carry = sum / 10;
  
            // Store result
            result[i_n1 + i_n2] = sum % 10;
  
            i_n2++;
        }
  
        // store carry in next cell
        if (carry > 0)
        {
            result[i_n1 + i_n2] += carry;
        }
            
        // To shift position to left after every
        // multiplication of a digit in num1.
        i_n1++;
    }
  
    // ignore '0's from the right
    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0)
    {
        i--;
    }
        
    // If all were '0's - means either both or
    // one of num1 or num2 were '0'
    if (i == -1)
    {
        return results.push_back(0);
    }
        
    // generate the result string
    long s = 0;
    int index = 0, multipler = 1;
    while (index <= i)
    {
        s += result[index] * multipler;
        multipler *= 10;
        index++;
    }
        
    return results.push_back(s);
}

// Divide a string among n threads
vector<string> divideString(string str, int n)
{
    int str_size = str.size();

    vector<string> parts(n, ""); // push parts in reverse order
  
    // If length of string is smaller than number of threads
    if (str_size == str_size % n)
    {
        int parts_index = 0;
        for(int i = str_size - 1; i >= 0; i--)
        {
            string partial_num = str[i] + "";
            for(int z = 0; z < parts_index; z++)
                partial_num += "0";
            parts[parts_index++] = partial_num;
        }
    }
    else if (str_size % n != 0) // Not evenly divisible
    {
        int part_size = str_size / n + 1;
        int parts_index = 0;
        for (int i = str_size; i > 0; i -= part_size)
        {
            int start_index = max(i - part_size, 0), end_index = i;
            string partial_num = str.substr(start_index, end_index);
            for(int z = 0; z < parts_index * part_size; z++)
                partial_num += "0"; 
            parts[parts_index++] = partial_num;
        }
    }
    else // Evenly divisible
    {
        int part_size = str_size / n;
        int parts_index = 0;
        for (int i = str_size; i > 0; i -= part_size)
        {
            int start_index = (i - part_size), end_index = i;
            string partial_num = str.substr(start_index, end_index);
            for(int z = 0; z < parts_index * part_size; z++)
                partial_num += "0";
            parts[parts_index++] = partial_num;
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
        {
            input.push_back(line);
        }

        ifile.close();
    }

    // Initialize variables
    int thread_count = stoi(input[0]);
    string str1 = input[1].size() > input[2].size() ? input[1] : input[2]; // bigger or equal length string
    string str2 = input[1].size() > input[2].size() ? input[2] : input[1]; // lesser or equal length string
    
    // Handle negative numbers
    if ((str1.at(0) == '-' || str2.at(0) == '-') && (str1.at(0) != '-' || str2.at(0) != '-' ))
    {
        cout << "-";
    }
    if (str1.at(0) == '-' && str2.at(0) != '-')
    {
        str1 = str1.substr(1);
    }
    else if(str1.at(0) != '-' && str2.at(0) == '-')
    {
        str2 = str2.substr(1);
    }
    else if(str1.at(0) == '-' && str2.at(0) == '-')
    {
        str1 = str1.substr(1);
        str2 = str2.substr(1);
    }

    vector<thread> threads;
    vector<string> parts = divideString(str1, thread_count); // split biggest string into n thread parts

    for(auto part: parts)
    {
        threads.push_back(thread(multiply, str2, part));
    }

    // execute all threads
    for (auto &th : threads) 
    {
        th.join();
    }

    // Add up all numbers in results vector
    long sum = 0;
    for (auto result : results)
    {
        cout << result << "\n";
        sum += result;
    }

    // print long number in output file
    ofstream ofile;
    ofile.open("output.txt");
    if (ofile.is_open())
    {
        ofile << sum;
        ofile.close();
    }

    return 0;
}