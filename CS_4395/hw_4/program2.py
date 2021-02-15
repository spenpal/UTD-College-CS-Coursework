# Filename:     program2.py
# Date:         2/13/21
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
    """Return accuracy score from predictions and test list
    Print line numbers of inaccurate predictions

    Args:
        x (List): prediction or test file
        y (List): prediction or test file

    Returns:
        int: accuracy score
    """
    print('Incorrect Classification Line Nums: ', end='')
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
    """Calculate the laplace probability that a text belongs to a given unigram/bigram language model

    Args:
        text (str): text to analyze
        unigrams_dict (Dict): unigram model (for a certain language)
        bigrams_dict (Dict): bigram model (for a certain language)
        V (int): vocabulary size from all corpora

    Returns:
        int: Laplace probability
    """
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
    # Read in language models (from pickled files)
    eng_unigram_dict, eng_bigrams_dict = pickle.load(open('english.pickle', 'rb'))
    fre_unigram_dict, fre_bigrams_dict = pickle.load(open('french.pickle', 'rb'))
    ita_unigram_dict, ita_bigrams_dict = pickle.load(open('italian.pickle', 'rb'))
    
    # Read in test text file
    with open('LangId.test', 'r', encoding='latin1') as f:
        test_cases = f.readlines()
    
    # Get vocab size from all languages
    vocab_size = len(eng_unigram_dict) + len(fre_unigram_dict) + len(ita_unigram_dict)
    
    if os.path.exists('LangId.preds'):
        os.remove('LangId.preds')
        
    with open('LangId.preds', 'a+') as f:
        # Calculate laplace probability for which language a line belongs to and write to predictions file
        for idx, test_case in enumerate(test_cases):
            test_case = test_case.replace('\n', '')
            
            english_prob = compute_prob(test_case, eng_unigram_dict, eng_bigrams_dict, vocab_size)
            french_prob = compute_prob(test_case, fre_unigram_dict, fre_bigrams_dict, vocab_size)
            italian_prob = compute_prob(test_case, ita_unigram_dict, ita_bigrams_dict, vocab_size)

            probs = [('English', english_prob), ('French', french_prob), ('Italian', italian_prob)]
            best_lang, best_prob = max(probs, key=lambda x: x[1]) # get max probability for language
            f.write(f'{idx + 1} {best_lang}\n') # write best language to predictions file
    
    # Read in correct answers file
    with open('LangId.preds', 'r') as p, open('LangId.sol', 'r') as s:
        preds = p.readlines()
        tests = s.readlines()
    
    print(f'Classification Accuracy: {accuracy_score(preds, tests):.5f}')
            
                 
if __name__ == '__main__':
    main()