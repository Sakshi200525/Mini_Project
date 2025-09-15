#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;
struct SubjectData {
    string name;
    int theory, cie, termwork, practical;
    SubjectData() {
        theory = cie = termwork = practical = -1;
    }
    int totalMarks() const {
        int total = 0;
        if (theory != -1) total += theory;
        if (cie != -1) total += cie;
        if (termwork != -1) total += termwork;
        if (practical != -1) total += practical;
        return total;
    }
};
struct Student {
    string name, prn, seat;
    SubjectData subjects[6];
    int totalScore() const {
        int total = 0;
        for (int i = 0; i < 6; i++)
            total += subjects[i].totalMarks();
        return total;
    }
};
class StudentSystem {
public:
    void ccolor(int clr) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, clr);
    }
    void loadingBar() {
        system("cls");
        ccolor(14);
        cout << "\n\n\n\t\tLoading... Please Wait\n\t";
        char load[] = "[          ]";
        for (int i = 1; i <= 10; i++) {
            load[i] = '=';
            cout << "\r\t" << load;
            Sleep(150);
        }
        system("cls");
    }
    void intro() {
        ccolor(15);
        cout << "______________________________________________________________________" << endl
             << "__________________ || STUDENT  RESULT ANALYSIS || ____________________" << endl
             << "______________________________________________________________________" << endl
             << "______________________________________________________________________\n";
        getch();
    }
    void login() {
        ccolor(14);
        char user[] = "1234", pass[] = "1234";
        char inputUser[10], inputPass[10];
        cout << "\n\t\tEnter User ID: ";
        cin >> inputUser;
        cout << "\n\t\tEnter Password: ";
        cin >> inputPass;
        if (strcmp(user, inputUser) != 0 || strcmp(pass, inputPass) != 0) {
            ccolor(12);
            cout << "\t\tINVALID USER ID OR PASSWORD!";
            getch();
            exit(0);
        } else {
            cout << "\n**************************** LOGIN SUCCESSFUL! ******************************\n";
        }
    }
    void searchStudentByPRN(const string& prnToFind) {
        ifstream inFile("Data.txt");
        if (!inFile) {
            cout << "Error opening file!\n";
            return;
        }
        string line, block;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.find("------------------------------") != string::npos) {
                if (block.find("PRN No: " + prnToFind) != string::npos) {
                    found = true;
                    stringstream ss(block);
                    string subline;
                    cout << "\nStudent found! Details:\n";
                    while (getline(ss, subline)) {
                        if (subline.find("Student Name:") != string::npos || 
                            subline.find("PRN No:") != string::npos) {
                            cout << subline << endl;
                        }
                    }
                    cout << block;
                    break;
                }
                block.clear();
            } else {
                block += line + "\n";
            }
        }
        if (!found) {
            cout << "Student with PRN No. " << prnToFind << " not found!" << endl;
        }
    }
    void viewAllData() {
        ifstream inFile("Data.txt");
        if (!inFile) {
            cout << "Error opening file!\n";
            return;
        }
        string line;
        cout << "\nAll student data:\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
    }
    vector<Student> loadStudentsFromFile(const string& filename) {
        ifstream file(filename.c_str());
        vector<Student> students;
        string line, block;
        while (getline(file, line)) {
            if (line.find("------------------------------") != string::npos) {
                stringstream ss(block);
                string subline;
                Student student;
                int idx = -1;
                while (getline(ss, subline)) {
                    if (subline.find("Student Name:") != string::npos)
                        student.name = subline.substr(subline.find(":") + 2);
                    else if (subline.find("PRN No:") != string::npos)
                        student.prn = subline.substr(subline.find(":") + 2);
                    else if (subline.find("Seat No:") != string::npos)
                        student.seat = subline.substr(subline.find(":") + 2);
                    else if (subline.find("Subject ") != string::npos)
                        student.subjects[++idx].name = subline.substr(subline.find(":") + 2);
                    else if (subline.find("Theory Marks") != string::npos)
                        student.subjects[idx].theory = extractMarks(subline);
                    else if (subline.find("CIE Marks") != string::npos)
                        student.subjects[idx].cie = extractMarks(subline);
                    else if (subline.find("Termwork Marks") != string::npos)
                        student.subjects[idx].termwork = extractMarks(subline);
                    else if (subline.find("Practical Marks") != string::npos)
                        student.subjects[idx].practical = extractMarks(subline);
                }
                students.push_back(student);
                block.clear();
            } else {
                block += line + "\n";
            }
        }
        return students;
    }
    int extractMarks(const string& line) {
        size_t colon = line.find(':');
        if (colon == string::npos) return 0;
        string numStr = line.substr(colon + 1);
        stringstream ss(numStr);
        int marks = 0;
        ss >> marks;
        return marks;
    }
    int extractOutOf(const string& line) {
        size_t slash = line.find('/');
        if (slash == string::npos) return 0;
        string outStr = line.substr(slash + 1);
        stringstream ss(outStr);
        int outOf = 0;
        ss >> outOf;
        return outOf;
    }
    void displayOverallTopperAndLowest(const vector<Student>& students) {
        if (students.empty()) return;
        int highest = 0, lowest = 0;
        for (int i = 1; i < students.size(); i++) {
            if (students[i].totalScore() > students[highest].totalScore())
                highest = i;
            if (students[i].totalScore() < students[lowest].totalScore())
                lowest = i;
        }
        cout << "\n--- Overall Topper ---\n";
        cout << "Name: " << students[highest].name << " | PRN: " << students[highest].prn
             << " | Total Marks: " << students[highest].totalScore() << endl;

        cout << "\n--- Overall Lowest Scorer ---\n";
        cout << "Name: " << students[lowest].name << " | PRN: " << students[lowest].prn
             << " | Total Marks: " << students[lowest].totalScore() << endl;
    }
    void displaySubjectWiseTopperLowest(const vector<Student>& students) {
        if (students.empty()) return;
        for (int sub = 0; sub < 6; sub++) {
            int top = 0, low = 0;
            for (int i = 1; i < students.size(); i++) {
                if (students[i].subjects[sub].totalMarks() > students[top].subjects[sub].totalMarks())
                    top = i;
                if (students[i].subjects[sub].totalMarks() < students[low].subjects[sub].totalMarks())
                    low = i;
            }
            cout << "\nSubject: " << students[top].subjects[sub].name << endl;
            cout << "  Topper:  " << students[top].name << " (PRN: " << students[top].prn
                 << ") -> Marks: " << students[top].subjects[sub].totalMarks() << endl;
            cout << "  Lowest:  " << students[low].name << " (PRN: " << students[low].prn
                 << ") -> Marks: " << students[low].subjects[sub].totalMarks() << endl;
        }
    }
    void generateReportCard() {
        ifstream inFile("Data.txt");
        if (!inFile) {
            cerr << "Error opening Data.txt file!" << endl;
            return;
        }
        string searchPRN;
        cout << "Enter PRN number to display report card: ";
        getline(cin, searchPRN);
        string line, block;
        bool studentFound = false;
        while (getline(inFile, line)) {
            if (line.find("------------------------------") != string::npos) {
                if (block.find("PRN No: " + searchPRN) != string::npos) {
                    studentFound = true;
                    stringstream ss(block);
                    string subline;
                    string currentSubject = "";
                    int totalMarks = 0;
                    int totalOutOf = 0;
                    cout << "\n-------------------- STUDENT REPORT CARD --------------------\n\n";
                    while (getline(ss, subline)) {
                        if (subline.find("Student Name:") != string::npos ||
                            subline.find("PRN No:") != string::npos ||
                            subline.find("Seat No:") != string::npos) {
                            cout << subline << endl;
                        }
                        if (subline.find("Subject ") != string::npos) {
                            currentSubject = subline.substr(subline.find(":") + 2);
                        }
                        if (subline.find("Theory Marks") != string::npos ||
                            subline.find("CIE Marks") != string::npos ||
                            subline.find("Termwork Marks") != string::npos ||
                            subline.find("Practical Marks") != string::npos) {
                            if ((currentSubject == "Web Technologies" || currentSubject == "Project - 1") &&
                                (subline.find("Theory") != string::npos || subline.find("CIE") != string::npos)) {
                                continue;
                            }
                            int marks = extractMarks(subline);
                            int outOf = extractOutOf(subline);
                            string component;
                            if (subline.find("Theory Marks") != string::npos) component = "Theory";
                            else if (subline.find("CIE Marks") != string::npos) component = "CIE";
                            else if (subline.find("Termwork Marks") != string::npos) component = "Termwork";
                            else if (subline.find("Practical Marks") != string::npos) component = "Practical";

                            if (totalMarks == 0 && totalOutOf == 0) {
                                cout << "\n";
                                cout << left << setw(40) << "Subject"
                                     << setw(20) << "Component"
                                     << setw(15) << "Marks"
                                     << setw(10) << "Out Of" << endl;
                                cout << string(85, '-') << endl;
                            }
                            cout << left << setw(40) << currentSubject
                                 << setw(20) << component
                                 << setw(15) << marks
                                 << setw(10) << outOf << endl;
                            totalMarks += marks;
                            totalOutOf += outOf;
                            currentSubject = "";
                        }
                    }
                    cout << "\n-------------------------------------------------------------\n";
                    cout << "Total Marks Obtained: " << totalMarks << " / " << totalOutOf << endl;

                    double percentage = totalOutOf > 0 ? (double)totalMarks / totalOutOf * 100.0 : 0.0;
                    cout << "Overall Percentage: " << fixed << setprecision(2) << percentage << "%" << endl;
                    cout << "-------------------------------------------------------------\n";
                    break;
                }
                block.clear();
            } else {
                block += line + "\n";
            }
        }
        if (!studentFound) {
            cout << "No student found with PRN number: " << searchPRN << endl;
        }
        inFile.close();
    }
};
int main() {
    StudentSystem sys;
    sys.loadingBar();
    sys.intro();
    sys.login();
    while (true) {
        int mainChoice;
        cout << "\n========= STUDENT RESULT SYSTEM =========\n";
        cout << "1. Search/View Student Data\n";
        cout << "2. Display Overall Topper & Lowest\n";
        cout << "3. Display Subject-wise Topper & Lowest\n";
        cout << "4. Generate Report Card by PRN\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;
        cin.ignore();
        if (mainChoice == 1) {
            int choice;
            cout << "\n1. Search by PRN\n2. View All Students\nEnter your choice: ";
            cin >> choice;
            cin.ignore();
            if (choice == 1) {
                string prn;
                cout << "Enter PRN: ";
                getline(cin, prn);
                sys.searchStudentByPRN(prn);
            } else if (choice == 2) {
                sys.viewAllData();
            } else {
                cout << "Invalid choice!\n";
            }
        } else if (mainChoice == 2) {
            vector<Student> students = sys.loadStudentsFromFile("Data.txt");
            sys.displayOverallTopperAndLowest(students);
        } else if (mainChoice == 3) {
            vector<Student> students = sys.loadStudentsFromFile("Data.txt");
            sys.displaySubjectWiseTopperLowest(students);
        } else if (mainChoice == 4) {
            sys.generateReportCard();
        } else if (mainChoice == 5) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}

