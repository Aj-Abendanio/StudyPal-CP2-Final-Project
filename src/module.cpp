#include "../headers/module.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void printReviewerFromFile(const string& path) {
    //cout << "TEST TEST TEST\n";
    ifstream file(path);
    if (!file) {
        cout << "\n[Reviewer not found]\n";
        cout << "Expected file path:\n" << path << "\n\n";
        cout << "Tip: Check if the file/folder name matches your course/module.\n";
        return;
    }

    string line;
    cout << "\n========== REVIEWER ==========\n\n";

    while (getline(file, line)) {
        cout << line << "\n";
    }

    file.close();
}