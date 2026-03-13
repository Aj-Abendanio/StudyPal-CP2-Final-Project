#include "../headers/utils.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <fstream>
using namespace std;

// clears screen depending on the OS 
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// pauses the program until the user hit enter
void pauseScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// intro UI
void printIntro() {
    cout << "+==============================================================+\n";
    cout << "|                       S T U D Y P A L                       |\n";
    cout << "+==============================================================+\n";
    cout << "|    A C++ Console based Study and Quiz System for FEUTech    |\n";
    cout << "|                Computer Science (AI) Students               |\n";
    cout << "|                                                             |\n";
    cout << "| Program Description:                                        |\n";
    cout << "| StudyPal is an academic support tool designed to help       |\n";
    cout << "| in reviewing course modules, answer quiz challenges,        |\n";
    cout << "| and monitor your quiz performance in a simple console       |\n";
    cout << "| environment.                                                |\n";
    cout << "+==============================================================+\n";
    cout << "| Main Features:                                              |\n";
    cout << "|  - Module reviewer loader                                   |\n";
    cout << "|  - Quiz challenge system                                    |\n";
    cout << "|  - Randomized quiz questions                                |\n";
    cout << "|  - Quiz history tracking                                    |\n";
    cout << "+==============================================================+\n\n";
}

//input validation for numbers
int readIntInRange(const string& prompt, int minVal, int maxVal) {
    int x;
    while (true) {
        cout << prompt;

        // accepts valid number inside the allowed range only
        if (cin >> x && x >= minVal && x <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }

        cout << "\nInvalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        pauseScreen();
    }
}

// for characters
char readChoiceChar(const string& prompt, const string& validOptions) {
    string s;

    while (true) {
        cout << prompt;
        getline(cin, s);

        // accepts one character only
        if (s.length() == 1) {
            char c = toupper(static_cast<unsigned char>(s[0]));

            // checks if the letter exists in the valid options string
            if (validOptions.find(c) != string::npos) {
                return c;
            }
        }
        cout << "\nInvalid input. Please enter a valid option.\n";
        pauseScreen();
    }
}

// converts course names to folder format
string sanitizeName(const string& s) {
    string result = "";

    for (char c : s) {
        // keeps letters and numbers
        if (isalnum(static_cast<unsigned char>(c))) {
            result += c;
        }
        // converts spaces and some symbols into _
        else if (c == ' ' || c == '&' || c == '-' || c == '/') {
            if (!result.empty() && result.back() != '_') {
                result += '_';
            }
        }
    }

    // removes extra _
    if (!result.empty() && result.back() == '_') {
        result.pop_back();
    }
    return result;
}

// checks if file exists
bool fileExists(const string& path) {
    ifstream f(path);
    return f.good();
}