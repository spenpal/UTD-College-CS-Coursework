# Filename:     main.py
# Date:         4/7/21
# Authors:      Sanjeev Penupala, Sanjana Sivakumar
# Email:        sanjeev.penupala@utdallas.edu, sxs170002@utdallas.edu
# Course:       CS 4365.001
#
# Description:
#
#       Implement a theorem prover for a clause logic using the resolution principle
#

###################
# IMPORTS/GLOBALS #
###################

# Standard Library Imports
from collections import namedtuple
import sys

Clause = namedtuple('Clause', 'literals resolution')

#############
# FUNCTIONS #
#############

def flip(literal):
    """Negate a literal

    Args:
        literal (str): a literal from a clause

    Returns:
        str: negation of a literal
    """
    return literal[1:] if literal.startswith('~') else f'~{literal}'

def parse_kb(file_path):
    """Parse a knowledge base file into effficient Python dictionary
    
    Python Dictionary Format:
        key: sorted clause (in tuple form)
        value: namedtuple of clause and its resolution numbers

    Args:
        file_path (str): path to knowledge base

    Returns:
        Dict: Encoded Python dictionary for knowledge base
    """
    with open(file_path, 'r', encoding='utf-8') as f:
        clauses = f.readlines()
        clauses = [clause.strip() for clause in clauses] # strip away any newlines from clauses
    
    clauses = [clause.split() for clause in clauses]
    re_test_clause = [[flip(literal)] for literal in clauses[-1]] # negate the test clause
    del clauses[-1]
    clauses.extend(re_test_clause) # add the literals of the test clause as their own clauses
    
    kb = {tuple(sorted(clause)): Clause(clause, None) for clause in clauses} # efficient data structure for knowledge base
    
    return kb

def tautology(clause):
    """Find if a clause is a tautology (always evaulates to true)
    
    The only case where a disjunctive clause will be a tautology is 
    if a literal and its negated literal are in the same clause

    Args:
        clause (List): disjuncted literals in the clause

    Returns:
        bool: Truth value if the clause is a tautology
    """
    lit_idx = {}
    
    for idx, literal in enumerate(clause):
        raw_lit = literal.replace('~', '') # remove negation symbol if there is one
        
        # Check if two raw literals are actual opposite literals
        if clause[lit_idx.get(raw_lit, idx)] != literal:
            return True
        else: # Add raw literal to dict
            lit_idx[raw_lit] = idx
    
    return False

def remove_repeats(clause):
    """Remove any repeated literals in a clause

    Args:
        clause (List): disjuncted literals in the clause

    Returns:
        List: unique list of literals (order-preserved)
    """
    return [*dict.fromkeys(clause)]

def resolve(c1, c2):
    """Apply resolution method to two given clauses. 
    Return a resolved clause, if two clauses can be resolved
    Else return False, indicating there is a contradiction

    Args:
        c1 (List): disjuncted literals in the clause
        c2 (List): disjuncted literals in the clause

    Returns:
        List: resolved clause from two clauses
    """
    c1, c2 = c1.copy(), c2.copy() # create a copy of the clauses
    for lit1 in c1:
        if flip(lit1) in c2: # if negated literal is in other clause
            # Remove literal and negated literal from both clauses
            c1.remove(lit1)
            c2.remove(flip(lit1))
            resolved_clause = c1 + c2 # combine clauses
            return resolved_clause if resolved_clause else False # return resolved clause unless it is empty
                
    return None
        
def resolution_prod(kb):
    """Procedure for the resolution principle on a given knowledge base

    Args:
        kb (Dict): knowledge base

    Returns:
        bool: if a contradiction can be found or not
    """
    # convert current knowledge base to a list of tuples [(key, value) pairs]
    # since we can't loop and add keys through a dictionary simultaenously, a list is preferred to do this
    kb_lst = list(kb.items())
    
    for i, _ in enumerate(kb_lst):
        for j in range(i):
            clause_i, clause_j = kb_lst[i][1], kb_lst[j][1]
            resolved_clause = resolve(clause_i.literals, clause_j.literals)
            
            # If the resolved clause is a list of literals
            if resolved_clause:
                resolved_clause = remove_repeats(resolved_clause) # remove repeated literals in resolved clause
                sorted_clause = tuple(sorted(resolved_clause)) # key for resolved clause in knowledge base
                
                # if there are no logically equivalent clauses in knowledge base AND
                # the resolved clause is not a tautology
                if not (sorted_clause in kb or tautology(resolved_clause)):
                    resolution_nums = (i+1, j+1) if i >= j else (j+1, i+1) # Order resolution numbers
                    resolved_clause = Clause(resolved_clause, resolution_nums)
                    kb[sorted_clause] = resolved_clause # put resolved clause in knowledge base
                    kb_lst.append((sorted_clause, resolved_clause)) # add key, value pair to iterating list
                    
            # If a contradiction was found (empty list)
            elif resolved_clause == False:
                resolution_nums = (i+1, j+1) if i >= j else (j+1, i+1) # Order resolution numbers
                resolved_clause = Clause('Contradiction', resolution_nums)
                kb['Contradiction'] = resolved_clause # put contradiction in knowledge base
                return True
    
    return False
                
def print_kb(kb):
    """Print knowledge base in a specified format

    Args:
        kb (Dict): knowledge base
    """
    for idx, clause in enumerate(kb.values()):
        printable = []
        printable.append(f'{idx + 1}.') # add number of the clause
        
        # if clause is a string, this means this is a contradiction statement
        if isinstance(clause.literals, str):
            printable.append(clause.literals)
        else:
            printable.append(' '.join(clause.literals))
        
        # print out resolution numbers for a clause, if they exist
        if clause.resolution:
            rc1, rc2 = clause.resolution
            printable.append(f'{{{rc1}, {rc2}}}')
        else:
            printable.append('{}')
        
        print(' '.join(printable))


########
# MAIN #
########

def main():
    # System argument check
    if len(sys.argv) != 2:
        sys.exit(f'ERROR: Need (only) file path to the kb.')
        
    kb = parse_kb(sys.argv[1]) # parse given file for knowledge base
    
    # Run resolution principle
    if resolution_prod(kb):
        print_kb(kb)
        print('Valid', end='')
    else:
        print_kb(kb)
        print('Fail', end='')
        
if __name__ == '__main__':
    main()