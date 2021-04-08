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
    return literal[1:] if literal.startswith('~') else f'~{literal}'

def parse_kb(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        clauses = f.readlines()
        clauses = [clause.strip() for clause in clauses]
    
    clauses = [clause.split() for clause in clauses]
    re_test_clause = [[flip(literal)] for literal in clauses[-1]]
    del clauses[-1]
    clauses.extend(re_test_clause)
    
    kb = {tuple(sorted(clause)): Clause(clause, None) for clause in clauses}
    
    return kb

def tautology(clause):
    lit_idx = {}
    
    for idx, literal in enumerate(clause):
        raw_lit = literal.replace('~', '')
        
        if raw_lit in lit_idx:
            if clause[lit_idx[raw_lit]] != literal:
                return True
        else:
            lit_idx[raw_lit] = idx
    
    return False

def remove_repeats(clause):
    return [*dict.fromkeys(clause)]

def resolve(c1, c2):
    c1, c2 = c1.copy(), c2.copy()
    for lit1 in c1:
        if flip(lit1) in c2:
            c1.remove(lit1)
            c2.remove(flip(lit1))
            new_clause = c1 + c2
            return new_clause if new_clause else False
                
    return None
        
def resolution_prod(kb):
    kb_lst = list(kb.items())
    for i, _ in enumerate(kb_lst):
        for j in range(i):
            clause_i, clause_j = kb_lst[i][1], kb_lst[j][1]
            new_clause = resolve(clause_i.literals, clause_j.literals)
            
            if new_clause:
                new_clause = remove_repeats(new_clause)
                sorted_clause = tuple(sorted(new_clause))
                if not (sorted_clause in kb or tautology(new_clause)):
                    resolution_nums = (i+1, j+1) if i >= j else (j+1, i+1)
                    new_clause = Clause(new_clause, resolution_nums)
                    kb[sorted_clause] = new_clause
                    kb_lst.append((sorted_clause, new_clause))
            elif new_clause == False:
                resolution_nums = (i+1, j+1) if i >= j else (j+1, i+1)
                new_clause = Clause('Contradiction', resolution_nums)
                kb['Contradiction'] = new_clause
                return True
    
    return False
                
def print_kb(kb):
    for idx, clause in enumerate(kb.values()):
        printable = []
        printable.append(f'{idx + 1}.')
        
        if isinstance(clause.literals, str):
            printable.append(clause.literals)
        else:
            printable.append(' '.join(clause.literals))
            
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
        
    kb = parse_kb(sys.argv[1])
    
    if resolution_prod(kb):
        print_kb(kb)
        print('Valid', end='')
    else:
        print_kb(kb)
        print('Fail', end='')
        
if __name__ == '__main__':
    main()