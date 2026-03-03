//HEADERS:
#include "../headers/studypal.h"
#include "../headers/utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// DATA (placeholder for now)
static vector<ModuleInfo> default5Modules() {
    return {
        {1, "Module 1"},
        {2, "Module 2"},
        {3, "Module 3"},
        {4, "Module 4"},
        {5, "Module 5"},
    };
}

vector<CourseInfo> getCourses(int year, int sem) {
    // placeholder + example
    if (year == 1 && sem == 1) {
        return {
            {"Computer Programming 1", default5Modules()},
            {"Introduction to Computing", default5Modules()}
        };
    }
    if (year == 1 && sem == 2) {
        return {
            {"Computer Programming 2", default5Modules()},
            {"Introduction to Human Computer Interaction", default5Modules()}
        };
    }
    if (year == 1 && sem == 3) {
        return {
            {"Data Structure & Algorithm", default5Modules()},
            {"Professional Development", default5Modules()}
        };
    }

    // (Put Sophomore, Junior, Senior here)
    // For now empty muna
    return {};
}

// FILE PATH RENAMER (file path validation, just in case some modules have wrong file path name) 
string reviewerPath(int year, int sem, const string& courseName, int moduleNo) {
    //  file path format ex. Reviewers/Y1S2/Computer_Programming_2/Module1.txt
    string ys = "Y" + to_string(year) + "S" + to_string(sem);
    return "Reviewers/" + ys + "/" + sanitizeName(courseName) + "/Module" + to_string(moduleNo) + ".txt";
}
string quizPath(int year, int sem, const string& courseName, int moduleNo) {
    string ys = "Y" + to_string(year) + "S" + to_string(sem);
    return "Quizzes/" + ys + "/" + sanitizeName(courseName) + "/Module" + to_string(moduleNo) + ".txt";
}

// ASCII UI 
void printMainMenuFrame() {
    cout << "+==============================================================+" << endl;
    cout << "|                       S T U D Y P A L                       |" << endl;
    cout << "+==============================================================+" << endl;
    cout << endl;
    cout << "                         [ MAIN MENU ]" << endl;
    cout << "  +----------------------------------------------------------+" << endl;
    cout << "  |                                                          |" << endl;
    cout << "  |   [1] Start Studying                                     |" << endl;
    cout << "  |   [2] Take a Quiz                                        |" << endl;
    cout << "  |   [3] View Quiz History                                  |" << endl;
    cout << "  |   [4] Exit                                               |" << endl;
    cout << "  |                                                          |" << endl;
    cout << "  +----------------------------------------------------------+" << endl;
}

int selectCourse(const User& u, const vector<CourseInfo>& courses) {
    clearScreen();
    cout << "                  [ SELECT YOUR COURSE ]" << endl;
    cout << "  +----------------------------------------------------------+" << endl;

    if (courses.empty()) {
        cout << "  |   (No courses available for this year/semester yet)      |" << endl;
        cout << "  +----------------------------------------------------------+" << endl;
        pauseScreen();
        return -1;
    }

    for (size_t i = 0; i < courses.size(); i++) {
        cout << "  |   [" << (i+1) << "] " << courses[i].name << " |" << endl;
    }

    cout << "  |   [0] Back                                                |" << endl;
    cout << "  +----------------------------------------------------------+" << endl;

    int choice = readIntInRange("  Enter choice: ", 0, (int)courses.size());

    if (choice == 0) return -1;
    return choice - 1;
}

int selectModule(const CourseInfo& course) {
    clearScreen();

    cout << "                  [ SELECT A MODULE ]" << endl;
    cout << "  +----------------------------------------------------------+" << endl;
    cout << "  |   Course: " << course.name << endl;
    cout << "  |   ------------------------------------------------------ |" << endl;

    for (const auto& m : course.modules) {
        cout << "  |   [" << m.number << "] " << m.title << endl;
    }

    cout << "  |   [0] Back to Course Selection                           |" << endl;
    cout << "  +----------------------------------------------------------+" << endl;
    cout << "  Enter choice: ";

    int choice = readIntInRange("", 0, (int)course.modules.size());

    if (choice == 0) return -1;
    return choice;
}

void printReviewerFromFile(const string& path) {
    ifstream file(path);
    if (!file) { // blocks program from continuing
        cout << "\n[Reviewer not found]\n";
        cout << "Expected file path:\n" << path << "\n\n"; 
        return;
    }
    string line;
    cout << "\n========== REVIEWER ==========\n\n";
    while (getline(file, line)) cout << line << "\n";
}

// QUIZ CHALLENGE
struct QuizQ {
    string q;
    string opt[4];
    char ans; 
};

void flowStartStudying(const User& u) {
    auto courses = getCourses(u.yearLevel, u.semester);
    int cIdx = selectCourse(u, courses);
    if (cIdx < 0) return;

    int moduleNo = selectModule(courses[cIdx]);
    if (moduleNo < 0) return;

    clearScreen();
    cout << "          [ MODULE REVIEWER ]\n";
    cout << "Course: " << courses[cIdx].name << "\n";
    cout << "Module: " << moduleNo << "\n";
    cout << "------------------------------------------------------------\n";

    string path = reviewerPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
    printReviewerFromFile(path);

    cout << "\n------------------------------------------------------------\n";
    char ch = readChoiceChar("[Q] Quiz this module  [B] Back : ");
    if (ch == 'Q') {
    cout << "\nQuiz system coming soon...\n";
    pauseScreen();
}
}

void flowTakeQuiz(const User& u) {
    clearScreen();
    cout << "==================== QUIZ FEATURE ====================\n";
    cout << "This feature is still under development.\n";
    cout << "Please check back later.\n";
    cout << "======================================================\n";
    pauseScreen();
}

void flowViewHistory() {
    clearScreen();
    cout << "                 [ QUIZ HISTORY ]\n";
    cout << "------------------------------------------------------------\n";

    ifstream hist("data/quiz_history.txt");
    if (!hist) {
        cout << "No quiz history file found yet.\n";
        pauseScreen();
        return;
    }

    string line;
    int count = 0;
    while (getline(hist, line)) {
        cout << line << "\n";
        count++;
    }
    if (count == 0) cout << "(No records yet)\n";

    pauseScreen();
}

void runMainMenu(const User& u) {
    while (true) {
        clearScreen();
        printMainMenuFrame();
        int choice = readIntInRange("  Enter choice: ", 1, 4);

        if (choice == 1) flowStartStudying(u);
        else if (choice == 2) flowTakeQuiz(u);
        else if (choice == 3) flowViewHistory();
        else if (choice == 4) break;
    }

    clearScreen();
    cout << "------------------------------------------------------------\n";
    cout << "               Thank you for using StudyPal!\n";
    cout << "                    Good luck, Tamaraw!\n";
    cout << "------------------------------------------------------------\n";
}