#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// #include "misc.h"
using namespace std;

class SpellChecker {
 private:
  class TrieNode {
   public:
    bool endOfWord;
    vector<TrieNode *> children;

    TrieNode() {
      endOfWord = false;
      children = vector<TrieNode*>(26, NULL);
    }
  };
  TrieNode *root;

 public:
  SpellChecker(string filePath) {
    root = new TrieNode();
    ifstream dictFile(filePath.c_str());

    string word;
    while (getline(dictFile, word)) {
      insertWord(word);
    }
    dictFile.close();
  }
  void insertWord(string &word) {
    TrieNode *currentNode = root;
    for (int i = 0; i < word.length(); i++) {
      char c = word[i];
      int index = tolower(c) - 'a';
      if (!currentNode->children[index]) {
        currentNode->children[index] = new TrieNode();
      }
      currentNode = currentNode->children[index];
    }
    currentNode->endOfWord = true;
  }

  bool searchWord(string &word) {
    TrieNode *currentNode = root;
    for (int i = 0; i < word.length(); i++) {
      char c = word[i];
      if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) continue;
      int index = c - 'a';
      if (!currentNode->children[index]) {
        return false;
      }
      currentNode = currentNode->children[index];
    }
    return (currentNode && currentNode->endOfWord);
  }

  vector<string> getSuggestions(string &word) {
    vector<string> suggestions;

    // Modify one char
    for (int i = 0; i < word.length(); ++i) {
      for (char c = 'a'; c <= 'z'; c++) {
        string modified = word;
        modified[i] = c;
        if (searchWord(modified)) {
          suggestions.push_back(modified);
        }
      }
    }

    // Insert one char
    for (int i = 0; i <= word.length(); ++i) {
      for (char c = 'a'; c <= 'z'; c++) {
        string modified = word;
        modified.insert(i, 1, c);
        if (searchWord(modified)) {
          suggestions.push_back(modified);
        }
      }
    }

    // Delete one char
    for (int i = 0; i < word.length(); ++i) {
      string modified = word;
      modified.erase(i, 1);
      if (searchWord(modified)) {
        suggestions.push_back(modified);
      }
    }

    // Swap one char
    for (int i = 0; i < word.length() - 1; ++i) {
      string modified = word;
      swap(modified[i], modified[i + 1]);
      if (searchWord(modified)) {
        suggestions.push_back(modified);
      }
    }

    return suggestions;
  }
};

void testClass() {
  SpellChecker spellchecker("dictionary.txt");

  string sentence;
  cout << "Enter a sentence: ";
  getline(cin, sentence);
  for (int i = 0; i < sentence.length(); i++) {
    sentence[i] = tolower(sentence[i]);
  }

  stringstream ss(sentence);
  vector<string> misspelledWords;

  string word;
  while (ss >> word) {
    if (!spellchecker.searchWord(word)) {
      setColor(Red, Black);
      misspelledWords.push_back(word);
    }
    cout << word;
    setColor(White, Black);
    cout << " ";
  }
  cout << endl;

  for (int i = 0; i < misspelledWords.size(); i++) {
    string misspelledWord = misspelledWords[i];
    vector<string> suggestions = spellchecker.getSuggestions(misspelledWord);
    if (!suggestions.empty()) {
      cout << "Suggestions for " << misspelledWord << ": ";
      for (int i = 0; i < suggestions.size(); i++) {
        cout << suggestions[i] << " ";
      }
      cout << endl;
    }
  }
}
