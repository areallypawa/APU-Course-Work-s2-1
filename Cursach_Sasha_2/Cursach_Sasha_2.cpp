#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <conio.h> 
#include <chrono>
#include <Windows.h>
#include <fstream>
#include <sstream>
using namespace std;

#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET   "\033[0m"

char fileName[20] = "output.txt";
char fileNameIn[20] = "input.txt";

int current = 0;
int current_change = 0;
int current_change_what = 0;

const int count_grades = 5;

int capacity = 2;
int sizeA = 0;

string items[] =
{
    "Добавить студента", "Добавить студентов с файла" ,"Внести правки в данные студента",
    "Все студенты университета", "Группы без стипендии",  "Выход"
};

string structure_items[] =
{
    "ФИО",
    "Пол (M/W)",
    "Группа",
    "Стипендия",
    "Оценки",
    "Форма обучения",
    "Дата изменения",
    "Числануть по приколу"
};


struct Profile
{
    string fullName;       // Фамилия Имя Отчество
    char sex;                   // Пол: M/W
    int group;                  // Номер группы
    int scholarship;            // Стипендия
    int grades[count_grades];	// Оценки за экзамены и диф. зачёты
    string depart;            // Форма обучения
    string date;           // Дата внесения изменнеия в запись (post/update) 
};


void printStudentInfo(const Profile& student, int number) {
    cout << "Студент " << number << "\n";
    cout << "  ФИО: " << student.fullName << endl;
    cout << "  Пол: " << (student.sex == 'M' ? "Мужской" : "Женский") << endl;
    cout << "  Группа: " << student.group << endl;
    cout << "  Форма обучения: " << student.depart << endl;
    cout << "  Оценки: ";
    for (int i = 0; i < count_grades; i++) {
        if (student.grades[i] > 0) {
            if (student.grades[i] >= 4) {
                cout << GREEN << student.grades[i] << " " << RESET;
            }
            else if (student.grades[i] == 3) {
                cout << YELLOW << student.grades[i] << " " << RESET;
            }
            else {
                cout << RED << student.grades[i] << " " << RESET;
            }
        }
        else {
            cout << "- ";
        }
    }
    cout << endl;

    // Вычисление среднего балла
    double sum = 0;
    int count = 0;
    for (int i = 0; i < count_grades; i++) {
        sum += student.grades[i];
        count++;
    }
    double average = (count > 0) ? sum / count : 0;
    cout << "  Средний балл: " << average << endl;

    // Определение успеваемости
    if (average >= 4.5) {
        cout << "  Статус: " << GREEN << "Отличник" << RESET << endl;
    }
    else if (average >= 3.5) {
        cout << "  Статус: " << YELLOW << "Хорошист" << RESET << endl;
    }
    else if (average > 0) {
        cout << "  Статус: " << RED << "Троечник" << RESET << endl;
    }

    // Есть ли стипендия?
    bool hasScholarship = true;
    for (int i = 0; i < count_grades; i++) {
        if (student.grades[i] > 0 && student.grades[i] < 4) {
            hasScholarship = false;
            break;
        }
    }
    cout << "  Стипендия: ";
    if (hasScholarship && average >= 4.0) {
             cout << GREEN << "Да : " << student.scholarship << RESET << endl;
         }
         else {
             cout << RED << "Нет" << RESET << endl;
	}

    // Дата изменения
    if (student.date != "") {
        cout << "  Дата изменения: " << student.date << endl;
    }

    cout << "------------------------\n";
}


void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void cin_ignore() {
    cin.ignore(100000, '\n');
}

void pause() {
    cout << '\n';
    system("Pause");
}


int getItemsCount() { return sizeof(items) / sizeof(items[0]); }
int getStructureItemsCount() { return sizeof(structure_items) / sizeof(structure_items[0]); }


int show_menu_simple(const string items[], int size, const string& title) {
    cout << "========== " << title << " ==========\n";

    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << items[i] << "\n";
    }

    cout << "Выберите пункт: ";

    int choice;
    cin >> choice;

    return choice - 1; // чтобы совпадало с индексами (0,1,2...)
}


bool writeFile(char* FileName, Profile* students, int size, int reWrite)
{
    if (reWrite == -1) {
        ofstream File(FileName);
        File << "";
        File.close();
        return 0;
    }

    ofstream File(FileName, (reWrite ? ios::app : ios::out));

    if (!File.is_open())
    {
        cout << "Открыть файл не удалось!\n";
        return 404;
    }

    for (int i = 0; i < size; i++)
    {
        if (students[i].fullName != "")
        {
            File << "Студент: " << students[i].fullName << "\n";
            File << "Группа: " << students[i].group << "\n";
            File << "Ship: " << students[i].scholarship << "\n";
            File << "Пол: " << students[i].sex << "\n";
            File << "Оценки: ";

            for (int j = 0; j < count_grades; j++)
            {
                File << students[i].grades[j] << ' ';
            }

            File << "\nФорма обучения: " << students[i].depart << "\n";
            File << "Дата последнего изменения: " << students[i].date << "\n\n";
        }
    }

    File.close();
    cout << "Данные внесены!";
    return 0;
}

bool readFile(char* FileName)
{
    ifstream File(FileName);   // Создали поток ввода для чтения данных из файла

    if (!File.is_open())
    {
        cout << "Открыть файл не удалось! \n";
        return 404;
    }

    char S[100];
    while (!File.eof()) // Читаем все строки из файла и выводим их на экран
    {
        File.getline(S, 100);
        cout << S << endl;
    }

    File.close();  // Закрываем файл
    return 0;
}

int loadFromFile(Profile*& students, int& size, int& capacity)
{
    ifstream File(fileNameIn);

    if (!File.is_open())
    {
        cout << "Файл не найден!\n";
        return 404;
    }

    string line;
    int loadedCount = 0;

    while (getline(File, line))
    {
        if (line.rfind("Студент", 0) == 0)   // быстрее и надежнее чем find
        {
            // resize если нужно
            if (size >= capacity)
            {
                capacity *= 2;

                Profile* temp = new Profile[capacity];

                for (int i = 0; i < size; i++)
                    temp[i] = students[i];

                delete[] students;
                students = temp;
            }

            Profile& p = students[size];

            // очистка перед записью (ВАЖНО чтобы не было мусора)
            p = Profile();

            // ФИО
            p.fullName = line.substr(line.find(":") + 2);

            // Группа
            if (!getline(File, line)) break;
            p.group = stoi(line.substr(line.find(":") + 2));

            // Стипендия
            if (!getline(File, line)) break;
            p.scholarship = stoi(line.substr(line.find(":") + 2));

            // Пол (ВАЖНО: теперь строка, не char)
            if (!getline(File, line)) break;
            string sexStr = line.substr(line.find(":") + 2);
            p.sex = sexStr.empty() ? 'M' : sexStr[0]; // fallback

            // Оценки
            if (!getline(File, line)) break;
            stringstream ss(line.substr(line.find(":") + 2));

            for (int i = 0; i < count_grades; i++)
            {
                if (!(ss >> p.grades[i]))
                    p.grades[i] = 0;
            }

            // Форма обучения
            if (!getline(File, line)) break;
            p.depart = line.substr(line.find(":") + 2);

            // Дата
            if (!getline(File, line)) break;
            p.date = line.substr(line.find(":") + 2);

            size++;
            loadedCount++;
        }
    }

    File.close();

    cout << GREEN << "Загружено студентов: " << loadedCount << RESET << endl;
    pause();

    writeFile(fileName, students, size, 0);

    return loadedCount;
}

void changeFio(Profile* p) {
    cout << "Текущее ФИО: " << p->fullName << endl;
    cout << "Введите новое ФИО: ";
    if (cin.peek() == '\n') {
        cin.ignore();
    }
    getline(cin, p->fullName);
    cout << "ФИО изменено!" << endl;
    pause();
}


void changeSex(Profile* p) {

    cout << "Текущий пол: " << p->sex << '\n';
    cout << "Введите новый пол (М/W): ";

    cin >> p->sex;
    p->sex = toupper(p->sex);

    while (cin.fail() || (p->sex != 'W' && p->sex != 'M')) {
        cin.clear();
        cin_ignore();
        cout << "\aОшибка ввода пола! Введите М или W: ";
        cin >> p->sex;
        p->sex = toupper(p->sex);
    }
    cin_ignore();
    cout << "Пол изменен на " << p->sex << '\n';
    pause();
}

void changeGroup(Profile* p) {
    cout << "Текущая группа: " << p->group << '\n';
    cout << "Введите новый номер группы: ";

    cin >> p->group;
    while (cin.fail()) {
        cin.clear();
        cin_ignore();
        cout << "\aОшибка! Введите число: ";
        cin >> p->group;
    }
    cin_ignore();
    cout << "Группа изменена на " << p->group << '\n';
    pause();
}


void changeShip(Profile* p) {
    cout << "Текущая стипендия : " << p->scholarship << "\n";
    cout << "Введите новую стипендию: ";

    cin >> p->scholarship;
    while (cin.fail()) {
        cin.clear();
        cin_ignore();
        cout << "\aОшибка! Введите число: ";
        cin >> p->scholarship;
    }
    cin_ignore();
    cout << "Стипендия изменена на " << p->scholarship << "\n";
    pause();
}


bool changeGrades(Profile* p, int index, int& size) {
    cout << "Текущие оценки: " << p->grades[0] << " " << p->grades[1] << "\n";
    cout << "Введите новые оценки за сессию (" << count_grades << " оценки):\n";

    for (int i = 0; i < count_grades; i++) {
        cin >> p->grades[i];

        while (cin.fail() || (p->grades[i] < 2 || p->grades[i] > 5)) {
            cin.clear();
            cin_ignore();
            cout << "\aОшибка! Оценка должна быть от 2 до 5: ";
            cin >> p->grades[i];
        }
    }
    cin_ignore();

    bool temp_two = 0;
    for (int i = 0; i < 2; i++) {
        if (p->grades[i] == 2) {
            temp_two = 1;
        }
    }
    if (!temp_two) {
        cout << "Все отлично, данные изменены!\n";
    }
    else {
        bool chislanut;
        cout << "Студент не может быть зачислен с двойками! Отчислить студента?(Yes - 1/No - 0): ";
        cin >> chislanut;
        if (chislanut) {
            // Сдвиг элементов влево
            for (int i = current_change; i < size - 1; i++) {
                p[i] = p[i + 1];
            }

            // Очистка последнего элемента (не обязательно, но аккуратно)
            p[size - 1] = Profile();

            // Сдвигаем указатель свободной позиции назад
            sizeA--;

            cout << GREEN << "Студент удалён!" << RESET;
            pause();
        }
        else {
            cout << "\nХорошо\n";
            
        }
        return 1;
    }
    pause();
    return 0;
}


void changeDepart(Profile* p) {
    cout << "Текущая форма обучения: " << p->depart << "\n";
    cout << "Введите новую форму обучения: ";

    if (cin.peek() == '\n') {
        cin.ignore();
    }
    cin >> p->depart;
    cout << "Форма обучения изменена на " << p->depart << "\n";
    pause();
}


void changeDate(Profile* p) {
    cout << "Текущая дата изменения: " << p->date;

    // Data
    time_t rawtime; time(&rawtime);
    p->date = ctime(&rawtime);

    cout << "Дата обновлена на " << p->date;
    pause();
}

void printStudent(Profile* p, int studentCount) {

    clear();

    if ((p->fullName) == "") {
        cout << RED << "Нет добавленных студентов!" << RESET;
        pause();
        return;
    }

    const string printItems[] = {
        "Вывести всех студентов",
        "Все студенты группы",
        "Топ студентов",
        "Студенты - мужчины",
        "Студенты - девушки",
        "Студенты без стипендии",
        "Студенты хорошисты",
        "Студенты отличники",
        "Назад"
    };
    int printCount = 9;
    int printCurrent = 0;

    do {
        printCurrent = show_menu_simple(printItems, printCount, "ПЕЧАТЬ");

        switch (printCurrent) {
        case 0:
            clear();
            cout << "=== ВСЕ СТУДЕНТЫ (" << studentCount << ") ===\n\n";
            if (studentCount == 0) {
                cout << RED << "Список студентов пуст!" << RESET << endl;
            }
            else {
                for (int i = 0; i < studentCount && p[i].fullName != ""; i++) {
                    printStudentInfo(p[i], i + 1);
                }
            }
            pause();
            clear();
            break;

        case 1: // Поиск по Group
            clear();
            cout << "Введите номер группы: ";
            {
                int searchGroup;
                cin >> searchGroup;

                bool found = false;
                cout << "\n=== СТУДЕНТЫ ГРУППЫ " << searchGroup << " ===\n\n";

                for (int i = 0; i < studentCount; i++) {
                    if (p[i].group == searchGroup && p[i].fullName != "") {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }

                if (!found) {
                    cout << RED << "Студенты группы " << searchGroup << " не найдены!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        case 2: // Топ студентов
            clear();
            cout << "=== ТОП СТУДЕНТОВ (по среднему баллу) ===\n\n";
            if (studentCount == 0) {
                cout << RED << "Нет студентов для рейтинга!" << RESET << endl;
            }
            else {
                struct StudentRating {
                    string fullName;
                    int group;
                    int schoolarship;
                    double average;
                };

                StudentRating ratings[100];

                // Вычисляем средние баллы
                for (int i = 0; i < studentCount; i++) {
                    double sum = 0;
                    int count = 0;
                    for (int j = 0; j < count_grades; j++) {
                        if (p[i].grades[j] > 0 && p[i].fullName != "") {
                            sum += p[i].grades[j];
                            count++;
                        }
                    }
                    ratings[i].average = (count > 0) ? sum / count : 0;
                    ratings[i].fullName = p[i].fullName;
                    ratings[i].group = p[i].group;
					ratings[i].schoolarship = p[i].scholarship;
                }

                // Сортировка
                for (int i = 0; i < studentCount - 1; i++) {
                    for (int j = 0; j < studentCount - i - 1; j++) {
                        if (ratings[j].average < ratings[j + 1].average) {
                            StudentRating temp = ratings[j];
                            ratings[j] = ratings[j + 1];
                            ratings[j + 1] = temp;
                        }
                    }
                }

                // Вывод
                for (int i = 0; i < studentCount; i++) {
                    if (ratings[i].average > 0 && p[i].fullName != "") {
                        cout << "Место #" << i + 1 << ":\n";
                        cout << "  ФИО: " << ratings[i].fullName << endl;
                        cout << "  Группа: " << ratings[i].group << endl;
                        cout << "  Стипендия: " << ratings[i].schoolarship << endl;
                        cout << "  Средний балл: " << ratings[i].average << endl;

                        if (ratings[i].average >= 4.5) {
                            cout << "  Статус: " << GREEN << "Отличник" << RESET << endl;
                        }
                        else if (ratings[i].average >= 3.5) {
                            cout << "  Статус: " << YELLOW << "Хорошист" << RESET << endl;
                        }
                        cout << "------------------------\n";
                    }
                }
            }
            pause();
            clear();
            break;
        case 3: // Студенты мужчины
            clear();
            cout << "=== СТУДЕНТЫ - МУЖЧИНЫ ===\n\n";
            {
                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    if (p[i].sex == 'M' && p[i].fullName != "") {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }
                if (!found) {
                    cout << RED << "Студенты-мужчины не найдены!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        case 4: // Студенты девушки
            clear();
            cout << "=== СТУДЕНТЫ - ДЕВУШКИ ===\n\n";
            {
                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    if (p[i].sex == 'Ж' && p[i].fullName != "") {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }
                if (!found) {
                    cout << RED << "Студенты-девушки не найдены!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        case 5: // Студенты без стипендии
            clear();
            cout << "=== СТУДЕНТЫ БЕЗ СТИПЕНДИИ ===\n\n";
            {
                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    bool hasScholarship = true;
                    for (int j = 0; j < count_grades; j++) {
                        if (p[i].grades[j] > 0 && p[i].grades[j] < 4 && p[i].fullName != "") {
                            hasScholarship = false;
                            break;
                        }
                    }

                    if (!hasScholarship) {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }
                if (!found) {
                    cout << GREEN << "Все студенты получают стипендию!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        case 6: // Студенты хорошисты
            clear();
            cout << "=== СТУДЕНТЫ - ХОРОШИСТЫ ===\n\n";
            {
                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    double sum = 0;
                    int count = 0;
                    for (int j = 0; j < count_grades; j++) {
                        if (p[i].grades[j] > 0 && p[i].fullName != "") {
                            sum += p[i].grades[j];
                            count++;
                        }
                    }
                    double average = (count > 0) ? sum / count : 0;

                    if (average >= 3.5 && average < 4.5) {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }
                if (!found) {
                    cout << RED << "Хорошисты не найдены!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        case 7: // Студенты отличники
            clear();
            cout << "=== СТУДЕНТЫ - ОТЛИЧНИКИ ===\n\n";
            {
                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    double sum = 0;
                    int count = 0;
                    for (int j = 0; j < count_grades; j++) {
                        if (p[i].grades[j] > 0 && p[i].fullName != "") {
                            sum += p[i].grades[j];
                            count++;
                        }
                    }
                    double average = (count > 0) ? sum / count : 0;

                    if (average >= 4.5) {
                        printStudentInfo(p[i], i + 1);
                        found = true;
                    }
                }
                if (!found) {
                    cout << RED << "Отличники не найдены!" << RESET << endl;
                }
            }
            pause();
            clear();
            break;
        }

    } while (printCurrent != printCount - 1);
}

void groupsWithoutScholarship(Profile* students, int size)
{
    int limit;

    cout << "Введите минимальное количество студентов без стипендии: ";
    cin >> limit;

    if (cin.fail())
    {
        cin.clear();
        cin_ignore();
        cout << "Ошибка ввода!\n";
        return;
    }

    // допустим группы максимум 1000 (можешь увеличить)
    int groups[1000] = { 0 };
    int groupIds[1000];
    int groupCount = 0;

    // собираем статистику
    for (int i = 0; i < size; i++)
    {
        if (students[i].fullName == "") continue;

        bool hasScholarship = true;

        for (int j = 0; j < count_grades; j++)
        {
            if (students[i].grades[j] > 0 && students[i].grades[j] < 4)
            {
                hasScholarship = false;
                break;
            }
        }

        if (!hasScholarship)
        {
            int g = students[i].group;

            // ищем группу
            int idx = -1;
            for (int k = 0; k < groupCount; k++)
            {
                if (groupIds[k] == g)
                {
                    idx = k;
                    break;
                }
            }

            if (idx == -1)
            {
                groupIds[groupCount] = g;
                groups[groupCount] = 1;
                groupCount++;
            }
            else
            {
                groups[idx]++;
            }
        }
    }

    // сортировка по номеру группы
    for (int i = 0; i < groupCount - 1; i++)
    {
        for (int j = 0; j < groupCount - i - 1; j++)
        {
            if (groupIds[j] > groupIds[j + 1])
            {
                swap(groupIds[j], groupIds[j + 1]);
                swap(groups[j], groups[j + 1]);
            }
        }
    }

    cout << "\n=== ГРУППЫ БЕЗ СТИПЕНДИИ > " << limit << " ===\n";

    bool found = false;

    for (int i = 0; i < groupCount; i++)
    {
        if (groups[i] > limit)
        {
            cout << "Группа " << groupIds[i]
                << " -> " << groups[i] << " студентов\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "Нет подходящих групп\n";
    }

    pause();
}

void deleteAllStudents(Profile* students, Profile** p_profile)
{
    int n = *p_profile - students;
    if (n == 0) {
        cout << RED << "База уже пуста!" << RESET;
        pause();
        return;
    }
    for (int i = 0; i < n; i++) {
        students[i] = Profile();
    }

    *p_profile = students;

    ofstream file(fileName, ios::trunc);
    file.close();

    cout << GREEN << "Все студенты удалены!" << RESET;
    pause();
}

void createStudent(Profile* p, Profile* p_students) {
    while (true) {
        clear();

        bool temp_two = 0;

        cout << "ФИО:";
        if (cin.peek() == '\n') {
            cin.ignore();
        }
        getline(cin, p->fullName);

        cout << "Пол:";
        cin >> p->sex;
        p->sex = toupper(p->sex);
        // проверка
        while (cin.fail() || ((p->sex) != 'W' && (p->sex) != 'M')) {
            cin.clear();
            cin_ignore();
            cout << "\aОшибка ввода пола, попробуй еще раз\n";
            cout << "Пол:";
            cin >> p->sex;
            p->sex = toupper(p->sex);

        }
        cin_ignore();

        cout << "Номер группы:";
        cin >> p->group;
        // проверка
        while (cin.fail()) {
            cin.clear();
            cin_ignore();
            cout << "\aОшибка ввода группы, попробуй еще раз\n";
            cout << "Номер группы:";
            cin >> p->group;
        }
        cin_ignore();

		cout << "Стипендия:";
        cin >> p->scholarship;
        // проверка
        while (cin.fail()) {
            cin.clear();
            cin_ignore();
            cout << "\aОшибка ввода id, попробуй еще раз\n";
            cout << "Номер группы:";
            cin >> p->group;
        }
        cin_ignore();

        cout << "Оценки за прошедшую сессию:";

        for (int i = 0; i < count_grades; i++) {
            cin >> (p->grades)[i];
            // проверка

            while (cin.fail() || (p->grades[i]) == 2 || (p->grades[i]) < 2 || (p->grades[i]) > 5) {
                cin.clear();
                cin_ignore();
                cout << "\aОшибка ввода оценки, попробуй еще раз\n";
                cout << "Оценки: ";
                cin >> (p->grades)[i];
            }
            if ((p->grades)[i] == 2) temp_two = 1;
        }
        cin_ignore();

        cout << "Форма обучения:";
        cin >> p->depart;

        // Data
        time_t rawtime; time(&rawtime);
        p->date = ctime(&rawtime);

        if (!temp_two) {
            char ok;
            cout << "Данные введены верно? (Y/N)"; cin >> ok;
            if (ok == 'Y' || ok == 'y') {
                writeFile(fileName, p, sizeA, 0);
                break;  // выходим из цикла
            }
        }
        else {
            cout << "Студент не может быть зачислен с двойками!";
            pause();
            continue;
        }

    }

    pause();
    clear();
}

int chooseStudent(Profile* students, int size) {
    cout << "====== Выберите студента ======\n";
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ". " << students[i].fullName << "\n";
    }

    int choice;
    cout << "Введите номер студента: ";
    cin >> choice;
    return choice - 1; // чтобы совпадало с индексом
}
int chooseField(const string structure_items[], int count, const string& studentName) {
    cout << "====== Что изменить у " << studentName << " ======\n";
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << structure_items[i] << "\n";
    }

    int choice;
    cout << "Введите номер поля: ";
    cin >> choice;
    return choice - 1; // индекс
}
void changeStudent(Profile* students, int& size) {
    clear();


    if (size == 0) {
        cout << RED << "Нет студентов для изменения!" << RESET;
        pause();
        return;
    }

    int current_change = chooseStudent(students, size);
    clear();

    int current_change_what = chooseField(structure_items, getStructureItemsCount(), students[current_change].fullName);
    clear();

    switch (current_change_what) {
    case 0: changeFio(students + current_change); break;
    case 1: changeSex(students + current_change); break;
    case 2: changeGroup(students + current_change); break;
    case 3: changeShip(students + current_change); break;
    case 4: changeGrades(students, current_change, size); break;
    case 5: changeDepart(students + current_change); break;
    case 6: changeDate(students + current_change); break;
    case 7: {
        
        // сдвиг элементов влево
        for (int i = current_change; i < sizeA - 1; i++) {
            students[i] = students[i + 1];
        }

        students[sizeA - 1] = Profile();
        sizeA--;
        cout << GREEN << "Студент удалён!" << RESET;
        pause();
        break;
    }
    }

    writeFile(fileName, students, size, 0);
    clear();
}

int main(){

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
	
    Profile* students = new Profile[capacity];

    //writeFile(fileName, students, sizeA, 0);
    int loaded;
    do {
        current = show_menu_simple(items, getItemsCount(), "МЕНЮ");

        switch(current) {
        case 0:
            if (sizeA >= capacity)
            {
                capacity *= 2;

                Profile* temp = new Profile[capacity];

                for (int i = 0; i < sizeA; i++)
                {
                    temp[i] = students[i];
                }

                delete[] students;

                students = temp;
            }

            createStudent(&students[sizeA], students);

            sizeA++;
            break;

        case 1:
            loaded = loadFromFile(students, sizeA, capacity);
           
            break;

        case 2:
            if (sizeA != 0) {
                changeStudent(students, sizeA);
            }
            else {
                clear();
                cout << RED << "Нет добавленных студентов!" << RESET;
                pause();
            }
            break;

        case 3:
            printStudent(students, sizeA);
            break;
        case 4:
            clear();
            groupsWithoutScholarship(students, sizeA);
            break;
        }


        clear();

    } while (current != getItemsCount() - 1);

    delete[] students;
    return 0;

}


