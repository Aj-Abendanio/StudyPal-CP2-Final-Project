// Main cpp
#include "../headers/user.h"
#include "../headers/studypal.h"
using namespace std;

int main() {
    // creates 1 user variable for current session of program
    User student;

    // calls function that asks user infos
    inputUserInfo(student); 

    // if valid and all done, it calls main menu function
    runMainMenu(student); 
    return 0;
}