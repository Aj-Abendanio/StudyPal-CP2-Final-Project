// Header File [studypal.h]
#ifndef STUDYPAL_H
#define STUDYPAL_H
#include <string>
#include <vector>
#include "user.h"
#include "module.h"
#include "quiz.h"
using namespace std;

struct ModuleInfo {
    int number;
    string title;
};

struct CourseInfo {
    string name;
    vector<ModuleInfo> modules; // 5 modules every course
};

// returns courses for a specific year/course
vector<CourseInfo> getCourses(int year, int sem);

// ui/flows
void runMainMenu(const User& u);

// actions
void flowStartStudying(const User& u);
void flowTakeQuiz(const User& u);
void flowViewHistory();

#endif