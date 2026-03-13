// Header File [utils.h]
#ifndef UTILS_H
#define UTILS_H
#include <string>
using namespace std;

void clearScreen();
void pauseScreen();
int readIntInRange(const string& prompt, int minVal, int maxVal);
char readChoiceChar(const string& prompt, const string& validOptions);
string sanitizeName(const string& s);
void printIntro();

#endif