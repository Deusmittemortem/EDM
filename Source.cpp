#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
using namespace std;

// Структура
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
    {1, {1, 1, "Иван Иванов", "Инженерия", "Группа-101"}},
    {2, {2, 2, "Анна Смирнова", "Инженерия", "Группа-101"}}
};

unordered_map<int, Teacher> teachers = {
    {1, {1, 3, "Иван Иванов", "Математика"}},
    {2, {2, 4, "Алексей Петров", "Физика"}},
    {3, {3, 5, "Игорь Сидоров", "Химия"}}
};

vector<Scholarship> scholarships = {
    {1, 1000.0},
    {2, 1500.0}
};

unordered_map<string, vector<Schedule>> schedule = {
    {"Понедельник", {
        {"Понедельник", "Математика", {"Иван Иванов", "Игорь Сидоров"}, "09:00", {1, 2}},
        {"Понедельник", "Физика", {"Алексей Петров"}, "11:00", {1}}
    }},
    {"Вторник", {
        {"Вторник", "Химия", {"Игорь Сидоров", "Алексей Петров"}, "10:00", {2}},
        {"Вторник", "Программирование", {"Иван Иванов"}, "13:00", {1, 2}}
    }},
    {"Среда", {}},
    {"Четверг", {}},
    {"Пятница", {}}
};

vector<TeacherSalary> teacherSalaries = {
    {1, 40, 25.0},  // ЗП Иван Иванов
    {2, 35, 30.0},  // ЗП Алексей Петров
    {3, 30, 28.0}   // ЗП Игорь Сидоров
};

// Авторизация
User* authenticate(const string& login, const string& password) {
    for (auto& [id, user] : users) {
        if (user.login == login && user.password == password) {
            return &user;
        }
    }
    return nullptr;
}

// Степух
void checkScholarship(int studentId) {
    for (const auto& scholarship : scholarships) {
        if (scholarship.studentId == studentId) {
            cout << "Стипендия: " << scholarship.amount << " грн.\n";
            return;
        }
    }
    cout << "Стипендия отсутствует.\n";
}

// отображение расписания в день
void displaySchedule(const string& day) {
    if (schedule.find(day) == schedule.end() || schedule[day].empty()) {
        cout << "Расписание на " << day << " отсутствует.\n";
        return;
    }

    cout << "Расписание на " << day << ":\n";
    for (const auto& s : schedule[day]) {
        cout << "Предмет: " << s.subject << ", Преподаватели: ";
        for (const auto& teacher : s.teachers) {
            cout << teacher << " ";
        }
        cout << ", Время: " << s.time << "\n";
    }
}

// Расписание учителей
void displayTeacherSchedule(int teacherId, const string& day) {
    double salary = 0.0;
    for (const auto& ts : teacherSalaries) {
        if (ts.teacherId == teacherId) {
            salary = ts.hoursPerMonth * ts.hourlyRate;
            cout << "Ежемесячная зарплата: " << salary << " грн.\n";
            break;
        }
    }

    if (schedule.find(day) == schedule.end() || schedule[day].empty()) {
        cout << "Расписание на " << day << " отсутствует.\n";
        return;
    }

    cout << "Расписание на " << day << ":\n";
    for (const auto& s : schedule[day]) {
        for (const auto& teacher : s.teachers) {
            if (teachers[teacherId].fullName == teacher) {
                cout << "Предмет: " << s.subject << ", Время: " << s.time << "\n";
                cout << "Студенты: \n";
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
    cout << "Добро пожаловать в систему электронного документооборота!\n";

    // User login
    string login, password;
    cout << "Логин: ";
    cin >> login;
    cout << "Пароль: ";
    cin >> password;

    User* user = authenticate(login, password);
    if (!user) {
        cout << "Неверный логин или пароль.\n";
        return 1;
    }

    cout << "Вход выполнен успешно!\n";

    // запросы для студентов
    if (user->role == "student") {
        for (const auto& [id, student] : students) {
            if (student.userId == user->id) {
                cout << "Добро пожаловать, " << student.fullName << "!\n";
                checkScholarship(student.id);

                string day;
                cout << "Введите день недели для просмотра расписания: ";
                cin >> day;
                displaySchedule(day);

                break;
            }
        }
    }
    else if (user->role == "teacher") {
        for (const auto& [id, teacher] : teachers) {
            if (teacher.userId == user->id) {
                cout << "Добро пожаловать, " << teacher.fullName << "!\n";

                string day;
                cout << "Введите день недели для просмотра расписания: ";
                cin >> day;
                displayTeacherSchedule(teacher.id, day);
                break;
            }
        }
    }
    else {
        cout << "Для Админа не готово.\n";
    }

    return 0;
}
