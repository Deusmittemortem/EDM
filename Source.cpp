#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
using namespace std;

// ���������
struct User {
    int id;
    string login;
    string password;
    string role;
};

struct Student {
    int id;
    int userId;
    string fullName;
    string faculty;
    string groupNumber;
};

struct Teacher {
    int id;
    int userId;
    string fullName;
    string department;
};

struct Scholarship {
    int studentId;
    double amount;
};

struct Schedule {
    string dayOfWeek;
    string subject;
    vector<string> teachers; 
    string time;
    vector<int> studentIds;
};

struct TeacherSalary {
    int teacherId;
    int hoursPerMonth;
    double hourlyRate;
};

// Simulated database
unordered_map<int, User> users = {
    {1, {1, "i.ivanov", "password", "student"}},
    {2, {2, "a.smirnova", "password", "student"}},
    {3, {3, "s.ivanov", "password", "teacher"}},
    {4, {4, "a.petrov", "password", "teacher"}},
    {5, {5, "i.sidorov", "password", "teacher"}},
    {6, {6, "admin", "admin", "admin"}}
};

unordered_map<int, Student> students = {
    {1, {1, 1, "���� ������", "���������", "������-101"}},
    {2, {2, 2, "���� ��������", "���������", "������-101"}}
};

unordered_map<int, Teacher> teachers = {
    {1, {1, 3, "���� ������", "����������"}},
    {2, {2, 4, "������� ������", "������"}},
    {3, {3, 5, "����� �������", "�����"}}
};

vector<Scholarship> scholarships = {
    {1, 1000.0},
    {2, 1500.0}
};

unordered_map<string, vector<Schedule>> schedule = {
    {"�����������", {
        {"�����������", "����������", {"���� ������", "����� �������"}, "09:00", {1, 2}},
        {"�����������", "������", {"������� ������"}, "11:00", {1}}
    }},
    {"�������", {
        {"�������", "�����", {"����� �������", "������� ������"}, "10:00", {2}},
        {"�������", "����������������", {"���� ������"}, "13:00", {1, 2}}
    }},
    {"�����", {}},
    {"�������", {}},
    {"�������", {}}
};

vector<TeacherSalary> teacherSalaries = {
    {1, 40, 25.0},  // �� ���� ������
    {2, 35, 30.0},  // �� ������� ������
    {3, 30, 28.0}   // �� ����� �������
};

// �����������
User* authenticate(const string& login, const string& password) {
    for (auto& [id, user] : users) {
        if (user.login == login && user.password == password) {
            return &user;
        }
    }
    return nullptr;
}

// ������
void checkScholarship(int studentId) {
    for (const auto& scholarship : scholarships) {
        if (scholarship.studentId == studentId) {
            cout << "���������: " << scholarship.amount << " ���.\n";
            return;
        }
    }
    cout << "��������� �����������.\n";
}

// ����������� ���������� � ����
void displaySchedule(const string& day) {
    if (schedule.find(day) == schedule.end() || schedule[day].empty()) {
        cout << "���������� �� " << day << " �����������.\n";
        return;
    }

    cout << "���������� �� " << day << ":\n";
    for (const auto& s : schedule[day]) {
        cout << "�������: " << s.subject << ", �������������: ";
        for (const auto& teacher : s.teachers) {
            cout << teacher << " ";
        }
        cout << ", �����: " << s.time << "\n";
    }
}

// ���������� ��������
void displayTeacherSchedule(int teacherId, const string& day) {
    double salary = 0.0;
    for (const auto& ts : teacherSalaries) {
        if (ts.teacherId == teacherId) {
            salary = ts.hoursPerMonth * ts.hourlyRate;
            cout << "����������� ��������: " << salary << " ���.\n";
            break;
        }
    }

    if (schedule.find(day) == schedule.end() || schedule[day].empty()) {
        cout << "���������� �� " << day << " �����������.\n";
        return;
    }

    cout << "���������� �� " << day << ":\n";
    for (const auto& s : schedule[day]) {
        for (const auto& teacher : s.teachers) {
            if (teachers[teacherId].fullName == teacher) {
                cout << "�������: " << s.subject << ", �����: " << s.time << "\n";
                cout << "��������: \n";
                for (int studentId : s.studentIds) {
                    if (students.find(studentId) != students.end()) {
                        cout << "- " << students[studentId].fullName << "\n";
                    }
                }
            }
        }
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "����� ���������� � ������� ������������ ����������������!\n";

    // User login
    string login, password;
    cout << "�����: ";
    cin >> login;
    cout << "������: ";
    cin >> password;

    User* user = authenticate(login, password);
    if (!user) {
        cout << "�������� ����� ��� ������.\n";
        return 1;
    }

    cout << "���� �������� �������!\n";

    // ������� ��� ���������
    if (user->role == "student") {
        for (const auto& [id, student] : students) {
            if (student.userId == user->id) {
                cout << "����� ����������, " << student.fullName << "!\n";
                checkScholarship(student.id);

                string day;
                cout << "������� ���� ������ ��� ��������� ����������: ";
                cin >> day;
                displaySchedule(day);

                break;
            }
        }
    }
    else if (user->role == "teacher") {
        for (const auto& [id, teacher] : teachers) {
            if (teacher.userId == user->id) {
                cout << "����� ����������, " << teacher.fullName << "!\n";

                string day;
                cout << "������� ���� ������ ��� ��������� ����������: ";
                cin >> day;
                displayTeacherSchedule(teacher.id, day);
                break;
            }
        }
    }
    else {
        cout << "��� ������ �� ������.\n";
    }

    return 0;
}
