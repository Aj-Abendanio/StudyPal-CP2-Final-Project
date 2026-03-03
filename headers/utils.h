// Header File [utils.h]

#ifndef UTILS_H
#define UTILS_H

#include <string>

void clearScreen();
void pauseScreen();
int readIntInRange(const std::string& prompt, int minVal, int maxVal);
char readChoiceChar(const std::string& prompt);
std::string sanitizeName(const std::string& s); // for folder-friendly names
bool fileExists(const std::string& path);

#endif