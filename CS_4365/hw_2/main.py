# Filename:     main.py
# Date:         3/29/21
# Authors:      Sanjeev Penupala, Sanjana Sivakumar
# Email:        sanjeev.penupala@utdallas.edu, sxs170002@utdallas.edu
# Course:       CS 4365.001
#
# Description:
#
#       Implement a CSP solver that takes exactly three arguments from the command line.
#

###########
# IMPORTS #
###########

# Standard Library Imports
from collections import namedtuple
import operator
from pprint import pprint
import sys


#############
# FUNCTIONS #
#############
def parse_var(path):
    var_dict = {}
    
    with open(path, 'r') as f:
        for line in f:
            var_name = line[0] # get variable name
            var_vals = line[line.find(':') + 1:].split() # put all values into a list
            var_vals = [int(var_val) if var_val.isnumeric() else var_val for var_val in var_vals] # convert values to integers
            var_dict[var_name] = var_vals # store variable name and values in dictionary
            
    return var_dict

def parse_con(path):
    Constraint = namedtuple('Constraint', 'var1 var2 op')
    con_dict = {} # stores all constraints in program readable format
    op_func = {
        '=': operator.eq,
        '!': operator.ne,
        '>': operator.gt,
        '<': operator.lt
    } # maps string operator to operator function
    switch_op = {
        '=': '=',
        '!': '!',
        '<': '>',
        '>': '<'
    } # new operator for flipping constraint
    
    with open(path,'r') as f:
        for line in f:
            con = line.split() # split up constraint
            var1, op, var2 = con[0], con[1], con[2]
            
            # Put constraint in namedtuple format
            con1 = Constraint(var1, var2, op_func[op])
            con2 = Constraint(var2, var1, op_func[switch_op[op]])
            
            # Add constraint in OG form to dict
            if con1.var1 in con_dict:
                con_dict[con1.var1].append(con1)
            else:
                con_dict[con1.var1] = [con1]
            
            # Add flipped constraint to dict
            if con2.var1 in con_dict:
                con_dict[con2.var1].append(con2)
            else:
                con_dict[con2.var1] = [con2]
                
    return con_dict

def print_assignment(assignment, truth):
    printable = []
    for var, val in assignment.items():
        printable.append(f'{var}={val}')
    return f'{",".join(printable)}\t{"solution" if truth else "failure"}'

def select_unassigned_variable(assignment, csp):
    pass
    
def order_domain_values(var, assingment, csp):
    pass

def check_consistency(value, assignment, csp):
    pass
    
def backtracking_search(csp):
    return recursive_backtracking({}, csp)

def recursive_backtracking(assignment, csp):
    # if assignment is complete
    if len(assignment) == len(csp.get('vars')):
        return assignment
    
    # select an unassigned variable using most constrained variable heuristic
    var = select_unassigned_variable(assignment, csp)
    
    # order each variable's value by least constraining value heuristic
    for value in order_domain_values(var, assignment, csp):
        # If value fits the csp's constraints
        if check_consistency(value, assignment, csp):
            assignment[var] = value                             # add value to current assignment
            result = recursive_backtracking(assingment, csp)    # repeat backtracking
            if result:
                return result                                   # return solution
            del assignment[var]                                 # delete variable from assignment
        else:
            # Print failure for current assignment
            assignment[var] = value
            print_assignment(assignment, False)
            del assignment[var]
            
    return None


########
# MAIN #
########

def main():
    # Read in args from command line
    if len(sys.argv) != 4:
        sys.exit("Need exactly 3 arguments: path to .var, path to .con, and consistency enforcing procedure")
        
    # Unpack command line args
    var_path, con_path, prod = sys.argv[1], sys.argv[2], sys.argv[3]
    
    # Parse .var and .con files and convert them into dicts
    var_dict = parse_var(var_path)
    con_dict = parse_con(con_path)
    
    # Call backtracking and find a solution
    csp = {'vars': var_dict, 'cons': con_dict}
    assignment = backtracking_search(csp)
    if assignment:
        print_assignment(assignment, True)
    
if __name__ == '__main__':
    main()