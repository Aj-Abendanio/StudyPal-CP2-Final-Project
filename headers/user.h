// Header File [user.h]
#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

typedef struct {
    string name;
    int yearLevel;
    int semester;
} User;

void inputUserInfo(User &u);

#endif