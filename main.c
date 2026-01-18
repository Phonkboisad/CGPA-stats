#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

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