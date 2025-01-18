#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <map>

using namespace std;

string remove_duplicates(const string& input) {
    unordered_set<char> seen; // To track seen characters
    string result;
    for (char c : input) {
        if (seen.find(c) == seen.end()) { // If the character is not in the set
            result += c;                 // Add it to the result
            seen.insert(c);              // Mark it as seen
        }
    }
    return result;
}

// Function to perform radix sort on a string
vector<char> radix_sort(const string& s, const string& alphabet) {
    int n = alphabet.size();
    unordered_map<char, int> char_to_index;
    for (int i = 0; i < n; ++i) {
        char_to_index[alphabet[i]] = i;
    }

    vector<int> count(n, 0);
    for (char c : s) {
        count[char_to_index[c]]++;
    }

    vector<char> sorted_string;
    for (int i = 0; i < n; ++i) {
        sorted_string.insert(sorted_string.end(), count[i], alphabet[i]);
    }

    return sorted_string;
}

// Function to construct a minimal alphabet from a string
pair<unordered_map<char, char>, vector<char>> construct_minimal_alphabet(const string& s, const string& initial_alphabet = "$abcdefghijklmnopqrstuvwxyz") {
    auto s_no_duplicates = remove_duplicates(s);
    vector<char> unique_letters_sorted = radix_sort(string(s_no_duplicates.begin(), s_no_duplicates.end()), initial_alphabet);

    unordered_map<char, char> minimal_alphabet_encode;
    vector<char> minimal_alphabet;

    char letter_to_use = '$';
    for (size_t i = 0; i < unique_letters_sorted.size(); ++i) {
        minimal_alphabet_encode[unique_letters_sorted[i]] = letter_to_use;
        minimal_alphabet.push_back(letter_to_use);
        // cout << letter_to_use << endl;
        if (letter_to_use == '$') {
            letter_to_use = 'a';
        } else {
            letter_to_use++;
        }
    }

    return {minimal_alphabet_encode, minimal_alphabet};
}

// Function to encode a string with a minimal alphabet
string minimal_string_encoding(const string& s, const unordered_map<char, char>& minimal_alphabet_encode) {
    string new_s;
    for (char c : s) {
        if (minimal_alphabet_encode.find(c) == minimal_alphabet_encode.end()) {
            cerr << "Character '" << c << "' not in minimal alphabet" << endl;
            return "";
        }
        new_s += minimal_alphabet_encode.at(c);
    }
    return new_s;
}

// Function to decode a string using a minimal alphabet map
string decode_string(const string& s, const unordered_map<char, char>& minimal_alphabet_decode) {
    string new_s;
    for (char c : s) {
        new_s += minimal_alphabet_decode.at(c);
    }
    return new_s;
}

// Function to construct the suffix array in linear time
vector<int> suffix_array_construction(const string& s) {
    int n = s.size();
    vector<int> suffix_array(n);
    vector<int> rank(n);

    for (int i = 0; i < n; ++i) {
        suffix_array[i] = i;
        rank[i] = s[i];
    }

    int k = 1;
    auto cmp = [&](int i, int j) {
        if (rank[i] != rank[j]) return rank[i] < rank[j];
        int ri = (i + k < n) ? rank[i + k] : -1;
        int rj = (j + k < n) ? rank[j + k] : -1;
        return ri < rj;
    };

    while (k < n) {
        sort(suffix_array.begin(), suffix_array.end(), cmp);

        vector<int> tmp(n, 0);
        for (int i = 1; i < n; ++i) {
            tmp[suffix_array[i]] = tmp[suffix_array[i - 1]];
            if (cmp(suffix_array[i - 1], suffix_array[i])) {
                tmp[suffix_array[i]]++;
            }
        }
        rank = tmp;
        k *= 2;
    }

    return suffix_array;
}

// Function to perform Burrows-Wheeler Transform
string burrows_wheeler_transform(const string& s) {
    vector<int> suffix_array = suffix_array_construction(s);
    string bwt;
    for (int i : suffix_array) {
        bwt += (i == 0) ? '$' : s[i - 1];
    }
    return bwt;
}

// Function to compute letter occurrences and indexed BWT
pair<unordered_map<char, int>, vector<pair<char, int>>> letter_occurrences_and_indexed_bwt(const string& bwt) {
    unordered_map<char, int> letters_count;
    vector<pair<char, int>> indexed_bwt;

    for (char c : bwt) {
        if (letters_count.find(c) == letters_count.end()) {
            letters_count[c] = 0;
        }
        indexed_bwt.emplace_back(c, letters_count[c]);
        letters_count[c]++;
    }

    return {letters_count, indexed_bwt};
}

// Function to compute prefix sums
unordered_map<char, int> compute_prefix_sums(const unordered_map<char, int>& letter_count, const vector<char>& alphabet) {
    unordered_map<char, int> prefix_sums;
    int previous_sum = 0;

    for (char c : alphabet) {
        prefix_sums[c] = previous_sum;
        previous_sum += letter_count.at(c);
    }

    return prefix_sums;
}

// Function to calculate rank of characters in BWT
unordered_map<char, vector<int>> calculate_rank(const string& bwt, const vector<char>& alphabet) {
    unordered_map<char, vector<int>> rank;
    int n = bwt.size();

    for (char c : alphabet) {
        vector<int> c_occ(n, 0);
        int counter = 0;
        for (int i = 0; i < n; ++i) {
            if (bwt[i] == c) {
                counter++;
            }
            c_occ[i] = counter;
        }
        rank[c] = c_occ;
    }

    return rank;
}

// Function to perform backward search
pair<int, int> backwards_search(const string& p_enc, const vector<pair<char, int>>& indexed_bwt, const unordered_map<char, int>& prefix_sums, char last_char_in_alphabet, const unordered_map<char, vector<int>>& rank) {
    int n = indexed_bwt.size();
    int a = 0, b = 0;

    for (int i = p_enc.size() - 1; i >= 0; --i) {
        char letter = p_enc[i];

        if (i == p_enc.size() - 1) {
            a = prefix_sums.at(letter);
            int next_sum;
            if (letter == last_char_in_alphabet) {
                next_sum = n;
            } else {
                next_sum = (letter == '$') ? prefix_sums.at('a') : prefix_sums.at(letter + 1);
            }
            b = next_sum - 1;
            ;
        } else {
            a = prefix_sums.at(letter) + ((a > 0) ? rank.at(letter)[a - 1] : 0);
            b = prefix_sums.at(letter) + rank.at(letter)[b] - 1;
        }

        if (b < a) return {-1, -1};
    }

    return {a, b};
}

vector<pair<char, int>> reverse_indexed_bwt(const vector<pair<char, int>>& indexed_bwt, const unordered_map<char, int>& prefix_sums) {
    int n = indexed_bwt.size();
    vector<pair<char, int>> indexed_s(n);

    int i = 0;
    pair<char, int> current_letter = {'$', 0}; // Start with the sentinel character

    for (int j = 0; j < n; ++j) {
        indexed_s[j] = current_letter;
        current_letter = indexed_bwt[i];
        i = prefix_sums.at(current_letter.first) + current_letter.second;
    }

    reverse(indexed_s.begin(), indexed_s.end());
    return indexed_s;
}

vector<pair<char, int>> start_letter_occurrences(int start, int end, char first_pattern_letter, const unordered_map<char, int>& prefix_sums) {
    int diff = end - start + 1;
    vector<pair<char, int>> occurrences;

    for (int i = 0; i < diff; ++i) {
        occurrences.emplace_back(first_pattern_letter, start - prefix_sums.at(first_pattern_letter) + i);
    }

    return occurrences;
}

// Function to compute initial string occurrences
vector<int> initial_string_occurrences(const vector<pair<char, int>>& indexed_s, const vector<pair<char, int>>& start_letter_occurrences) {
    vector<int> occurrences;

    for (size_t i = 0; i < indexed_s.size(); ++i) {
        if (find(start_letter_occurrences.begin(), start_letter_occurrences.end(), indexed_s[i]) != start_letter_occurrences.end()) {
            occurrences.push_back(i);
        }
    }

    return occurrences;
}

// Function to check occurrences against the original string
// vector<string> check_occurrences(const vector<int>& initial_s_occurrences, const string& s, const string& p) {
//     vector<string> matches;
//     int m = p.size();

//     for (int index : initial_s_occurrences) {
//         matches.push_back(s.substr(index, m));
//     }

//     return matches;
// }

vector<string> check_occurrences(const vector<int>& initial_s_occurrences, const string& s, const string& p) {
    vector<string> matches;
    int m = p.size();

    for (int index : initial_s_occurrences) {
        matches.push_back(s.substr(index, m));
    }

    return matches;
}

template <typename K, typename V>
void printUnorderedMap(const unordered_map<K, V>& umap) {
    cout<< "{";
    for (const auto& pair : umap) {
        cout << pair.first << " : " << pair.second << ", ";
    }
    cout << "}\n";
}

template <typename T>
void printVector(const vector<T>& vec) {
    cout << "[ ";
    for (const auto& element : vec) {
        cout << element << ", ";
    }
    cout << "]" << endl;
}

template <typename T>
void printVectorOfPairs(const vector<T>& vec) {
    cout << "[ ";
    for (const auto& element : vec) {
        cout << "(" << element.first << ", " << element.second << "), ";
    }
    cout << "]" << endl;
}

template <typename K, typename V>
void printUnordnedMapOfPairsVector(const unordered_map<K, V>& umap) {
    cout << "[ ";
    for (const auto& element : umap) {
        cout << "(" << element.first << ", ";
        printVector(element.second);
        cout << "), ";
    }
    cout << "]" << endl;
}

// Main function to test BWT Search
void bwt_search(const string& s_org, const string& p) {
    string s = s_org + "$";
    auto [minimal_alphabet_encode, alphabet] = construct_minimal_alphabet(s);
    cout << "Minimal alphabet encoding: " << endl;
    printUnorderedMap(minimal_alphabet_encode);
    cout << "Minimal alphabet: " << endl;
    printVector(alphabet);
    string s_enc = minimal_string_encoding(s, minimal_alphabet_encode);
    string p_enc = minimal_string_encoding(p, minimal_alphabet_encode);

    cout << "Encoded string: " << s_enc<< endl;
    cout << "Encoded pattern: " << p_enc<< endl;

    if (p_enc.empty()) {
        cout << "Pattern '" << p << "' not found in '" << s_org << "'" << endl;
        return;
    }

    string bwt = burrows_wheeler_transform(s_enc);
    cout << "BWT: " << bwt << endl;
    auto [letters_count, indexed_bwt] = letter_occurrences_and_indexed_bwt(bwt);
    cout << "Letter count: " << endl;
    printUnorderedMap(letters_count);
    cout << "Indexed BWT: " << endl;
    printVectorOfPairs(indexed_bwt);
    auto prefix_sums = compute_prefix_sums(letters_count, alphabet);
    cout << "Prefix sums: " << endl;
    printUnorderedMap(prefix_sums);
    auto rank = calculate_rank(bwt, alphabet);
    cout << "Rank: " << endl;
    printUnordnedMapOfPairsVector(rank);


    auto [start, end] = backwards_search(p_enc, indexed_bwt, prefix_sums, alphabet.back(), rank);
    if (start == -1 || end == -1) {
        cout << "Pattern '" << p << "' not found in '" << s_org << "'" << endl;
        return;
    }
    auto indexed_s = reverse_indexed_bwt(indexed_bwt, prefix_sums);
    cout << "Indexed S: " << endl;
    printVectorOfPairs(indexed_s);
    auto start_l_occ = start_letter_occurrences(start, end, p_enc.front(), prefix_sums);
    cout << "Start_l_occ: " << endl;
    printVectorOfPairs(start_l_occ);
    auto final_occ = initial_string_occurrences(indexed_s, start_l_occ);
    cout << "Final_occ: " << endl;
    printVector(final_occ);
    auto matches = check_occurrences(final_occ, s, p);
    cout << "Matches: " << endl;
    printVector(matches);
    // cout << "Pattern found between indices " << start << " and " << end << endl;
}

// Example usage
int main() {
    string s = "banana";
    string p = "an";
    bwt_search(s, p);
    return 0;
}
