#include "bwt.hpp"

string remove_duplicates(const string& input) {
    unordered_set<char> seen; 
    string result;
    for (char c : input) {
        if (seen.find(c) == seen.end()) { 
            result += c;                 
            seen.insert(c);              
        }
    }
    return result;
}

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

pair<unordered_map<char, char>, vector<char>> construct_minimal_alphabet(const string& s, const string& initial_alphabet) {
    auto s_no_duplicates = remove_duplicates(s);
    vector<char> unique_letters_sorted = radix_sort(string(s_no_duplicates.begin(), s_no_duplicates.end()), initial_alphabet);

    unordered_map<char, char> minimal_alphabet_encode;
    vector<char> minimal_alphabet;

    char letter_to_use = '$';
    for (size_t i = 0; i < unique_letters_sorted.size(); ++i) {
        minimal_alphabet_encode[unique_letters_sorted[i]] = letter_to_use;
        minimal_alphabet.push_back(letter_to_use);
        if (letter_to_use == '$') {
            letter_to_use = 'a';
        } else {
            letter_to_use++;
        }
    }

    return {minimal_alphabet_encode, minimal_alphabet};
}

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

string decode_string(const string& s, const unordered_map<char, char>& minimal_alphabet_decode) {
    string new_s;
    for (char c : s) {
        new_s += minimal_alphabet_decode.at(c);
    }
    return new_s;
}

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

string burrows_wheeler_transform(const string& s) {
    vector<int> suffix_array = suffix_array_construction(s);
    string bwt;
    for (int i : suffix_array) {
        bwt += (i == 0) ? '$' : s[i - 1];
    }
    return bwt;
}

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

unordered_map<char, int> compute_prefix_sums(const unordered_map<char, int>& letter_count, const vector<char>& alphabet) {
    unordered_map<char, int> prefix_sums;
    int previous_sum = 0;

    for (char c : alphabet) {
        prefix_sums[c] = previous_sum;
        previous_sum += letter_count.at(c);
    }

    return prefix_sums;
}

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
    pair<char, int> current_letter = {'$', 0}; 

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

vector<int> initial_string_occurrences(const vector<pair<char, int>>& indexed_s, const vector<pair<char, int>>& start_letter_occurrences) {
    vector<int> occurrences;

    for (size_t i = 0; i < indexed_s.size(); ++i) {
        if (find(start_letter_occurrences.begin(), start_letter_occurrences.end(), indexed_s[i]) != start_letter_occurrences.end()) {
            occurrences.push_back(i);
        }
    }

    return occurrences;
}

vector<string> check_occurrences(const vector<int>& initial_s_occurrences, const string& s, const string& p) {
    vector<string> matches;
    int m = p.size();

    for (int index : initial_s_occurrences) {
        matches.push_back(s.substr(index, m));
    }

    return matches;
}

vector<int> bwt_search(const string& s_org, const string& p) {
    string s = s_org + "$";
    auto [minimal_alphabet_encode, alphabet] = construct_minimal_alphabet(s);
    string s_enc = minimal_string_encoding(s, minimal_alphabet_encode);
    string p_enc = minimal_string_encoding(p, minimal_alphabet_encode);
    if (p_enc.empty()) {
        cout << "Pattern '" << p << " not found (1)" << endl;
        return vector<int>();
    }
    string bwt = burrows_wheeler_transform(s_enc);
    auto [letters_count, indexed_bwt] = letter_occurrences_and_indexed_bwt(bwt);
    auto prefix_sums = compute_prefix_sums(letters_count, alphabet);
    auto rank = calculate_rank(bwt, alphabet);
    auto [start, end] = backwards_search(p_enc, indexed_bwt, prefix_sums, alphabet.back(), rank);
    if (start == -1 || end == -1) {
        cout << "Pattern " << p << " not found (2)"<< endl;
        return vector<int>();
    }
    auto indexed_s = reverse_indexed_bwt(indexed_bwt, prefix_sums);
    auto start_l_occ = start_letter_occurrences(start, end, p_enc.front(), prefix_sums);
    auto final_occ = initial_string_occurrences(indexed_s, start_l_occ);
    auto matches = check_occurrences(final_occ, s, p);
    return final_occ;
}

string naive_compression(const string& input) {
    if (input.empty()) {
        return ""; 
    }
    string result;
    char currentChar = input[0];
    int count = 1;
    for (size_t i = 1; i < input.size(); ++i) {
        if (input[i] == currentChar) {
            ++count;
        } else {
            if (count > 1){
                result += currentChar + to_string(count);
            }
            else {
                result += currentChar;
            }
            currentChar = input[i];
            count = 1;
        }
    }
    result += currentChar + to_string(count);
    return result;
}


