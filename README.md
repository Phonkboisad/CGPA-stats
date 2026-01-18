# 🎓 Student CGPA Management System (Pro-System)

A robust, console-based management system developed in **Pure C**. This project was designed for a First Year University Group Project to demonstrate a deep understanding of structural programming, file-based persistence, and interactive CLI design.



## 🌟 Overview
The **Student CGPA Pro-System** provides an efficient way to manage academic records. It transitions from simple data entry to advanced analytics, including automated CGPA calculation, ranking, and visual performance distribution.

## 🚀 Key Features
* **Persistent Data Storage**: Uses file I/O to store student records in `students.txt`, ensuring data is kept even after the program closes.
* **Automated Academic Logic**: 
    * Calculates CGPA on a 4.0 scale based on 5 core subjects.
    * Automatic letter grade assignment (A, B, C, D, F) based on academic performance.
* **Data Visualization**: Includes a unique **Grade Distribution Graph** that renders a bar chart in the console using ASCII characters.
* **Ranked Analytics**: Implements a sorting algorithm (Bubble Sort) to display a ranked list of students from high to low CGPA.
* **Professional UX**: 
    * **Animated UI**: Custom loading screens and headers for a modern feel.
    * **Input Validation**: Safe integer and mark validation to prevent runtime errors.
    * **Cross-Platform**: Compatible with both Windows and Linux/Unix systems.

## 📊 System Modules
| Module | Description |
| :--- | :--- |
| **Add Student** | Captures Matric ID, Name, and marks for 5 subjects. |
| **Display All** | Tabular view of all registered student records. |
| **Ranked List** | Displays students sorted by their academic performance. |
| **Class Stats** | Identifies the class topper and calculates class averages. |
| **Export CSV** | Generates a `.csv` datasheet for individual students. |
| **Delete/Search** | Full CRUD operations for managing specific records. |

## 🛠️ Technical Stack
* **Language**: C (C11/C17 standard)
* **Library Dependencies**: `stdio.h`, `stdlib.h`, `string.h`, `windows.h` (Windows), `unistd.h` (Linux).
* **Database**: Flat-file system (`students.txt`) using custom delimiters (`|`).

## ⚙️ Installation & Usage

### 1. Prerequisites
Ensure you have a C compiler installed (GCC, Clang, or MSVC).

### 2. Compilation and Execution
Compile and run  the program using GCC:
```bash
gcc student_system.c -o student_system
./student_system
```
Note: **Ensure header.txt is in the same directory as the executable to see the branded ASCII logo**

## 🛠️ Building with CMake

This project uses **CMake** for cross-platform build automation. This is the recommended method for compiling the three-file structure (`main.c`, `functions.c`, and `student.h`).

### 1. Prerequisites
* **CMake** (v3.10 or higher)
* **GCC** or **Clang** compiler

### 2. Build Instructions
From the project root directory, run the following commands:

```bash
# 1. Create a build directory to keep the source clean
mkdir build
cd build

# 2. Generate the build files
cmake ..

# 3. Build the executable
cmake --build .
```
###Running the App
After a successful build, the executable will be located in the build folder:
```bash
.\student.exe
```
Linux/macOS:
```bash
./student_app
```



👥 Contributors

[https://github.com/Phonkboisad] - Lead Developer / Architecture

[https://github.com/alamin2507, https://github.com/elmantouhid09] - UI Design / Testing

[https://github.com/itsIrfaaan] - Documentation
