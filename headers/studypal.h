// Header File [studypal.h]
#ifndef STUDYPAL_H
#define STUDYPAL_H
#include <string>
#include <vector>
#include "user.h"
#include "module.h"
#include "quiz.h"
using namespace std;

typedef struct {
    int number;
    string title;
} ModuleInfo;

typedef struct {
    string name;
    vector<ModuleInfo> modules; // 5 modules every course
} CourseInfo;

// linked list node for quiz history
typedef struct HistoryNode {
    string course;
    int module;
    int score;
    int total;
    string status;
    struct HistoryNode* next;
} HistoryNode;

// returns courses for a specific year/course
vector<CourseInfo> getCourses(int year, int sem);

// ui/flows
void runMainMenu(const User& u);

// actions
void flowStartStudying(const User& u);
void flowTakeQuiz(const User& u);
void flowViewHistory();
void deleteQuizHistoryRecord();

// linked list helpers for quiz history
HistoryNode* loadQuizHistory();
void displayQuizHistory(HistoryNode* head);
void freeQuizHistory(HistoryNode* head);

#endif