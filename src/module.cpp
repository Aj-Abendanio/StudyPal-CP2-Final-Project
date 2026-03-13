#include "../headers/module.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// function that prints reviewer contents from txt files
void printReviewerFromFile(const string& path) {
    // opens txt reviewr file using given path
    ifstream file(path);

    // checks if file not found or failed to open it
    if (!file) {
        cout << "\n[Reviewer not found]\n";
        cout << "Expected file path:\n" << path << "\n\n";
        cout << "The reviewer for this module may not be available yet.\n";
        return;
    }

    string line;

    // reads each line in the file then prints it
    while (getline(file, line)) {
        cout << line << "\n";
    }

    file.close();
}