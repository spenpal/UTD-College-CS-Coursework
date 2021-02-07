# Filename:     hw3_sxp170022.py
# Date:         2/5/21
# Author:       Sanjeev Penupala
# Email:        sanjeev.penupala@utdallas.edu
# Course:       CS 4395.0W1
# Copyright     2021, All Rights Reserved
#
# Description:
#
#       Use Python, regex, and NLTK to process text.
#


###########
# IMPORTS #
###########

# Standard Library Imports
from collections import Counter, OrderedDict
from pprint import pprint
import pathlib
import re
import sys

# Third Party Library Imports
from nltk import word_tokenize, pos_tag
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer


########
# MAIN #
########

def edit_distance(s1, s2):
    """The Levenshtein distance is a string metric for measuring the difference
    between two sequences.
    It is calculated as the minimum number of single-character edits necessary to
    transform one string into another.

    Args:
        s1 (str): string 1
        s2 (str): string 2

    Returns:
        int: min. number of single character edits
    """
    matrix = [[0] * (len(s2) + 1) for _ in range(len(s1) + 1)]
    
    for r in range(1, len(s1) + 1):
        matrix[r][0] = r
    
    for c in range(1, len(s2) + 1):
        matrix[0][c] = c
    
    for c in range(1, len(s2) + 1):
        for r in range(1, len(s1) + 1):
            if s1[r-1] == s2[c-1]:
                substitutionCost = 0
            else:
                substitutionCost = 1
            
            matrix[r][c] = min(matrix[r-1][c] + 1, 
                               matrix[r][c-1] + 1,
                               matrix[r-1][c-1] + substitutionCost)
    
    return matrix[-1][-1]
       

def preprocessing(raw_text):
    """Preprocess the raw text using NLTK tools

    Args:
        raw_text (str): text to preprocess

    Returns:
        tuple: tokenized text
    """  
    raw_text = raw_text.lower()                     # lowercase text
    raw_text = raw_text.replace('--', ' ')          # replace all occurrences of '--' with ' '
    raw_text = re.sub(r'\d', '', raw_text)          # remove all digits
    raw_text = re.sub(r'[^\w\s]', ' ', raw_text)    # replace punctuation with a single space
    tokens = word_tokenize(raw_text)                # tokenize text
    return tokens


def statistics(file_name, tokens):
    """Perform various statistic computations on the tokenized text

    Args:
        file_name (str): file name of text
        tokens (List[str]): tokenized text
    """
    print(f'"{file_name}" statistics:')
    print(f'No. of tokens: {len(tokens)}')
    
    unique_tokens = set(tokens)
    print(f'No. of unique tokens: {len(unique_tokens)}')
    
    stop_words = stopwords.words('english')
    important_words = [ut for ut in unique_tokens if ut not in stop_words] # remove stopwords from tokens
    print(f'No. of important tokens: {len(important_words)}')
    
    stemmer = PorterStemmer()
    word_and_stems = [(w, stemmer.stem(w)) for w in important_words] # pair words with their stemmed version
    
    words_of_stems = {} # key: stem word | value: words of that stem
    for word, stem in word_and_stems:
        if stem in words_of_stems:
            words_of_stems[stem].append(word)
        else:
            words_of_stems[stem] = [word]
    print(f'No. of dictionary entries: {len(words_of_stems)}')
    
    sorted_stems = sorted(words_of_stems.items(), key=lambda x: len(x[1]), reverse=True) # sort stems by no. of words
    print()
    print('25 Longest Word Lists of Stems:')
    for stem, words in sorted_stems[:25]:
        print(f'{stem + ":":<10}{words}')
    
    print()
    print('Edit Distance (between all words of stem "continue"):')
    for word in words_of_stems['continu']:
        edit_dist = edit_distance(word, 'continue') # find Levenshtein distance between 'continu' and word
        print(f'{word + ":":<15}{edit_dist}')
    
    print()
    print(f'POS Tag Counts of "{file_name}":')
    pos_tokens = pos_tag(tokens)
    pos_dict = Counter(p[1] for p in pos_tokens) # count up all pos tags
    pprint(pos_dict)
    
    
def main():
    # If no file path is given
    if len(sys.argv) < 2:
        sys.exit(f'ERROR: Insufficient arguments. Enter a file path to the data.')
    
    # Get absolute path to data file
    rel_path = sys.argv[1]
    path = pathlib.Path.cwd().joinpath(rel_path)   
    
    # Tokenize raw text from data file
    with open(path, 'r') as f:
        raw_text = f.read()
    
    file_name = path.parts[-1]
    tokens = preprocessing(raw_text)
    statistics(file_name, tokens)
   
        
if __name__ == '__main__':
    main()