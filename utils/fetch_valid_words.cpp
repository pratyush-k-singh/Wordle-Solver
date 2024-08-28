#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <curl/curl.h>
#include <gumbo.h>
#include <unistd.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        return 0;
    }
    return newLength;
}

std::string fetch_page_content(int page_number) {
    std::string url = "https://www.bestwordlist.com/5letterwords";
    if (page_number > 1) {
        url += "page" + std::to_string(page_number) + ".htm";
    } else {
        url += ".htm";
    }

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

void search_for_words(GumboNode* node, std::set<std::string>& words) {
    if (node->type != GUMBO_NODE_ELEMENT) return;

    GumboAttribute* class_attr;
    if ((node->v.element.tag == GUMBO_TAG_SPAN) && 
        (class_attr = gumbo_get_attribute(&node->v.element.attributes, "class")) &&
        (strcmp(class_attr->value, "mt") == 0 || strcmp(class_attr->value, "rd") == 0)) {

        GumboNode* text = static_cast<GumboNode*>(node->v.element.children.data[0]);
        if (text->type == GUMBO_NODE_TEXT) {
            std::string content = text->v.text.text;
            size_t pos = 0;
            while ((pos = content.find(' ', pos)) != std::string::npos) {
                std::string word = content.substr(0, pos);
                if (word.size() == 5) {
                    words.insert(word);
                }
                content = content.substr(pos + 1);
            }
            if (content.size() == 5) {
                words.insert(content);
            }
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_words(static_cast<GumboNode*>(children->data[i]), words);
    }
}

std::set<std::string> fetch_words_from_page(int page_number) {
    std::string page_content = fetch_page_content(page_number);

    GumboOutput* output = gumbo_parse(page_content.c_str());
    std::set<std::string> words;
    search_for_words(output->root, words);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return words;
}

int main() {
    std::set<std::string> all_words;
    std::string output_file = "../data/valid_words.txt";
    int total_pages = 26;

    for (int page_number = 1; page_number <= total_pages; ++page_number) {
        std::cout << "\rFetching Page " << page_number << " of " << total_pages << ": ["
                  << std::string(page_number * 50 / total_pages, '#')
                  << std::string(50 - (page_number * 50 / total_pages), '.') << "]";
        std::cout.flush();

        std::set<std::string> words = fetch_words_from_page(page_number);
        all_words.insert(words.begin(), words.end());
        sleep(1);
    }

    std::cout << std::endl;

    std::ofstream out(output_file);
    for (const auto& word : all_words) {
        out << word << std::endl;
    }

    std::cout << "Finished fetching words and saved to " << output_file << std::endl;

    return 0;
}
