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
from collections import deque

# Third Party Library Imports
from bs4 import BeautifulSoup
import requests


#############
# FUNCTIONS #
#############

def relevant_url(topic, link):
    blacklist = ['google', 'imdb', 'pdf', 'php']
    
    if not link.startswith('http'):
        return False
    if topic not in link:
        return False
    if any(b in link for b in blacklist):
        return False
    if 'wikipedia' in link and not 'en.wikipedia' in link:
        return False
    
    return True
    
def web_crawler(topic, starter_url, count):
    relevant_urls = set()
    urls = deque([starter_url])
    
    while len(relevant_urls) < count:
        url = urls.popleft()
        
        # Check if URL is accessible and/or has content
        try:
            r = requests.get(url)
            if r.status_code != 200:
                continue
        except requests.exceptions.RequestException:
            continue
        
        soup = BeautifulSoup(r.text, 'html.parser')
    
        for link in soup.find_all('a'):
            link_str = str(link.get('href')).lower()
            
            # Edit Scraped Link
            if link_str.startswith('/url?q='):
                link_str = link_str[7:]
                print('MOD:', link_str)
            if '&' in link_str:
                i = link_str.find('&')
                link_str = link_str[:i] if i != -1 else link_str
            
            # Check if relevant URL
            if not relevant_url(topic=topic, link=link_str):
                continue
            
            urls.append(link_str)
        
        relevant_urls.add(url)
        
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
    topic = 'volleyball'
    starter_url = 'https://en.wikipedia.org/wiki/Volleyball'
    count = 20
    relevant_urls = web_crawler(topic=topic.lower(), starter_url=starter_url.lower(), count=count)
    
    for idx, url in enumerate(relevant_urls):
        print(f'{idx + 1}. {url}')
    
    
if __name__ == '__main__':
    main()