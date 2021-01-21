# Filename:     hw1_sxp170022.py
# Date:         1/20/21
# Author:       Sanjeev Penupala
# Email:        sanjeev.penupala@utdallas.edu
# Course:       CS 4395.0W1
# Copyright     2021, All Rights Reserved
#
# Description:  
#
#       An employee file has been created with an obsolete system. 
#       Your task is to:
#           1. read in the file
#           2. process the text to be more standardized as described below3
#           3. create an object for each person with corrections from the user
#           4. output each person’s information. 
#


###########
# IMPORTS #
###########

# Standard Library Imports
import pathlib
import pickle
import re
import sys


########
# MAIN #
########
        
class Person():
    
    def __init__(self, last, first, middle, id, phone):
        self.last = last
        self.first = first
        self.middle = middle
        self.id = id
        self.phone = phone
        
    def display(self):
        print(f'Employee ID: {self.id}')
        print(f'{self.first} {self.middle} {self.last}')
        print(f'{self.phone}')
        print()


def valid_phone_number(phone):
    """Checks if the phone number given is valid (regardless of delimiter).

    Args:
        phone (str)): Phone number

    Returns:
        bool: Phone number has 10 digits or not
    """
    return sum(1 for c in phone if c.isdigit()) == 10


def format_phone_number(phone):
    """Put valid phone number in 'XXX-XXX-XXXX' format

    Args:
        phone (str): Phone number

    Returns:
        str: Formatted phone number
    """
    area, prefix, _ = re.findall(r'\d{3}', phone)
    line_num, = re.findall(r'\d{4}$', phone)
    return "-".join([area, prefix, line_num])
    
    
def process_input(path):
    """Process the data file to a more standardized format and prepare for displaying.
    Ask user for correct data, if one or more data rows is incorrect

    Args:
        path (str): Path to data file

    Returns:
        Dict: Dictionary of employees (key -> employee ID: value -> Person())
    """
    # Read in .csv file and delete header
    with open(path, 'r') as f:
        lines = f.readlines()
        del lines[0]
    
    persons = {}
    
    # Go through all rows in .csv file
    for line in enumerate(lines):
        
        # Order of each row: [last (0), first (1), middle (2), id (3), phone (4)]
        # Perform some preprocessing of each data row
        info = line.split(',')
        info = [line.strip() for line in info]  # Get rid of leading or trailing whitespaces
        info[0], info[1] = info[0].capitalize(), info[1].capitalize()
        info[2] = info[2].upper() if info[2] else 'X'
        info[3]= info[3].upper()
        
        # If ID is valid (2 letters, 4 numbers)
        while not re.search(r'[A-Z]{2}\d{4}', info[3]):
            print(f'The following ID is not valid: {info[3]}')
            print('Make sure IDs are 2 letters followed by 4 digits')
            info[3] = input("Re-enter correct ID: ").strip().upper()
        
        # If phone number is valid (i.e has 10 digits in it)
        while not valid_phone_number(info[4]):
            print(f'The following phone number is not valid: {info[4]}')
            print('Make sure you are not entering the country code')
            info[4] = input("Re-enter correct phone number: ").strip()
        
        # Format phone number using '-'
        info[4] = format_phone_number(info[4])
        
        person = Person(*info)
        
        # If duplicate ID is found from data file
        if info[3] in persons:
            print(f'ERROR: Duplicate ID found - {info[3]}')
        else:
            persons[info[3]] = person
            
    return persons


def main():
    # If no file path is given
    if len(sys.argv) < 2:
        sys.exit(f'ERROR: Insufficient arguments. Enter a file path to the data')
    
    rel_path = sys.argv[1]
    path = pathlib.Path.cwd().joinpath(rel_path)    # Get absolute path to data file
    employees = process_input(path)                 # Dictionary of employees
    
    # Practice using pickles
    pickle.dump(employees, open('employees.pickle', 'wb'))
    employees = pickle.load(open('employees.pickle', 'rb'))
    
    # Display all employees from data file
    print('----------------------------------------------------------------')
    print('Employees List')
    print('----------------------------------------------------------------')
    for employee in employees.values():
        employee.display()


if __name__ == '__main__':
    main()