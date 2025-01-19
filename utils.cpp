#include "utils.hpp"

string load_file(const string& filename) {
    ifstream file(filename); // Open the file
    if (!file) {
        cerr << "Unable to open the file!" << endl;
        return "";
    }

    ostringstream buffer;
    buffer << file.rdbuf(); 
    string fileContent = buffer.str(); 
    file.close();
    return fileContent;
}

vector<int> all_matches(const string& text, const string& pattern) {
    vector<int> matches;
    try {
        regex re(pattern);
        auto begin = sregex_iterator(text.begin(), text.end(), re);
        auto end = sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            matches.push_back(it->position());
        }
    } catch (const regex_error& e) {
        cerr << "Regex error: " << e.what() << endl;
    }
    return matches;
}

bool prove_correctess(const string& text, const string& pattern) {
    auto m1 = bwt_search(text, pattern);
    auto m2 = all_matches(text, pattern);
    cout << "Number of occurences BWT: " << m1.size() << endl;
    cout << "Number of occurences Regex: " << m2.size() << endl;
    return m1 == m2;
}

void log_execution_time(const string& text, const string& pattern, int start, int step) {
    ofstream file("execution_times/execution_times_" + pattern + ".csv");
    for (int i = start; i <= text.size(); i += step) {
        string sub = text.substr(0, i);
        auto start = chrono::high_resolution_clock::now();
        bwt_search(sub, pattern);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        file << i << "," << duration.count() << "\n";
    }
    file.close();
}