# Filename:     program1.py
# Date:         2/10/21
# Author:       Sanjeev Penupala
# Email:        sanjeev.penupala@utdallas.edu
# Course:       CS 4395.0W1
# Copyright     2021, All Rights Reserved
#
# Description:
#
#       Use n-gram models for text analysis.
#


###########
# IMPORTS #
###########

# Standard Library Imports
from collections import Counter
from pprint import pprint
import pickle

# Third Party Library Imports
from nltk import word_tokenize
from nltk.util import ngrams


#############
# FUNCTIONS #
#############

def ngrams_models(filename):
    with open(filename, 'r') as f:
        raw_text = f.read()
        raw_text = raw_text.replace('\n', '')
    
    tokens = word_tokenize(raw_text)
    unigrams = ngrams(tokens, 1)
    bigrams = ngrams(tokens, 2)
    
    unigrams_dict = Counter(unigrams)
    bigrams_dict = Counter(bigrams)
    
    return [unigrams_dict, bigrams_dict]
    
    
########
# MAIN #
########

def main():
    # Get ngrams from data
    eng_dicts = ngrams_models('LangId.train.English')
    fre_dicts = ngrams_models('LangId.train.French')
    ita_dicts = ngrams_models('LangId.train.Italian')
    
    # Dump dicts into pickle
    pickle.dump(eng_dicts, open('english.pickle', 'wb'))
    pickle.dump(fre_dicts, open('french.pickle', 'wb'))
    pickle.dump(ita_dicts, open('italian.pickle', 'wb'))
    
    
if __name__ == '__main__':
    main()