// HEADERS:
#include "../headers/user.h"
#include "../headers/utils.h"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
using namespace std;

// name validator (makes sure inputs are letters only)
bool isValidName(const string& name) {
    if (name.empty()) return false; // rejects spaces

    // checks all characters in string
    for (char c : name) {
        // rejects anything thats not letters or spaces
        if (!isalpha(static_cast<unsigned char>(c)) && c != ' ')
            return false;
    }
    return true;
}

// function for user info inputs
void inputUserInfo(User& u) {
    // loop for name input
    while (true) {
        clearScreen();
        printIntro();

        cout << "Enter your name: ";
        getline(cin, u.name);

        // if valid, moves to the next field
        if (isValidName(u.name)) break;

        cout << "\nInvalid input. Please enter only LETTERS.\n";
        pauseScreen();
    }

    // loop for year level input
    while (true) {
        clearScreen();
        printIntro();

        cout << "Enter your name: " << u.name << "\n";
        cout << "Enter Year Level (1-4): ";

        // accepts numbers from 1 to 4 only
        if (cin >> u.yearLevel && u.yearLevel >= 1 && u.yearLevel <= 4) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        cout << "\nInvalid input. Please enter 1 - 4 ONLY.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pauseScreen();
    }

    // loop for semester input
    while (true) {
        clearScreen();
        printIntro();

        cout << "Enter your name: " << u.name << "\n";
        cout << "Enter Year Level (1-4): " << u.yearLevel << "\n";
        cout << "Enter Semester (1-3): ";

        if (cin >> u.semester && u.semester >= 1 && u.semester <= 3) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        cout << "\nInvalid input. Please enter 1 - 3 ONLY.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pauseScreen();
    }

    clearScreen();
    printIntro();
    cout << "Enter your name: " << u.name << "\n";
    cout << "Enter Year Level (1-4): " << u.yearLevel << "\n";
    cout << "Enter Semester (1-3): " << u.semester << "\n";

    cout << "\nWelcome, " << u.name << "!\n";
    pauseScreen();
}