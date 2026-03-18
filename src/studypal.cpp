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

    // returns empty list if not available
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

         // if no courses found
        if (courses.empty()) {
            cout << "  |   (No courses available for this year/semester yet)      |" << endl;
            cout << "  +----------------------------------------------------------+" << endl;
            pauseScreen();
            return -1;
        }

        // prints all courses dynamicaly
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

        // checks if input is valid number within range
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

        // prints modules (1–5)
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

        // validates module input
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

        // loads and prints reviewer from file
        string path = reviewerPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
        printReviewerFromFile(path);

        cout << "\n------------------------------------------------------------\n";

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "[Q] Quiz this module  [B] Back : ";
        string input;
        getline(cin, input);

        // validates Q or B only
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

    // let user pick a course
    int cIdx = selectCourse(u, courses);
    if (cIdx < 0) return;

    // for module
    int moduleNo = selectModule(courses[cIdx]);
    if (moduleNo < 0) return;

    // builds path to quiz file and then runs quiz
    string qFile = quizPath(u.yearLevel, u.semester, courses[cIdx].name, moduleNo);
    runQuiz10(courses[cIdx].name, moduleNo, qFile);
}

// loads quiz history from file into a linked list
HistoryNode* loadQuizHistory() {
    ifstream hist("data/quiz_history.txt");
    if (!hist) return nullptr;

    // initialize linked list pointers
    HistoryNode* head = nullptr;
    HistoryNode* tail = nullptr;

    string line;

     // reads file line by line
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

        // if complete data is read
        if (idx == 5) {
            HistoryNode* newNode = new HistoryNode; // creates new node

            // assign values from file
            newNode->course = parts[0];
            newNode->module = stoi(parts[1]);
            newNode->score = stoi(parts[2]);
            newNode->total = stoi(parts[3]);
            newNode->status = parts[4];
            newNode->next = nullptr;

            // insert node into linked list
            if (head == nullptr) {
                head = newNode; // first node basically becomes the head
                tail = newNode;
            } else {
                tail->next = newNode; // append to end
                tail = newNode;
            }
        }
    }

    hist.close();
    return head;
}

// displays the quiz history by traversing the linked list
void displayQuizHistory(HistoryNode* head) {
    if (head == nullptr) {
        cout << "|   No quiz history yet.                                      |\n";
        return;
    }

    int count = 1;
    HistoryNode* current = head; // start from head

    // traverse until nullptr
    while (current != nullptr) {
        cout << "|   Attempt #" << count << "\n";
        cout << "|   Course : " << current->course << "\n";
        cout << "|   Module : " << current->module << "\n";
        cout << "|   Score  : " << current->score << "/" << current->total << "\n";
        cout << "|   Status : " << current->status << "\n";
        cout << "|--------------------------------------------------------------\n";

        // move to next node
        current = current->next;
        count++;
    }
}

// frees the entire linked list
void freeQuizHistory(HistoryNode* head) {
    HistoryNode* current = head;

    // loop until all nodes deleted
    while (current != nullptr) {
        HistoryNode* temp = current; // stores current node
        current = current->next; // moves forwar
        delete temp; // free memory
    }
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

        // view history
        if (input == "0") return;

        if (input == "1") {
            clearScreen();
            printIntro();
            cout << "                    [ YOUR QUIZ HISTORY ]\n";
            cout << "+==============================================================+\n";
            HistoryNode* head = loadQuizHistory();
            displayQuizHistory(head);
            cout << "+==============================================================+\n";

            freeQuizHistory(head);
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
    // load history into linked list
    HistoryNode* head = loadQuizHistory();

    if (head == nullptr) {
        clearScreen();
        printIntro();
        cout << "                   [ DELETE QUIZ HISTORY ]\n";
        cout << "+==============================================================+\n";
        cout << "|   No quiz history yet.                                      |\n";
        cout << "+==============================================================+\n";
        pauseScreen();
        return;
    }

    string errorMessage = "";

    while (true) {
        clearScreen();
        printIntro();
        cout << "                   [ DELETE QUIZ HISTORY ]\n";
        cout << "+==============================================================+\n";

        int count = 1;
        HistoryNode* current = head;

        // display all records with numbering
        while (current != nullptr) {
            cout << "  [" << count << "] "
                 << current->course << " | Module " << current->module
                 << " | Score " << current->score << "/" << current->total
                 << " | " << current->status << "\n";
            current = current->next;
            count++;
        }

        cout << "+==============================================================+\n";

        if (!errorMessage.empty()) {
            cout << errorMessage << "\n\n";
        }

        cout << "Enter attempt number to delete: ";
        string input;
        getline(cin, input);

        // check if input is numeric
        bool valid = true;
        for (char c : input) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                valid = false;
                break;
            }
        }

        if (valid && !input.empty()) {
            int choice = stoi(input);

            int totalNodes = count - 1;

            // valid range check
            if (choice >= 1 && choice <= totalNodes) {
                // delete node logic
                if (choice == 1) {
                    HistoryNode* temp = head;
                    head = head->next;
                    delete temp;
                } else {
                    // find node before target
                    HistoryNode* prev = head;
                    for (int i = 1; i < choice - 1; i++) {
                        prev = prev->next;
                    }
                    // remove node
                    HistoryNode* temp = prev->next;
                    prev->next = temp->next;
                    delete temp;
                }

                // rewrite file after deletion
                ofstream out("data/quiz_history.txt");
                HistoryNode* savePtr = head;
                while (savePtr != nullptr) {
                    out << savePtr->course << "|"
                        << savePtr->module << "|"
                        << savePtr->score << "|"
                        << savePtr->total << "|"
                        << savePtr->status << "\n";
                    savePtr = savePtr->next;
                }
                out.close();

                clearScreen();
                printIntro();
                cout << "                   [ DELETE QUIZ HISTORY ]\n";
                cout << "+==============================================================+\n";
                cout << "|   Record deleted successfully.                              |\n";
                cout << "+==============================================================+\n";

                freeQuizHistory(head);
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

        // validate input (1–4 only)
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
    // bye message
    cout << "------------------------------------------------------------\n";
    cout << "               Thank you for using StudyPal!\n";
    cout << "                    Good luck, Tamaraw!\n";
    cout << "------------------------------------------------------------\n";
}