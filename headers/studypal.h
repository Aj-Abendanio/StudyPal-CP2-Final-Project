// Header File [studypal.h]
#ifndef STUDYPAL_H
#define STUDYPAL_H

#include <string>
#include <vector>
#include "user.h"

struct ModuleInfo {
    int number;
    std::string title;
};

struct CourseInfo {
    std::string name;
    std::vector<ModuleInfo> modules; // 5 modules every course
};

// Returns courses for a specific year/course
std::vector<CourseInfo> getCourses(int year, int sem);

// UI/flows
void runMainMenu(const User& u);

// actions
void flowStartStudying(const User& u);
void flowTakeQuiz(const User& u);
void flowViewHistory();

#endif