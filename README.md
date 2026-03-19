# StudyPal – Computer Programming 2 Final Project

Authors: Angelika Abendanio, Krisma Rempis, Raffaele Tababan

StudyPal is a console based C++ application developed as a final project for Computer Programming 2. The program is designed to assist FEU Tech Computer Science (AI) students in reviewing course modules and testing their understanding through structured quiz challenges.

The system allows the users to select their year level, semester, course, and module. After selecting a module, users can view reviewer materials directly in the console and optionally take a quiz consisting of randomized multiple choice questions.

The program also maintains a record of quiz attempts, allowing users to review and manage their performance history.



## Overview

StudyPal focuses on three core subjects:

- Introduction to Computing  
- Computer Programming 1  
- Human Computer Interaction  

Each subject contains five modules, with corresponding reviewer materials and quizzes stored externally in text files.



## Main Features

- Course and module selection based on year level and semester  
- Module reviewer system that loads study material from text files  
- Quiz system with randomized multiple-choice questions (A–D format)  
- Quiz scoring and result display  
- Quiz history tracking stored in a file  
- Deletion of specific quiz records  
- Consistent input validation and error handling across the program  



## Programming Concepts Demonstrated

This project applies key concepts from Computer Programming 2, including:

- Functions and modular programming  
- Structures and data organization  
- Pointers and dynamic memory allocation (`new` and `delete`)  
- Dynamic arrays  
- Linked lists (used for quiz history management)  
- File handling using `fstream`  
- String processing and input validation  
- Console based user interface design  



## Project Structure
StudyPal-CP2-Final-Project/
- src/ # Source code files (.cpp)
- headers/ # Header files (.h)
- Reviewers/ # Module reviewer text files
- Quizzes/ # Quiz question text files
- data/ # Quiz history storage
- README.md



## How to Run

1. Make sure that the entire project is stored locally in your computer.

2. Compile the program:
   
  Using g++:
  g++ src/main.cpp src/module.cpp src/quiz.cpp src/studypal.cpp src/user.cpp src/utils.cpp -I headers -o studypal.exe

Using Clang:
  clang++ src/main.cpp src/module.cpp src/quiz.cpp src/studypal.cpp src/user.cpp src/utils.cpp -I headers -o studypal

Using Visual Studio (MSVC)
- Open the project folder in Visual Studio.
- Add all .cpp files into a project.
- Make sure the headers/ folder is included in Additional Include Directories.
- Build the solution (Ctrl + Shift + B).

3. Run the program:
Windows: .\studypal or studypal
Linux/Mac: ./studypal



## Notes

- Make sure the data/, Reviewers/, and Quizzes/ folders are in the correct directory.
- If files are missing, the program will display an error message showing the expected file path.
- The program is designed for single user only  
- Quiz history is stored locally in a text file  
- File paths are generated dynamically based on user input  
- Folder and file naming must follow the expected structure for proper functionality  

---

FEU Institute of Technology  
Bachelor of Science in Computer Science with Specialization in AI
