# Filename:     program2.py
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
import os
import pickle

# Third Party Library Imports
from nltk import word_tokenize
from nltk.util import ngrams


#############
# FUNCTIONS #
#############

def accuracy_score(x, y):
    print('Incorrect Classifcation Line No.: ', end='')
    incorrect_lines = []
    correct = 0
    line_num = 1
    
    for a, b in zip(x, y):
        if a == b:
            correct += 1
        else:
            incorrect_lines.append(line_num)
            
        line_num += 1
    
    print(incorrect_lines)
    return correct / len(x)
    

def compute_prob(text, unigrams_dict, bigrams_dict, V):
    tokens = word_tokenize(text)
    bigrams_test = ngrams(tokens, 2)
    
    p_laplace = 1
    
    for bigram in bigrams_test:
        b = bigrams_dict.get(bigram, 0)
        u = unigrams_dict.get(bigram[0], 0)
        p_laplace *= ((b + 1) / (u + V))
        
    return p_laplace
        
        
########
# MAIN #
########

def main():
    # Read in pickled dictionaries
    eng_unigram_dict, eng_bigrams_dict = pickle.load(open('english.pickle', 'rb'))
    fre_unigram_dict, fre_bigrams_dict = pickle.load(open('french.pickle', 'rb'))
    ita_unigram_dict, ita_bigrams_dict = pickle.load(open('italian.pickle', 'rb'))
    
    with open('LangId.test', 'r') as f:
        test_cases = f.readlines()
    
    vocab_size = len(eng_unigram_dict) + len(fre_unigram_dict) + len(ita_unigram_dict)
    
    if os.path.exists('LangId.preds'):
        os.remove('LangId.preds')
        
    with open('LangId.preds', 'a+') as f:
        for idx, test_case in enumerate(test_cases):
            english_prob = compute_prob(test_case, eng_unigram_dict, eng_bigrams_dict, vocab_size)
            french_prob = compute_prob(test_case, fre_unigram_dict, fre_bigrams_dict, vocab_size)
            italian_prob = compute_prob(test_case, ita_unigram_dict, ita_bigrams_dict, vocab_size)
            
            probs = [('English', english_prob), ('French', french_prob), ('Italian', italian_prob)]
            best_lang, best_prob = max(probs, key=lambda x: x[1])
            f.write(f'{idx + 1} {best_lang}\n')
    
    with open('LangId.preds', 'r') as p, open('LangId.sol', 'r') as s:
        preds = p.readlines()
        tests = s.readlines()
    
    print(f'Classification Accuracy: {accuracy_score(preds, tests):.5f}')
            
                 
if __name__ == '__main__':
    main()