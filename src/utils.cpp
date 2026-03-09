#include "../headers/utils.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <fstream>
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readIntInRange(const std::string& prompt, int minVal, int maxVal) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= minVal && x <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

char readChoiceChar(const std::string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (!s.empty()) {
            return static_cast<char>(toupper(static_cast<unsigned char>(s[0])));
        }
        cout << "Invalid input. Try again.\n";
    }
}

// for file name validation
string sanitizeName(const string& s) {
    string result = "";

    for (char c : s) {
        if (isalnum(static_cast<unsigned char>(c))) {
            result += c;
        }
        else if (c == ' ' || c == '&' || c == '-' || c == '/') {
            if (!result.empty() && result.back() != '_') {
                result += '_';
            }
        }
    }
    if (!result.empty() && result.back() == '_') {
        result.pop_back();
    }

    return result;
}

bool fileExists(const string& path) {
    ifstream f(path);
    return f.good();
}