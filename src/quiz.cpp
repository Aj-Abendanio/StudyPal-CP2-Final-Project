#include "../headers/quiz.h"
#include "../headers/utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

//counts the valid quiz lines first
static int countQuizLines(const string& path) {
    ifstream file(path);

    // if file cant be open it returns 0 agad
    if (!file) return 0;

    int count = 0;
    string line;

    // counts only the lines thats not empty
    while (getline(file, line)) {
        if (!line.empty()) count++;
    }
    file.close();
    return count;
}

//split one line using "|" then stores in QuizQ struct
static bool parseQuizLine(const string& line, QuizQ& out) {
    string parts[6];
    int idx = 0;
    string cur;

    // manually splits each line when it reads |
    for (char c : line) {
        if (c == '|') {
            if (idx < 6) parts[idx++] = cur;
            cur.clear();
        } else {
            cur += c;
        }
    }

    // pushes last part after loop ends
    if (idx < 6) parts[idx++] = cur;

    // valid quiz lines need to be exactly 6
    if (idx != 6) return false;

    // stores the split values into quiz struct
    out.q = parts[0];
    out.opt[0] = parts[1];
    out.opt[1] = parts[2];
    out.opt[2] = parts[3];
    out.opt[3] = parts[4];
    out.ans = toupper(static_cast<unsigned char>(parts[5][0]));

    return true;
}

// loads quiz file into dynamic aray
QuizQ* loadQuizFile(const string& path, int& count) {

    // counts how many valid lines exists first
    count = countQuizLines(path);
    if (count == 0) return nullptr;

    ifstream file(path);
    if (!file) return nullptr;

    // creates the dynamic array based on total number of quiz lines
    QuizQ* arr = new QuizQ[count];
    string line;
    int i = 0;

    // reads every line and then extracts info into the array
    while (getline(file, line) && i < count) {
        if (line.empty()) continue;

        QuizQ q;
        if (parseQuizLine(line, q)) {
            arr[i] = q;
            i++;
        }
    }

    file.close();
    count = i;

    // if no valid quiz questions deletes the array
    if (count == 0) {
        delete[] arr;
        return nullptr;
    }
    return arr;
}

// function for freeing dynamic quiz array
void freeQuizArray(QuizQ* arr) {
    delete[] arr;
}

// runs quiz up to 10 questions
int runQuiz10(const string& courseName, int moduleNo, const string& quizFile) {
    int count = 0;

    // loads quiz questions from txt
    QuizQ* bank = loadQuizFile(quizFile, count);

    // if empty or missing error messafge
    if (bank == nullptr || count == 0) {
        clearScreen();
        printIntro();
        cout << "                      [ QUIZ ERROR ]\n";
        cout << "+==============================================================+\n";
        cout << "|   Quiz file not found or empty.                             |\n";
        cout << "+==============================================================+\n";
        cout << "Expected file path:\n" << quizFile << "\n";
        pauseScreen();
        return -1;
    }


    // dynamic index array for random order of questions
    int* idx = new int[count];
    for (int i = 0; i < count; i++) idx[i] = i;

    // shuffles question indexes
    srand((unsigned)time(0));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = idx[i];
        idx[i] = idx[j];
        idx[j] = temp;
    }

    int total = (count < 10) ? count : 10;
    int score = 0;

    // asks question one by one
    for (int i = 0; i < total; i++) {
        QuizQ q = bank[idx[i]];
        string errorMessage = "";

         // input validation loop so invalid input prints same quiz screen
        while (true) {
            clearScreen();
            printIntro();

            cout << "                         [ QUIZ ]\n";
            cout << "+==============================================================+\n";
            cout << " Course   : " << courseName << "\n";
            cout << " Module   : " << moduleNo << "\n";
            cout << " Question : " << (i + 1) << " of " << total << "\n";
            cout << "+==============================================================+\n\n";

            cout << q.q << "\n\n";
            cout << "  A. " << q.opt[0] << "\n";
            cout << "  B. " << q.opt[1] << "\n";
            cout << "  C. " << q.opt[2] << "\n";
            cout << "  D. " << q.opt[3] << "\n\n";

            if (!errorMessage.empty()) {
                cout << errorMessage << "\n\n";
            }

            cout << " Your answer (A-D): ";
            string input;
            getline(cin, input);

            // accepts abcd only
            if (input.length() == 1) {
                char ans = toupper(static_cast<unsigned char>(input[0]));

                if (string("ABCD").find(ans) != string::npos) {
                    if (ans == q.ans) score++;
                    break;
                }
            }

            errorMessage = "Invalid input. Please enter A, B, C, or D ONLY.";
            pauseScreen();
        }
    }

    // computes if pass or failed
    int percent = (score * 100) / total;
    string status = (percent >= 60) ? "PASSED" : "FAILED";

    // saves quiz attempt to quiz history txt
    ofstream hist("data/quiz_history.txt", ios::app);
    if (hist) {
        hist << courseName << "|"
             << moduleNo << "|"
             << score << "|"
             << total << "|"
             << status << "\n";
        hist.close();
    }

    clearScreen();
    printIntro();
    cout << "                     [ QUIZ RESULTS ]\n";
    cout << "+==============================================================+\n";
    cout << " Course : " << courseName << "\n";
    cout << " Module : " << moduleNo << "\n";
    cout << " Score  : " << score << " / " << total << "\n";
    cout << " Grade  : " << percent << "%\n";
    cout << " Status : " << status << "\n";
    cout << "+==============================================================+\n";

    // frees dynamic memory
    delete[] idx;
    freeQuizArray(bank);

    pauseScreen();
    return score;
}