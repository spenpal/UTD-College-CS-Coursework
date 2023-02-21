# Homework 5: Web Scraping
> Author: Sanjeev Penupala (SXP170022)

## Installation
```bash
pip install < requirements.txt
```

## Workflow
- Web crawler grabs as many specificed links as needed
- Web scraper scrapes only paragraph elements, to avoid getting header or footer text
    - The number of the scraped file, in the title, corresponds to its relevant url number, found in *relevant_urls.txt*
- Before finding important terms, manual parsing through cleaned scraped data is done first, to ensure scraped content is quality
    - Moderate changes were made to cleaned scraped files, such as removing redundant lines, non-english characters, and empty files, duplicate files
    - This may be why there may be less cleaned files than scraped files
    - There are a total of **20** cleaned files used to find important terms and compile a knowledge base out of
- Knowlege base is in form of a Python dictionary with keywords being an important term from the corpus, and value being a list of sentences that has the important term in it





