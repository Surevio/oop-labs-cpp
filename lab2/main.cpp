#include <iostream>
#include <limits>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "RikhelgofGroup.h"

using namespace std;

void DisplayMenu()
{
    wcout << L"\n1. Добавить студента" << endl;
    wcout << L"2. Добавить старосту" << endl;
    wcout << L"3. Показать всех студентов" << endl;
    wcout << L"4. Загрузить из файла" << endl;
    wcout << L"5. Сохранить в файл" << endl;
    wcout << L"6. Очистить" << endl;
    wcout << L"0. Выход" << endl;
    wcout << L"Ваш выбор: ";
}

int main()
{
    // консоль в режиме UTF-16, иначе wcout и wcin портят кириллицу
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    RikhelgofGroup group;
    int choice = -1;

    do {
        DisplayMenu();
        if (!(wcin >> choice)) {
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            choice = -1;
            continue;
        }
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

        switch (choice) {
        case 1:
            group.addStudent();
            break;
        case 2:
            group.addStarosta();
            break;
        case 3:
            group.displayAllStudents();
            break;
        case 4: {
            wstring filename;
            wcout << L"Имя файла: ";
            getline(wcin, filename);
            group.readFromFile(filename);
            break;
        }
        case 5: {
            wstring filename;
            wcout << L"Имя файла: ";
            getline(wcin, filename);
            group.writeToFile(filename);
            break;
        }
        case 6:
            group.clear();
            break;
        case 0:
            break;
        default:
            wcout << L"Нет такого пункта" << endl;
        }
    } while (choice != 0);

    return 0;
}
