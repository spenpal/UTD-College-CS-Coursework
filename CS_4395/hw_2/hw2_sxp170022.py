# Filename:     hw2_sxp170022.py
# Date:         1/27/21
# Author:       Sanjeev Penupala
# Email:        sanjeev.penupala@utdallas.edu
# Course:       CS 4395.0W1
# Copyright     2021, All Rights Reserved
#
# Description:
#
#       Objective: Use Python and NLTK features to explore a text file and create a word guessing game
#       Your task is to:
#           1. read in the file
#           2. calculate lexical diversity of file
#           3. preprocess the file with NLTK methods
#           4. find the 50 most common nouns in the file
#           5. create a word guessing game from those list of nouns
#


###########
# IMPORTS #
###########

# Standard Library Imports
from collections import Counter
from random import randint
import pathlib
import sys

# Third Party Library Imports
from nltk import word_tokenize, pos_tag
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer


########
# MAIN #
########

def lexical_diversity(tokens):
    """Calculate the lexical diversity of the tokenized text 

    Args:
        tokens (List): list of tokens from a text

    Returns:
        float: lexical diversity percentage
    """
    return len(set(tokens)) / len(tokens)


def findOccurrences(s, ch):
    """Find all occurences of a certain character in a string

    Args:
        s (str): string you want to search
        ch (str): character you want to find all occurences of

    Returns:
        List: a list of the indices of 's' where 'ch' occured
    """
    return [i for i, letter in enumerate(s) if letter == ch]


def preprocessing(raw_text):
    tokens = word_tokenize(raw_text)
    stpwrds = stopwords.words('english')
    tokens = [t.lower() for t in tokens if t.isalpha() and len(t) > 5 and t not in stpwrds]
    
    wnl = WordNetLemmatizer()
    lemmas = [wnl.lemmatize(t) for t in tokens]
    unique_lemmas = set(lemmas)
    
    tags = pos_tag(unique_lemmas)
    print(f'First 20 POS Tagged Lemmas: {tags[:20]}\n')
    
    unique_nouns = [token for token, tag in tags if tag.startswith('N')]
    
    print(f'Number of tokens: {len(tokens)}')
    print(f'Number of unique nouns: {len(unique_nouns)}\n')
    
    return tokens, unique_nouns
        

def guessing_game(words):
    print('----------------------------------------------------------------')
    print('WELCOME TO THE GUESSING GAME!')
    print()
    print('A random word will be selected and the length of the word will be shown as underscores.')
    print('You starts off with 5 points. Everytime you guess a letter from the word, you get 1 point.')
    print('Everytime you guess a letter incorrectly, you lose 1 point. If you go below 0 points, you are out of luck!')
    print('----------------------------------------------------------------')
    
    pts = 5
    word = words[randint(0, len(words))]
    hidden_word = ['_'] * len(word)
    guesses = set()
    
    while True:
        print(f'Hidden Word: {"".join(hidden_word)}')
        guess = input('Enter a letter guess: ').lower()
        
        if guess == '!':
            break
        elif not guess:
            print('Make sure you are entering a letter guess.\n')
            continue
        elif len(guess) > 1:
            print('You can only enter one letter at a time.\n')
            continue
        elif guess in guesses:
            print(f'Looks like you have already guessed this letter!\n')
            continue
        elif guess in word:
            pts += 1
            print('Right!')
            for i in findOccurrences(word, guess):
                hidden_word[i] = guess
        else:
            pts -= 1
            print(f'Sorry, guess again!')
        
        guesses.add(guess)
        print(f'Current Score: {pts}\n')
        
        if pts < 0 or hidden_word.count('_') == 0:
            break
            
    if guess == '!':
        print('Wow...cannot believe you are a quitter! Oh well...\n')
    elif pts < 0:
        print('Looks you ran out of points. Better luck next time!\n')
    elif hidden_word.count('_') == 0:
        print('Yay! You guessed the word correctly!\n')
    
    print(f'Word: {word}')
    print(f'End Game Score: {pts}')
    
        
def main():
    # If no file path is given
    if len(sys.argv) < 2:
        sys.exit(f'ERROR: Insufficient arguments. Enter a file path to the data.')

    rel_path = sys.argv[1]
    path = pathlib.Path.cwd().joinpath(rel_path)    # Get absolute path to data file
    
    with open(rel_path, 'r') as f:
        raw_text = f.read()
        tokens = word_tokenize(raw_text)
    
    lex_div = lexical_diversity(tokens)
    print(f'Lexical Diversity of "{sys.argv[1]}": {lex_div:.2f}\n')
    
    tokens, nouns = preprocessing(raw_text)
    
    freq_nouns = Counter({noun: tokens.count(noun) for noun in nouns})
    top_50_nouns = freq_nouns.most_common(50)
    print('50 Most Common Nouns')
    for noun, count in top_50_nouns:
        print(f'{noun}: {count}')
    fifty_nouns = [noun for noun, count in top_50_nouns]
    
    guessing_game(fifty_nouns)
        
        
if __name__ == '__main__':
    main()