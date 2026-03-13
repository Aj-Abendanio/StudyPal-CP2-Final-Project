//HEADERS:
#include "../headers/studypal.h"
#include "../headers/utils.h"
#include "../headers/module.h"
#include "../headers/quiz.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// default module list (reusable 5 module setup for each course)
vector<ModuleInfo> default5Modules() {
    return {
        {1, "Module 1"},
        {2, "Module 2"},
        {3, "Module 3"},
        {4, "Module 4"},
        {5, "Module 5"},
    };
}

// returns available courses based on the users year level and semester
vector<CourseInfo> getCourses(int year, int sem) {
    if (year == 1 && sem == 1) {
        return {
            {"Introduction to Computing", default5Modules()},
            {"Computer Programming 1", default5Modules()}
        };
    }

    if (year == 1 && sem == 2) {
        return {
            {"Introduction to Human Computer Interaction", default5Modules()}
        };
    }

    return {};
}

// builds reviewer file path based on (year, semester, course, module number)
string reviewerPath(int year, int sem, const string& courseName, int moduleNo) {
    // !! file path format:Reviewers/Y1S2/Computer_Programming_2/Module1.txt
    string ys = "Y" + to_string(year) + "S" + to_string(sem);
    return "Reviewers/" + ys + "/" + sanitizeName(courseName) + "/Module" + to_string(moduleNo) + ".txt";
}

// builds quiz file path based on (year, semester, course, module number)
string quizPath(int year, int sem, const string& courseName, int moduleNo) {
    string ys = "Y" + to_string(year) + "S" + to_string(sem);
    return "Quizzes/" + ys + "/" + sanitizeName(courseName) + "/Module" + to_string(moduleNo) + ".txt";
}

// main menu UI 
void printMainMenuFrame() {
    printIntro();

    cout << "                       [ MAIN MENU ]" << endl;
    cout << " +----------------------------------------------------------+" << endl;
    cout << " |                                                          |" << endl;
    cout << " |   [1] Start Studying                                     |" << endl;
    cout << " |   [2] Take a Quiz                                        |" << endl;
    cout << " |   [3] View Quiz History                                  |" << endl;
    cout << " |   [4] Exit                                               |" << endl;
    cout << " |                                                          |" << endl;
    cout << " +----------------------------------------------------------+" << endl;
}

// lets user choose a course from the available course list
int selectCourse(const User& u, const vector<CourseInfo>& courses) {
    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();

        cout << "                  [ SELECT YOUR COURSE ]" << endl;
        cout << "  +----------------------------------------------------------+" << endl;

        if (courses.empty()) {
            cout << "  |   (No courses available for this year/semester yet)      |" << endl;
            cout << "  +----------------------------------------------------------+" << endl;
            pauseScreen();
            return -1;
        }

        for (size_t i = 0; i < courses.size(); i++) {
            cout << "  |   [" << (i + 1) << "] " << courses[i].name << endl;
        }

        cout << "  |   [0] Back                                               |" << endl;
        cout << "  +----------------------------------------------------------+" << endl;

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "Enter choice: ";
        string input;
        getline(cin, input);

        if (input.length() == 1 && input[0] >= '0' && input[0] <= char('0' + courses.size())) {
            int choice = input[0] - '0';
            if (choice == 0) return -1;
            return choice - 1;
        }

        errorMessage = "Invalid input. Please input valid menu number [1-4].";
        pauseScreen();
    }
}

// lets user choose a module from the selected course
int selectModule(const CourseInfo& course) {
    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();

        cout << "                  [ SELECT A MODULE ]" << endl;
        cout << "  +----------------------------------------------------------+" << endl;
        cout << "  |   Course: " << course.name << endl;
        cout << "  |   ------------------------------------------------------ |" << endl;

        for (const auto& m : course.modules) {
            cout << "  |   [" << m.number << "] " << m.title << endl;
        }

        cout << "  |   [0] Back to Course Selection                           |" << endl;
        cout << "  +----------------------------------------------------------+" << endl;

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "Enter choice: ";
        string input;
        getline(cin, input);

        if (input.length() == 1 && input[0] >= '0' && input[0] <= char('0' + course.modules.size())) {
            int choice = input[0] - '0';
            if (choice == 0) return -1;
            return choice;
        }

        errorMessage = "Invalid input. Please enter valid module number.";
        pauseScreen();
    }
}

// reviewer flow: select course, select module, read reviewer
void flowStartStudying(const User& u) {
    auto courses = getCourses(u.yearLevel, u.semester);
    int cIdx = selectCourse(u, courses);
    if (cIdx < 0) return;

    int moduleNo = selectModule(courses[cIdx]);
    if (moduleNo < 0) return;

    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();

        cout << "                 [ MODULE REVIEWER ]\n";
        cout << "------------------------------------------------------------\n";
        cout << "Course : " << courses[cIdx].name << "\n";
        cout << "Module : " << moduleNo << "\n";
        cout << "------------------------------------------------------------\n\n";

        string path = reviewerPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
        printReviewerFromFile(path);

        cout << "\n------------------------------------------------------------\n";

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "[Q] Quiz this module  [B] Back : ";
        string input;
        getline(cin, input);

        if (input.length() == 1) {
            char ch = toupper(static_cast<unsigned char>(input[0]));

            if (ch == 'Q') {
                string qFile = quizPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
                runQuiz10(courses[cIdx].name, moduleNo, qFile);
                return;
            }
            else if (ch == 'B') {
                return;
            }
        }

        errorMessage = "Invalid input. Please enter Q or B ONLY.";
        pauseScreen();
    }
}

// quiz flow: select course, select module, start quiz
void flowTakeQuiz(const User& u) {
    auto courses = getCourses(u.yearLevel, u.semester);
    int cIdx = selectCourse(u, courses);
    if (cIdx < 0) return;

    int moduleNo = selectModule(courses[cIdx]);
    if (moduleNo < 0) return;

    string qFile = quizPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
    runQuiz10(courses[cIdx].name, moduleNo, qFile);
}

// quiz history menu (view history / delete record / back)
void flowViewHistory() {
    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();

        cout << "                    [ YOUR QUIZ HISTORY ]\n";
        cout << "+==============================================================+\n";
        cout << "|   [1] View Quiz History                                     |\n";
        cout << "|   [2] Delete a Quiz Record                                  |\n";
        cout << "|   [0] Back                                                  |\n";
        cout << "+==============================================================+\n";

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "Enter choice: ";
        string input;
        getline(cin, input);

        if (input == "0") return;

        if (input == "1") {
            clearScreen();
            printIntro();
            cout << "                    [ YOUR QUIZ HISTORY ]\n";
            cout << "+==============================================================+\n";

            ifstream hist("data/quiz_history.txt");
            if (!hist) {
                cout << "|   No quiz history file found yet.                           |\n";
                cout << "+==============================================================+\n";
                pauseScreen();
                continue;
            }

            string line;
            int count = 0;

            while (getline(hist, line)) {
                if (line.empty()) continue;

                string parts[5];
                int idx = 0;
                string cur;

                for (char c : line) {
                    if (c == '|') {
                        if (idx < 5) parts[idx++] = cur;
                        cur.clear();
                    } else {
                        cur += c;
                    }
                }
                if (idx < 5) parts[idx++] = cur;

                if (idx == 5) {
                    cout << "|   Attempt #" << (count + 1) << "\n";
                    cout << "|   Course : " << parts[0] << "\n";
                    cout << "|   Module : " << parts[1] << "\n";
                    cout << "|   Score  : " << parts[2] << "/" << parts[3] << "\n";
                    cout << "|   Status : " << parts[4] << "\n";
                    cout << "|--------------------------------------------------------------\n";
                    count++;
                }
            }

            if (count == 0) {
                cout << "|   No quiz history yet.                                      |\n";
            }

            cout << "+==============================================================+\n";
            pauseScreen();
        }
        else if (input == "2") {
            deleteQuizHistoryRecord();
        }
        else {
            errorMessage = "Invalid input. Please enter 0, 1, or 2 ONLY.";
            pauseScreen();
        }
    }
}

// deletes quiz attempt from quiz history file
void deleteQuizHistoryRecord() {
    ifstream hist("data/quiz_history.txt");
    if (!hist) {
        clearScreen();
        printIntro();
        cout << "                   [ DELETE QUIZ HISTORY ]\n";
        cout << "+==============================================================+\n";
        cout << "|   No quiz history yet.                                      |\n";
        cout << "+==============================================================+\n";
        pauseScreen();
        return;
    }

    vector<string> records;
    string line;

    while (getline(hist, line)) {
        if (!line.empty()) {
            records.push_back(line);
        }
    }
    hist.close();

    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();
        cout << "                   [ DELETE QUIZ HISTORY ]\n";
        cout << "+==============================================================+\n";

        if (records.empty()) {
            cout << "|   No history available to delete.                           |\n";
            cout << "+==============================================================+\n";
            pauseScreen();
            return;
        }

        for (size_t i = 0; i < records.size(); i++) {
            cout << "  [" << i + 1 << "] " << records[i] << "\n";
        }

        cout << "+==============================================================+\n";


        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "Enter attempt number to delete: ";
        string input;
        getline(cin, input);

        bool valid = true;
        for (char c : input) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                valid = false;
                break;
            }
        }

        if (valid && !input.empty()) {
            int choice = stoi(input);

            if (choice >= 1 && choice <= (int)records.size()) {
                records.erase(records.begin() + (choice - 1));

                ofstream out("data/quiz_history.txt");
                for (const string& rec : records) {
                    out << rec << "\n";
                }
                out.close();

                clearScreen();
                printIntro();
                cout << "                   [ DELETE QUIZ HISTORY ]\n";
                cout << "+==============================================================+\n";
                cout << "|   Record deleted successfully.                              |\n";
                cout << "+==============================================================+\n";
                pauseScreen();
                return;
            }
        }
        errorMessage = "Invalid input. Please enter a valid attempt number.";
        pauseScreen();
    }
}

// controls main menu loop for whole program
void runMainMenu(const User& u) {
    string errorMessage = "";

    while (true) {
        clearScreen();
        printMainMenuFrame();

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << " Enter choice: ";
        string input;
        getline(cin, input);

        if (input.length() == 1 && input[0] >= '1' && input[0] <= '4') {
            int choice = input[0] - '0';

            if (choice == 1) flowStartStudying(u);
            else if (choice == 2) flowTakeQuiz(u);
            else if (choice == 3) flowViewHistory();
            else if (choice == 4) break;

            errorMessage = "";
        }
        else {
            errorMessage = "Invalid input. Please enter 1 - 4 ONLY.";
            pauseScreen();
        }
    }
    clearScreen();
    printIntro();
    cout << "------------------------------------------------------------\n";
    cout << "               Thank you for using StudyPal!\n";
    cout << "                    Good luck, Tamaraw!\n";
    cout << "------------------------------------------------------------\n";
}