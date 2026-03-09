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

void inputUserInfo(User &u);

#endif