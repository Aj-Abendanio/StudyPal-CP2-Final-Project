// Header File [quiz.h]
#ifndef QUIZ_H
#define QUIZ_H
#include <string>
#include "user.h"
using namespace std;

struct QuizQ {
    string q;
    string opt[4];
    char ans;
};

QuizQ* loadQuizFile(const std::string& path, int& count);
void freeQuizArray(QuizQ* arr);
int runQuiz10(const User& u, const std::string& courseName, int moduleNo, const std::string& quizFile);

#endif