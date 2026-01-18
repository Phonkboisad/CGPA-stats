#ifndef STUDENT_H
#define STUDENT_H

// Color codes
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"   // Added
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
    #define CLEAR "clear"
#endif

typedef struct {
    char matricID[20];    
    char name[50];
    float subjects[5];    
    float cgpa;
    char grade;
} Student;

void loadingScreen();
void printAnimatedHeader();
void printStaticHeader();
void drawBox(char *text);
int getSafeInt();
float validateMark(char *subjectName); 
char calculateGrade(float cgpa);
float calculateCGPA(float marks[]);

void addStudent();
void displayAll();
void searchStudent();
void deleteStudent();
void displaySorted();     
void exportToCSV();      
void showClassStats();    
void showGradeGraph(); // <--- NEW FEATURE
#endif