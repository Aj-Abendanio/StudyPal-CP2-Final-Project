#include "../headers/user.h"
#include "../headers/utils.h"
#include <iostream>
#include <string>

using namespace std;

void inputUserInfo(User& u) {
    clearScreen();
    cout << "+==============================================================+\n";
    cout << "|                       S T U D Y P A L                       |\n";
    cout << "+==============================================================+\n\n";

    cout << "Enter your name: ";
    getline(cin, u.name);

    u.yearLevel = readIntInRange("Enter Year Level (1-4): ", 1, 4);
    u.semester  = readIntInRange("Enter Semester (1-3): ", 1, 3);

    cout << "\nWelcome, " << u.name << "!\n";
    pauseScreen();
}