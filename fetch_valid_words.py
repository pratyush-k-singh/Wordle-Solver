import requests
from bs4 import BeautifulSoup
import time
import os
import sys

def fetch_words_from_page(page_number):
    url = f'https://www.bestwordlist.com/5letterwords{"page" + str(page_number) + ".htm" if page_number > 1 else ".htm"}'
    response = requests.get(url)
    response.raise_for_status()
    
    soup = BeautifulSoup(response.text, 'html.parser')
    
    words = set()
    spans = soup.find_all('span', class_=['mt', 'rd'])
    
    for span in spans:
        text = span.get_text()
        words_list = text.split()
        for word in words_list:
            if len(word) == 5:
                words.add(word.lower())
    
    return words

def main():
    all_words = set()
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_file = os.path.join(script_dir, 'valid_words.txt')
    
    total_pages = 26
    
    for page_number in range(1, total_pages + 1):
        sys.stdout.write(f"\rFetching Page {page_number} of {total_pages}: [{'#' * (page_number * 50 // total_pages)}{'.' * (50 - (page_number * 50 // total_pages))}]")
        sys.stdout.flush()
        words = fetch_words_from_page(page_number)
        all_words.update(words)
        time.sleep(1)
    
    sys.stdout.write("\n")
    
    with open(output_file, 'w') as f:
        for word in sorted(all_words):
            f.write(f"{word}\n")
    
    print(f"Finished fetching words and saved to {output_file}")

if __name__ == "__main__":
    main()
