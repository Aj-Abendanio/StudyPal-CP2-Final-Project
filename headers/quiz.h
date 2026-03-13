// Header File [quiz.h]
#ifndef QUIZ_H
#define QUIZ_H
#include <string>
#include "user.h"
using namespace std;

typedef struct {
    string q;
    string opt[4];
    char ans;
} QuizQ; 

QuizQ* loadQuizFile(const string& path, int& count);
void freeQuizArray(QuizQ* arr);
int runQuiz10(const string& courseName, int moduleNo, const string& quizFile);

#endif