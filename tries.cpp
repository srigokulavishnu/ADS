#include <iostream>
#include <unordered_map>
#include <vector>
#include<climits>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
    }
};

class Trie {
private:
    TrieNode* root;

    void getWords(TrieNode* node, string prefix, vector<string>& suggestions) {
        if (node->isEndOfWord) {
            suggestions.push_back(prefix);
        }
        for (auto& child : node->children) {
            getWords(child.second, prefix + child.first, suggestions);
        }
    }

    string findClosestMatch(const string& word) {
        string bestMatch;
        int minDiff = INT_MAX;
        vector<string> suggestions = autoComplete("");
        for (const string& suggestion : suggestions) {
            int diff = 0;
            for (size_t i = 0; i < min(word.size(), suggestion.size()); i++) {
                if (word[i] != suggestion[i]) {
                    diff++;
                }
            }
            diff += abs((int)(word.size() - suggestion.size()));
            if (diff < minDiff) {
                minDiff = diff;
                bestMatch = suggestion;
            }
        }
        return bestMatch;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                return false;
            }
            current = current->children[ch];
        }
        return current->isEndOfWord;
    }

    vector<string> autoComplete(const string& prefix) {
        TrieNode* current = root;
        vector<string> suggestions;

        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return suggestions; // No words with this prefix
            }
            current = current->children[ch];
        }
        getWords(current, prefix, suggestions);
        return suggestions;
    }

    string spellCheck(const string& word) {
        if (search(word)) {
            return "Word is correct.";
        } else {
            return "Did you mean: " + findClosestMatch(word) + "?";
        }
    }
};

int main() {
    Trie trie;
    int n;
    cout << "Enter number of words to insert: ";
    cin >> n;
    cout << "Enter words: " << endl;
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        trie.insert(word);
    }

    string word;
    cout << "Enter a word to search: ";
    cin >> word;
    cout << trie.spellCheck(word) << endl;

    string prefix;
    cout << "Enter a prefix for auto-complete suggestions: ";
    cin >> prefix;
    vector<string> suggestions = trie.autoComplete(prefix);

    if (!suggestions.empty()) {
        cout << "Auto-complete suggestions: " << endl;
        for (const string& suggestion : suggestions) {
            cout << suggestion << endl;
        }
    } else {
        cout << "No suggestions found." << endl;
    }
    return 0;
}

