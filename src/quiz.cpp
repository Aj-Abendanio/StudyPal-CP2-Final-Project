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
    if (!file) return 0;

    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) count++;
    }
    file.close();
    return count;
}

//split one line by using |
static bool parseQuizLine(const string& line, QuizQ& out) {
    string parts[6];
    int idx = 0;
    string cur;

    for (char c : line) {
        if (c == '|') {
            if (idx < 6) parts[idx++] = cur;
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (idx < 6) parts[idx++] = cur;
    if (idx != 6) return false;

    out.q = parts[0];
    out.opt[0] = parts[1];
    out.opt[1] = parts[2];
    out.opt[2] = parts[3];
    out.opt[3] = parts[4];
    out.ans = toupper(static_cast<unsigned char>(parts[5][0]));

    return true;
}

// dynamic aray
QuizQ* loadQuizFile(const string& path, int& count) {
    count = countQuizLines(path);
    if (count == 0) return nullptr;

    ifstream file(path);
    if (!file) return nullptr;

    QuizQ* arr = new QuizQ[count];
    string line;
    int i = 0;

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

    if (count == 0) {
        delete[] arr;
        return nullptr;
    }
    return arr;
}

void freeQuizArray(QuizQ* arr) {
    delete[] arr;
}

int runQuiz10(const User& u, const string& courseName, int moduleNo, const string& quizFile) {
    int count = 0;
    QuizQ* bank = loadQuizFile(quizFile, count);

    if (bank == nullptr || count == 0) {
        cout << "\n[Quiz file not found or empty]\n";
        cout << "Expected file path:\n" << quizFile << "\n";
        pauseScreen();
        return -1;
    }

    // randomizing the indexes
    int* idx = new int[count];
    for (int i = 0; i < count; i++) idx[i] = i;

    srand((unsigned)time(0));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = idx[i];
        idx[i] = idx[j];
        idx[j] = temp;
    }

    int total = (count < 10) ? count : 10;
    int score = 0;

    for (int i = 0; i < total; i++) {
        QuizQ q = bank[idx[i]];

        clearScreen();
        cout << "==================== QUIZ ====================\n";
        cout << "Course : " << courseName << "\n";
        cout << "Module : " << moduleNo << "\n";
        cout << "Question " << (i + 1) << " of " << total << "\n";
        cout << "==============================================\n\n";

        cout << q.q << "\n\n";
        cout << "A. " << q.opt[0] << "\n";
        cout << "B. " << q.opt[1] << "\n";
        cout << "C. " << q.opt[2] << "\n";
        cout << "D. " << q.opt[3] << "\n\n";

        char ans = readChoiceChar("Your answer (A-D): ");
        if (toupper(ans) == q.ans) {
            score++;
        }
    }

    int percent = (score * 100) / total;
    string status = (percent >= 60) ? "PASSED" : "FAILED";

    //save history
    ofstream hist("data/quiz_history.txt", ios::app);
    if (hist) {
        hist << u.name << "|"
             << u.yearLevel << "|"
             << u.semester << "|"
             << courseName << "|"
             << moduleNo << "|"
             << score << "|"
             << total << "|"
             << status << "\n";
        hist.close();
    }

    clearScreen();
    cout << "================= QUIZ RESULTS =================\n";
    cout << "Name   : " << u.name << "\n";
    cout << "Course : " << courseName << "\n";
    cout << "Module : " << moduleNo << "\n";
    cout << "Score  : " << score << " / " << total << "\n";
    cout << "Grade  : " << percent << "%\n";
    cout << "Status : " << status << "\n";
    cout << "================================================\n";

    delete[] idx;
    freeQuizArray(bank);

    pauseScreen();
    return score;
}