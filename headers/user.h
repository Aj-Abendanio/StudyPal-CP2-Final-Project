// Header File [user.h]

#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct User {
    string name;
    int yearLevel;
    int semester;
};

// Function prototypes:
void inputUserInfo(User &u);
void displayUserInfo(const User &u);

#endif