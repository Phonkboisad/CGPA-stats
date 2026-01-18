#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Platform Specific Configuration ---
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
    #define CLEAR "clear"
#endif

// --- Color Codes ---
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

// --- Data Structures ---
typedef struct {
    char matricID[20];    
    char name[50];
    float subjects[5];    
    float cgpa;
    char grade;
} Student;

// --- Function Prototypes ---
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
void showGradeGraph();

// --- UI Helpers ---

void printAnimatedHeader() {
    FILE *fptr = fopen("header.txt", "r");
    char line[256];
    int lineCount = 0;

    if (fptr == NULL) {
        printf(RED "\n[Note] header.txt not found! Skipping ASCII header.\n" RESET);
        return;
    }

    while (fgets(line, sizeof(line), fptr)) {
        if (lineCount < 4) printf(CYAN);
        else if (lineCount < 8) printf(BLUE);
        else printf(GREEN);

        for (int i = 0; line[i] != '\0'; i++) {
            printf("%c", line[i]);
            fflush(stdout);
            SLEEP(.85); 
        }
        lineCount++;
    }
    printf(RESET); 
    fclose(fptr);
}

void printStaticHeader() {
    FILE *f = fopen("header.txt", "r");
    if(f) {
        char line[256];
        int lineCount = 0;
        while(fgets(line, sizeof(line), f)) {
            if (lineCount < 4) printf(CYAN);
            else if (lineCount < 8) printf(BLUE);
            else printf(GREEN);
            printf("%s", line);
            lineCount++;
        }
        printf(RESET);
        fclose(f);
    }
}

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
        SLEEP(20); 
    }
    printf("]" GREEN " 100%% COMPLETE\n" RESET);
    SLEEP(500);
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
    return (percentage / 100.0) * 4.0; 
}

char calculateGrade(float cgpa) {
    if (cgpa >= 3.75) return 'A';
    if (cgpa >= 3.00) return 'B';
    if (cgpa >= 2.25) return 'C';
    if (cgpa >= 2.00) return 'D';
    return 'F';
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

    fprintf(fptr, "%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%c\n", 
            s.matricID, s.name, s.subjects[0], s.subjects[1], 
            s.subjects[2], s.subjects[3], s.subjects[4], s.cgpa, s.grade);
            
    fclose(fptr);
    printf(GREEN "\n Record saved successfully!" RESET);
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

void showGradeGraph() {
    FILE *fptr = fopen("students.txt", "r");
    if (!fptr) {
        printf(RED "No records found!\n" RESET);
        SLEEP(1500);
        return;
    }

    int counts[5] = {0, 0, 0, 0, 0}; 
    Student s;
    char line[250];

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

    system(CLEAR);
    drawBox("GRADE DISTRIBUTION GRAPH");
    char *labels[] = {"Grade A", "Grade B", "Grade C", "Grade D", "Grade F"};
    
    printf("\n");
    for (int i = 0; i < 5; i++) {
        printf(BLUE "%-8s | " RESET, labels[i]);
        for (int j = 0; j < counts[i]; j++) printf(GREEN "#" RESET);
        printf(" (%d)\n", counts[i]);
    }
    
    printf("\n" YELLOW "Visual representation of student performance." RESET);
    printf("\n\nPress Enter to return to menu...");
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
        printf(YELLOW "\n--- CLASS TOPPER ---\n" RESET);
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

// --- Main Execution Block ---

int main() {
    loadingScreen(); 
    int choice;

    do {
        system(CLEAR);
        printStaticHeader();
        
        drawBox("STUDENT CGPA PRO-SYSTEM");
        printf("1. " BLUE "Add New Student\n" RESET);
        printf("2. " BLUE "Display All Records\n" RESET);
        printf("3. " BLUE "Ranked List (High to Low)\n" RESET);
        printf("4. " YELLOW "Class Topper & Stats\n" RESET);
        printf("5. " YELLOW "Grade Distribution Graph\n" RESET);
        printf("6. " YELLOW "Search by Matric ID\n" RESET);
        printf("7. " GREEN "Export CSV Datasheet\n" RESET);
        printf("8. " RED "Delete Record\n" RESET);
        printf("0. Exit\n");
        printf("\nChoice > ");
        
        choice = getSafeInt();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: displaySorted(); break;
            case 4: showClassStats(); break;
            case 5: showGradeGraph(); break;
            case 6: searchStudent(); break;
            case 7: exportToCSV(); break;
            case 8: deleteStudent(); break;
            case 0: printf("\nSystem Closed.\n"); break;
            default: printf(RED "Invalid choice!" RESET); SLEEP(1000);
        }
    } while (choice != 0);

    return 0;
}