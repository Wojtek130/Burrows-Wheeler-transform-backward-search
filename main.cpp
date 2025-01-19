#include "bwt.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>

using namespace std; 

int main() {
    string p1 = "patroklos";
    string p2 = "romeo";
    string p3 = "gamemnon";
    string p4 = "thatbroughtontheachaianswoesinnumerableandhurleddownintohadesmany";
    string p5 = "on";
    string p6 = "pt";
    string iliad = load_file("data/iliad_ready.txt");
    string romeo_and_juliet = load_file("data/romeo_and_juliet_ready.txt");
    bool c1 = prove_correctess(iliad, p1);
    cout << "Pattern: " << p1 << ", correctness: " << boolalpha << c1  << endl;
    bool c2 = prove_correctess(romeo_and_juliet, p2);
    cout << "Pattern: " << p2 << ", correctness: " << boolalpha << c2  << endl;
    // log_execution_time(iliad, p6, 10000, 50000);
    string bwt_iliad = burrows_wheeler_transform(iliad);
    string bwt_romeo_and_juliet = burrows_wheeler_transform(romeo_and_juliet);
    string bwt_iliad_compressed = naive_compression(bwt_iliad);
    string bwt_romeo_and_juliet_compressed = naive_compression(bwt_romeo_and_juliet);
    float ratio_romeo_and_juliet = (static_cast<float>(bwt_romeo_and_juliet_compressed.size()) / romeo_and_juliet.size()) * 100;
    float ratio_iliad = (static_cast<float>(bwt_iliad_compressed.size()) / iliad.size()) * 100;
    cout << "Romeo and Juliet compressed BWT: " << ratio_romeo_and_juliet <<"\% size of the original" << endl;
    cout << "Iliad compressed BWT: " << ratio_iliad <<"\% size of the original" << endl;
    return 0;
}