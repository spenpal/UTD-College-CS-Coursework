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
from collections import Counter, namedtuple
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
    branch = f'{", ".join(printable)}\t{"solution" if truth else "failure"}'
    print(branch)

def remove_constraints(vars_, cons):
    for var in vars_:
        cons.pop(var, None)
        
        for var, cons_lst in cons.items():
            for con in cons_lst:
                if con.var2 == var:
                    cons_lst.remove(con)
                   
    return cons
            
def select_unassigned_variable(assignment, csp):
    vars_ = csp.get('vars')
    un_vars = set(vars_) - set(assignment)
    most_con_var, min_dom_size = [], float('inf')
    
    # Find most constrained variable
    for un_var in un_vars:
        if len(vars_.get(un_var, 0)) < min_dom_size: # TODO: Remove 0 from .get() statement, if u think so
            most_con_var = [un_var]
            min_dom_size = len(vars_.get(un_var, 0))
        elif len(vars_.get(un_var, 0)) == min_dom_size:
            most_con_var.append(un_var)

    # If there is only one most constrained variable
    if len(most_con_var) == 1:
        return most_con_var[0]
    else:
        # Use most constraining variable heuristic
        most_cons_var, num_of_cons = [], float('-inf')
        cons = remove_constraints(set(assignment), csp.get('cons').copy())
        
        for var in most_con_var:
            if len(cons.get(var, 0)) > num_of_cons:
                most_cons_var = [var]
                num_of_cons = len(cons.get(var, 0))
            elif len(cons.get(var, 0)) == num_of_cons:
                most_cons_var.append(var)
        
    # If there is only one most constraining variable
    if len(most_cons_var) == 1:
        return most_cons_var[0]
    else:
        # break ties in sorted order
        return sorted(most_cons_var)[0]
                
def order_domain_values(var, assignment, csp):
    vars_ = csp.get('vars')
    cons = remove_constraints(set(assignment), csp.get('cons').copy())
    lcv = Counter()
    
    for val1 in vars_.get(var):
        lcv_pts = 0
        for con in cons.get(var):
            for val2 in vars_.get(con.var2):
                lcv_pts += con.op(val1, val2)
        lcv[val1] = lcv_pts
       
    return (v[0] for v in lcv.most_common())

def check_consistency(var, val, assignment, csp):
    cons = csp.get('cons')
    
    for con in cons.get(var):
        if con.var2 in assignment:
            if not con.op(val, assignment.get(con.var2)):
                return False
        
    return True
    
def backtracking_search(csp):
    return recursive_backtracking({}, csp)

def recursive_backtracking(assignment, csp):
    # if assignment is complete
    if len(assignment) == len(csp.get('vars')):
        return assignment
    
    # select an unassigned variable using most constrained variable heuristic
    var = select_unassigned_variable(assignment, csp)
    
    # order each variable's value by least constraining value heuristic
    for val in order_domain_values(var, assignment, csp):
        # If value fits the csp's constraints
        if check_consistency(var, val, assignment, csp):
            assignment[var] = val                               # add value to current assignment
            result = recursive_backtracking(assignment, csp)    # repeat backtracking
            if result:
                return result                                   # return solution
            del assignment[var]                                 # delete variable from assignment
        else:
            # Print failure for current assignment
            assignment[var] = val
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