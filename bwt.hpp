#ifndef BWT_HPP
#define BWT_HPP

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils.hpp" 

using namespace std;

string remove_duplicates(const string& input);
vector<char> radix_sort(const string& s, const string& alphabet);
pair<unordered_map<char, char>, vector<char>> construct_minimal_alphabet(
    const string& s, const string& initial_alphabet = "$abcdefghijklmnopqrstuvwxyz");
string minimal_string_encoding(const string& s, const unordered_map<char, char>& minimal_alphabet_encode);
string decode_string(const string& s, const unordered_map<char, char>& minimal_alphabet_decode);
vector<int> suffix_array_construction(const string& s);
string burrows_wheeler_transform(const string& s);
pair<unordered_map<char, int>, vector<pair<char, int>>> letter_occurrences_and_indexed_bwt(const string& bwt);
unordered_map<char, int> compute_prefix_sums(const unordered_map<char, int>& letter_count, const vector<char>& alphabet);
unordered_map<char, vector<int>> calculate_rank(const string& bwt, const vector<char>& alphabet);
pair<int, int> backwards_search(const string& p_enc, const vector<pair<char, int>>& indexed_bwt,
                                 const unordered_map<char, int>& prefix_sums, char last_char_in_alphabet,
                                 const unordered_map<char, vector<int>>& rank);
vector<pair<char, int>> reverse_indexed_bwt(const vector<pair<char, int>>& indexed_bwt, const unordered_map<char, int>& prefix_sums);
vector<pair<char, int>> start_letter_occurrences(int start, int end, char first_pattern_letter, const unordered_map<char, int>& prefix_sums);
vector<int> initial_string_occurrences(const vector<pair<char, int>>& indexed_s, const vector<pair<char, int>>& start_letter_occurrences);
vector<string> check_occurrences(const vector<int>& initial_s_occurrences, const string& s, const string& p);
vector<int> bwt_search(const string& s_org, const string& p);
string naive_compression(const string& input);

#endif // BWT_HPP
