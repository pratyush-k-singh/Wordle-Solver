#ifndef FETCH_VALID_WORDS_H
#define FETCH_VALID_WORDS_H

#include <set>
#include <string>

// Function to fetch page content
std::string fetch_page_content(int page_number);

// Function to search for words in a GumboNode
void search_for_words(GumboNode* node, std::set<std::string>& words);

// Function to fetch words from a specific page
std::set<std::string> fetch_words_from_page(int page_number);

#endif // FETCH_VALID_WORDS_H
