#ifndef UTILS_HPP
#define UTILS_HPP

#include "bwt.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std; 

template <typename K, typename V>
void print_unordered_map(const unordered_map<K, V>& umap);

template <typename T>
void print_vector(const vector<T>& vec);

template <typename T>
void print_vector_of_pairs(const vector<T>& vec);

template <typename K, typename V>
void print_unordned_map_of_pairs_vector(const unordered_map<K, V>& umap);

string load_file(const string& filename);
vector<int> all_matches(const string& text, const string& pattern);
bool prove_correctess(const string& text, const string& pattern);
void log_execution_time(const string& text, const string& pattern, int start, int step);

template <typename K, typename V>
void print_unordered_map(const unordered_map<K, V>& umap) {
    cout << "{";
    for (const auto& pair : umap) {
        cout << pair.first << " : " << pair.second << ", ";
    }
    cout << "}\n";
}

template <typename T>
void print_vector(const vector<T>& vec) {
    cout << "[ ";
    for (const auto& element : vec) {
        cout << element << ", ";
    }
    cout << "]" << endl;
}

template <typename T>
void print_vector_of_pairs(const vector<T>& vec) {
    cout << "[ ";
    for (const auto& element : vec) {
        cout << "(" << element.first << ", " << element.second << "), ";
    }
    cout << "]" << endl;
}

template <typename K, typename V>
void print_unordned_map_of_pairs_vector(const unordered_map<K, V>& umap) {
    cout << "[ ";
    for (const auto& element : umap) {
        cout << "(" << element.first << ", ";
        print_vector(element.second);
        cout << "), ";
    }
    cout << "]" << endl;
}

#endif 
