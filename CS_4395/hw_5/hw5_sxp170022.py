# Filename:     hw5_sxp170022.py
# Date:         2/21/21
# Author:       Sanjeev Penupala
# Email:        sanjeev.penupala@utdallas.edu
# Course:       CS 4395.0W1
# Copyright     2021, All Rights Reserved
#
# Description:
#
#       Create a knowledge base scraped from the web. 
#       This knowledge base will be used in a later homework to create a chatbot that can carry 
#       on a limited conversation in a particular domain using the knowledge base, 
#       as well as knowledge it learns from the user.
#

###########
# IMPORTS #
###########

# Standard Library Imports


# Third Party Library Imports
from bs4 import BeautifulSoup
import requests


#############
# FUNCTIONS #
#############

def web_crawler(topic, starter_url):
    r = requests.get(starter_url)
    soup = BeautifulSoup(r.text, 'html.parser')
    relevant_urls = []
    
    blacklist = ['google', 'imdb', 'pdf', 'php']
    for link in set(soup.find_all('a')):
        link_str = str(link.get('href')).lower()
        
        # Edit Scraped Link
        if link_str.startswith('/url?q='):
            link_str = link_str[7:]
            print('MOD:', link_str)
        if '&' in link_str:
            i = link_str.find('&')
            link_str = link_str[:i]
        
        # Check if relevant URL
        if topic not in link_str:
            continue
        if not link_str.startswith('http'):
            continue
        if any(b in link_str for b in blacklist):
            continue
        if 'wikipedia' in link_str and not 'en.wikipedia' in link_str:
            continue
        
        # Check if URL is accessible and/or has content
        try:
            status = requests.get(link_str).status_code
            if status != 200:
                continue
        except requests.exceptions.RequestException:
            continue
        
        relevant_urls.append(link_str)
        
    return relevant_urls
        

def web_scraper():
    pass

def preprocessing():
    pass

def important_terms():
    pass


########
# MAIN #
########

def main():
    topic = 'magic'
    starter_url = 'https://en.wikipedia.org/wiki/Magic_(illusion)'
    
    relevant_urls = web_crawler(topic, starter_url)
    
    for relevant in relevant_urls:
        print(relevant)
    
    
if __name__ == '__main__':
    main()