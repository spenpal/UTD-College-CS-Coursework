# Filename:     main.py
# Date:         3/31/21
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
import copy
import operator
import sys

branch_num = 1

#############
# FUNCTIONS #
#############

def parse_var(path):
    var_dict = {}
    
    with open(path, 'r') as f:
        for line in f:
            var = line[0] # get variable name
            vals = line[line.find(':') + 1:].split() # put all values into a list
            try:
                vals = [int(val) for val in vals] # convert values to integers
            except:
                sys.exit('One or more values are not valid integers')
            var_dict[var] = vals # store variable name and values in dictionary
            
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
    global branch_num
    printable = []
    for var, val in assignment.items():
        printable.append(f'{var}={val}')
    branch = f'{branch_num}. {", ".join(printable)}  {"solution" if truth else "failure"}'
    branch_num += 1
    print(branch)

def remove_constraints(vars_, cons):
    for var in vars_:
        cons.pop(var, None) # remove assigned variable from list of constraints

        for _, cons_lst in cons.items():
            for i in range(len(cons_lst) - 1, -1, -1):
                con = cons_lst[i]
                if var == con.var2: # if you see assigned variable in other constraints, remove that constraint as well
                    cons_lst.remove(con)
                   
    return cons
            
def select_unassigned_variable(assignment, csp):
    vars_ = csp.get('vars')
    un_vars = set(vars_) - set(assignment) # get all unassigned variables
    most_con_var, min_dom_size = [], float('inf')
    
    # Find most constrained variable(s)
    for un_var in un_vars:
        if len(vars_.get(un_var, 0)) < min_dom_size:
            most_con_var = [un_var]
            min_dom_size = len(vars_.get(un_var, 0))
        elif len(vars_.get(un_var, 0)) == min_dom_size:
            most_con_var.append(un_var)

    # If there is only one most constrained variable, return that
    if len(most_con_var) == 1:
        return most_con_var[0]
    else:
        # Use most constraining variable heuristic
        most_cons_var, num_of_cons = [], float('-inf')
        # Get rid of constraints that have assigned variables in them
        cons = remove_constraints(set(assignment), copy.deepcopy(csp.get('cons')))
        
        # Find most constraining variable(s)
        for var in most_con_var:
            if len(cons.get(var, 0)) > num_of_cons:
                most_cons_var = [var]
                num_of_cons = len(cons.get(var, 0))
            elif len(cons.get(var, 0)) == num_of_cons:
                most_cons_var.append(var)
        
    # If there is only one most constraining variable, return that
    if len(most_cons_var) == 1:
        return most_cons_var[0]
    else:
        # break ties in sorted order (alphabetical order)
        return sorted(most_cons_var)[0]
                
def order_domain_values(var, assignment, csp):
    vars_ = csp.get('vars')
    cons = remove_constraints(set(assignment), copy.deepcopy(csp.get('cons')))
    lcv = dict.fromkeys(vars_.get(var), 0) # pre-populate dictionary with var's values
    
    for val1 in vars_.get(var, ()):
        lcv_pts = 0
        for con in cons.get(var, ()):
            for val2 in vars_.get(con.var2):
                lcv_pts += con.op(val1, val2) # if operation returns true, add one to total lcv pts
        lcv[val1] = lcv_pts # store lcv pts for a certain value
    
    # return least value with highest lcv pts (sorted order)
    for v in sorted(lcv.items(), key=lambda x: (-x[1], x[0])):
        yield v[0]

def forward_check(var, val, assignment, csp):
    csp = copy.deepcopy(csp)
    vars_, cons = csp.get('vars'), remove_constraints(set(assignment), copy.deepcopy(csp.get('cons')))
    
    for con in cons.get(var, ()):
        var2 = vars_.get(con.var2)
        for i in range(len(var2) - 1, -1, -1):
            val2 = var2[i]
            if not con.op(val, val2): # if values between variables are violated, remove from domain
                var2.remove(val2)
    
    return csp
    
def check_consistency(var, val, assignment, csp):
    vars_, cons = csp.get('vars'), csp.get('cons')
    
    # Check if any constraints are violated
    for con in cons.get(var, ()):
        if con.var2 in assignment:
            if not con.op(val, assignment.get(con.var2)):
                return False
    
    # Check if any domains are empty
    if any(len(vals) == 0 for vals in vars_.values()):
        return False
       
    return True
    
def backtracking_search(csp, prod):
    if prod == 'fc': # forward checking
        return recursive_backtracking_fc({}, csp)
    else: # regular backtracking
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

def recursive_backtracking_fc(assignment, csp):
    # if assignment is complete
    if len(assignment) == len(csp.get('vars')):
        return assignment

    # select an unassigned variable using most constrained variable heuristic
    var = select_unassigned_variable(assignment, csp)
    
    # order each variable's value by least constraining value heuristic
    for val in order_domain_values(var, assignment, csp):
        # If value fits the csp's constraints
        new_csp = forward_check(var, val, assignment, csp)
        if check_consistency(var, val, assignment, new_csp):
            assignment[var] = val                                       # add value to current assignment
            result = recursive_backtracking_fc(assignment, new_csp)     # repeat backtracking
            if result:
                return result                                           # return solution
            del assignment[var]                                         # delete variable from assignment
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
    assignment = backtracking_search(csp, prod)
    if assignment:
        print_assignment(assignment, True)
    
if __name__ == '__main__':
    main()