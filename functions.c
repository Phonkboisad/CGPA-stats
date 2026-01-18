#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
    #define CLEAR "clear"
#endif

// --- UI Helpers ---
void printAnimatedHeader() {
    FILE *fptr = fopen("header.txt", "r");
    char line[256];
    int lineCount = 0;

    if (fptr == NULL) {
        printf(RED "\n[Error] header.txt not found!\n" RESET);
        return;
    }

    // Read line by line to apply a different color to each row
    while (fgets(line, sizeof(line), fptr)) {
        // Apply color gradient based on line number
        if (lineCount < 4) printf(CYAN);
        else if (lineCount < 8) printf(BLUE);
        else printf(GREEN);

        for (int i = 0; line[i] != '\0'; i++) {
            printf("%c", line[i]);
            fflush(stdout);
            SLEEP(2); 
        }
        lineCount++;
    }
    printf(RESET); 
    fclose(fptr);
}

//------------------------------------------------------------------------------------------------------
void drawBox(char *text) {
    int len = strlen(text);
    printf(YELLOW "\n+");
    for(int i = 0; i < len + 2; i++) printf("-");
    printf("+\n| %s |\n+", text);
    for(int i = 0; i < len + 2; i++) printf("-");
    printf("+\n" RESET);
}

void loadingScreen() {
    system(CLEAR);
    printAnimatedHeader(); 
    
    printf(YELLOW "\n  [SYSTEM] Initializing Modules...\n  " RESET);
    printf(BLUE "[");
    for(int i = 0; i < 40; i++) {
        printf("="); 
        fflush(stdout);
        SLEEP(30); 
    }
    printf("]" GREEN " 100%% COMPLETE\n" RESET);
    SLEEP(800);
}

int getSafeInt() {
    int val;
    while (scanf("%d", &val) != 1) {
        while (getchar() != '\n'); 
        printf(RED "Invalid input! Enter a number: " RESET);
    }
    return val;
}

// --- Logic Helpers ---
float validateMark(char *subjectName) {
    float m;
    while(1) {
        printf("Enter marks for %s (0-100): ", subjectName);
        if (scanf("%f", &m) != 1 || m < 0 || m > 100) {
            while(getchar() != '\n'); 
            printf(RED "Error: Marks must be between 0 and 100.\n" RESET);
        } else return m;
    }
}

float calculateCGPA(float marks[]) {
    float total = 0;
    for(int i = 0; i < 5; i++) total += marks[i];
    float percentage = total / 5.0;
    return (percentage / 100.0) * 4.0; // Converts 100% scale to 4.0 scale
}

char calculateGrade(float cgpa) {
    if (cgpa >= 3.75) return 'A';
    if (cgpa >= 3.00) return 'B';
    if (cgpa >= 2.25) return 'C';
    if (cgpa >= 2.00) return 'D';
    return 'F';
}
void showGradeGraph() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) {
        printf(RED "No records found!\n" RESET);
        SLEEP(1500);
        return;
    }

    int counts[5] = {0, 0, 0, 0, 0}; // Indices: 0:A, 1:B, 2:C, 3:D, 4:F
    Student s;
    char line[250];

    // 1. Count the grades
    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        
        if (s.grade == 'A') counts[0]++;
        else if (s.grade == 'B') counts[1]++;
        else if (s.grade == 'C') counts[2]++;
        else if (s.grade == 'D') counts[3]++;
        else if (s.grade == 'F') counts[4]++;
    }
    fclose(fptr);

    // 2. Display the graph
    system(CLEAR);
    drawBox("GRADE DISTRIBUTION GRAPH");
    char *labels[] = {"Grade A", "Grade B", "Grade C", "Grade D", "Grade F"};
    
    printf("\n");
    for (int i = 0; i < 5; i++) {
        printf(BLUE "%-8s | " RESET, labels[i]);
        
        // Print bars based on count
        for (int j = 0; j < counts[i]; j++) {
            printf(GREEN "#" RESET); // Solid block character
        }
        
        printf(" (%d)\n", counts[i]);
    }
    
    printf("\n" YELLOW "Visual representation of student performance." RESET);
    printf("\n\nPress Enter to return to menu...");
    getchar(); getchar();
}

// --- Core Features ---
void addStudent() {
    FILE *fptr = fopen("students.txt", "a");
    if (!fptr) return;
    Student s;
    system(CLEAR);
    drawBox("ADD NEW STUDENT");
    
    printf("Enter Matric ID: "); scanf("%s", s.matricID);
    printf("Enter Name: "); scanf(" %[^\n]s", s.name); 
    
    char *subs[] = {"CSE", "CP", "EEE", "Math", "Physics"};
    for(int i = 0; i < 5; i++) s.subjects[i] = validateMark(subs[i]);
    
    s.cgpa = calculateCGPA(s.subjects);
    s.grade = calculateGrade(s.cgpa);

    // Using '|' as a delimiter to separate fields safely
    fprintf(fptr, "%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%c\n", 
            s.matricID, s.name, s.subjects[0], s.subjects[1], 
            s.subjects[2], s.subjects[3], s.subjects[4], s.cgpa, s.grade);
            
    fclose(fptr);
    printf(GREEN "\n✔ Record saved successfully!" RESET);
    SLEEP(1500);
}

void displayAll() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) { printf(RED "No records found!\n" RESET); SLEEP(1500); return; }
    
    Student s;
    char line[250];
    system(CLEAR);
    drawBox("ALL STUDENT RECORDS");
    printf(BLUE "%-12s %-20s %-8s %-5s\n" RESET, "MatricID", "Name", "CGPA", "Grade");
    printf("------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        printf("%-12s %-20s %-8.2f %-5c\n", s.matricID, s.name, s.cgpa, s.grade);
    }
    fclose(fptr);
    printf("\nPress Enter to return...");
    getchar(); getchar();
}

void displaySorted() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) return;

    Student list[100];
    int count = 0;
    char line[250];

    while (fgets(line, sizeof(line), fptr) && count < 100) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               list[count].matricID, list[count].name, &list[count].subjects[0], 
               &list[count].subjects[1], &list[count].subjects[2], 
               &list[count].subjects[3], &list[count].subjects[4], 
               &list[count].cgpa, &list[count].grade);
        count++;
    }
    fclose(fptr);

    // Bubble Sort: High to Low
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].cgpa < list[j+1].cgpa) {
                Student temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }

    system(CLEAR);
    drawBox("RANKED LIST (TOP TO BOTTOM)");
    printf(BLUE "%-5s %-12s %-20s %-8s\n" RESET, "Rank", "MatricID", "Name", "CGPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-12s %-20s %-8.2f\n", i+1, list[i].matricID, list[i].name, list[i].cgpa);
    }
    printf("\nPress Enter..."); getchar(); getchar();
}

void showClassStats() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) return;

    Student s, topper;
    float sum = 0, max = -1;
    int count = 0;
    char line[250];

    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        sum += s.cgpa;
        if (s.cgpa > max) { max = s.cgpa; topper = s; }
        count++;
    }
    fclose(fptr);

    system(CLEAR);
    drawBox("CLASS ANALYTICS");
    if (count > 0) {
        printf("Total Students      : %d\n", count);
        printf("Class Average CGPA  : %.2f\n", sum / count);
        printf(YELLOW "\n CLASS TOPPER \n" RESET); //invalid character'
        printf("Name: %s | ID: %s | CGPA: %.2f\n", topper.name, topper.matricID, topper.cgpa);
    }
    printf("\nPress Enter..."); getchar(); getchar();
}

void exportToCSV() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) return;
    char target[20];
    printf("\nEnter Matric ID to export: "); scanf("%s", target);

    Student s;
    char line[250];
    int found = 0;

    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        
        if (strcmp(s.matricID, target) == 0) {
            char fName[40];
            sprintf(fName, "%s_Report.csv", s.matricID);
            FILE *csv = fopen(fName, "w");
            fprintf(csv, "MatricID,Name,CSE,CP,EEE,Math,Physics,CGPA,Grade\n");
            fprintf(csv, "%s,%s,%.1f,%.1f,%.1f,%.1f,%.1f,%.2f,%c\n", 
                    s.matricID, s.name, s.subjects[0], s.subjects[1], 
                    s.subjects[2], s.subjects[3], s.subjects[4], s.cgpa, s.grade);
            fclose(csv);
            printf(GREEN "\n✔ Exported to %s" RESET, fName);
            found = 1; break;
        }
    }
    if(!found) printf(RED "Matric ID not found!" RESET);
    fclose(fptr); SLEEP(2000);
}

void searchStudent() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) return;
    char target[20];
    Student s;
    char line[250];
    int found = 0;

    system(CLEAR);
    printf("Enter Matric ID to Search: "); scanf("%s", target);

    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        if (strcmp(s.matricID, target) == 0) {
            printf(GREEN "\nRecord Found!\n" RESET);
            printf("Name: %s | CGPA: %.2f | Grade: %c\n", s.name, s.cgpa, s.grade);
            found = 1; break;
        }
    }
    if(!found) printf(RED "Record not found.\n" RESET);
    fclose(fptr);
    printf("\nPress Enter..."); getchar(); getchar();
}

void deleteStudent() {
    FILE *fptr = fopen("students.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fptr || !temp) return;
    char target[20];
    int found = 0;
    Student s;
    char line[250];

    printf("Enter Matric ID to Delete: "); scanf("%s", target);

    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%[^|]|%[^|]|%f|%f|%f|%f|%f|%f|%c", 
               s.matricID, s.name, &s.subjects[0], &s.subjects[1], 
               &s.subjects[2], &s.subjects[3], &s.subjects[4], &s.cgpa, &s.grade);
        
        if (strcmp(s.matricID, target) == 0) found = 1;
        else fprintf(temp, "%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%c\n", 
                     s.matricID, s.name, s.subjects[0], s.subjects[1], 
                     s.subjects[2], s.subjects[3], s.subjects[4], s.cgpa, s.grade);
    }
    fclose(fptr); fclose(temp);
    remove("students.txt"); rename("temp.txt", "students.txt");
    printf(found ? GREEN "Deleted successfully." RESET : RED "ID not found." RESET);
    SLEEP(1500);
}