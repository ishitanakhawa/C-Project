#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;


struct Student {
    int    id;
    string name;
    string campus;
    string course;
    double fees;
};

vector<Student> students;  

string encode(string s) {
    for (char& c : s) if (c == ',') c = '|';
    return s;
}

string decode(string s) {
    for (char& c : s) if (c == '|') c = ',';
    return s;
}

void saveToFile() {
    ofstream file("students.txt");
    file << fixed << setprecision(2); 

    for (const Student& s : students) {
        file << s.id << ","
             << encode(s.name) << ","
             << encode(s.campus) << ","
             << encode(s.course) << ","
             << s.fees << "\n";
    }
}
void loadFromFile() {
    ifstream file("students.txt");
    if (!file.is_open()) return;  

    Student s;
    char comma;

    while (file >> s.id >> comma) {
        getline(file, s.name,   ',');  s.name   = decode(s.name);
        getline(file, s.campus, ',');  s.campus = decode(s.campus);
        getline(file, s.course, ',');  s.course = decode(s.course);
        file >> s.fees;
        file.ignore();  
        students.push_back(s);
    }
}

int getNextId() {
    int maxId = 0;
   for (const Student& s : students)
        if (s.id > maxId) maxId = s.id;
    return maxId + 1;
}

double enterFees() {
    double fees;
    while (true) {
        cin >> fees;
        if (!cin.fail() && fees >= 0) return fees;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a valid positive number: ";
    }
}

void printStudent(Student s) {
    cout << "  ID     : " << s.id     << "\n"
         << "  Name   : " << s.name   << "\n"
         << "  Campus : " << s.campus << "\n"
         << "  Course : " << s.course << "\n"
         << "  Fees   : " << s.fees   << "\n"
         << "  --------------------------------\n";
}

void addStudent() {
    Student s;
    s.id = getNextId();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Name   : "; getline(cin, s.name);
    cout << "  Enter Campus : "; getline(cin, s.campus);
    cout << "  Enter Course : "; getline(cin, s.course);
    cout << "  Enter Fees   : "; s.fees = enterFees();

    students.push_back(s);
    saveToFile();
    cout << "\n  Student added! ID is: " << s.id << "\n";
}

void viewAllStudents() {
    if (students.empty()) {
        cout << "\n  No students found.\n";
        return;
    }
    cout << "\n======== All Students ========\n";
    for (const Student& s : students)
        printStudent(s);
}

void viewByCampus() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Campus Name: ";
    string campus;
    getline(cin, campus);

    bool found = false;
    cout << "\n===== Students from: " << campus << " =====\n";

    for (const Student& s : students){
        if (s.campus == campus) {
            printStudent(s);
            found = true;
        }
    }

    if (!found)
        cout << "  No students found for this campus.\n";
}

void searchByName() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Name to Search: ";
    string keyword;
    getline(cin, keyword);

    bool found = false;
    cout << "\n===== Search Results =====\n";

    for (const Student& s : students) {
        if (s.name.find(keyword) != string::npos) {
            printStudent(s);
            found = true;
        }
    }

    if (!found)
        cout << "  No students found with that name.\n";
}

void updateStudent() {
    cout << "  Enter Student ID to Update: ";
    int id;
    cin >> id;

    for (Student& s : students) {
        if (s.id == id) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Enter New Name   : "; getline(cin, s.name);
            cout << "  Enter New Campus : "; getline(cin, s.campus);
            cout << "  Enter New Course : "; getline(cin, s.course);
            cout << "  Enter New Fees   : "; s.fees = enterFees();

            saveToFile();
            cout << "\n  Student updated successfully!\n";
            return;
        }
    }
    cout << "\n  Student ID " << id << " not found.\n";
}

void deleteStudent() {
    cout << "  Enter Student ID to Delete: ";
    int id;
    cin >> id;

    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].id == id) {
            cout << "  Delete '" << students[i].name << "'? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                students.erase(students.begin() + i);
                saveToFile();
                cout << "\n  Student deleted successfully!\n";
            } else {
                cout << "\n  Delete cancelled.\n";
            }
            return;
        }
    }
    cout << "\n  Student ID " << id << " not found.\n";
}

void showReport() {
    double totalFees = 0;
    map<string, int> campusCount;

    for (const Student& s : students) {
        totalFees += s.fees;
        campusCount[s.campus]++;
    }

    cout << "\n======== Central Report ========\n";
    cout << "Total Students : " << students.size() << "\n";
    cout << "Total Fees     : " << totalFees << "\n";

    cout << "\nStudents Per Campus:\n";
    for (auto& c : campusCount) {
        cout << "  " << c.first << " : " << c.second << "\n";
    }

    cout << "================================\n";
}

int main() {
    cout << fixed << setprecision(2);  
    loadFromFile();

    int choice;

    while (true) {
        cout << "\n====== Multi-Campus University ERP ======\n"
             << "  1. Add Student\n"
             << "  2. View All Students\n"
             << "  3. View Students by Campus\n"
             << "  4. Search Student by Name\n"
             << "  5. Update Student\n"
             << "  6. Delete Student\n"
             << "  7. Central Report\n"
             << "  8. Exit\n"
             << "=========================================\n"
             << "  Enter Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n  Please enter a number between 1 and 8.\n";
            continue;
        }

        switch (choice) {
            case 1: addStudent();      break;
            case 2: viewAllStudents(); break;
            case 3: viewByCampus();    break;
            case 4: searchByName();    break;
            case 5: updateStudent();   break;
            case 6: deleteStudent();   break;
            case 7: showReport();      break;
            case 8:
                cout << "\n  Goodbye!\n";
                return 0;
            default:
                cout << "\n  Invalid choice. Enter 1 to 8.\n";
        }
    }
}