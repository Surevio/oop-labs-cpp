#include "RikhelgofStudent.h"
#include <iostream>
#include <limits>

using namespace std;

RikhelgofStudent::RikhelgofStudent() : name(L""), age(0), group(L"") {}

void RikhelgofStudent::SetStudent()
{
    wcout << L"Введите имя: ";
    getline(wcin >> ws, name);

    wcout << L"Введите возраст: ";
    while (!(wcin >> age) || age <= 0) {
        wcin.clear();
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        wcout << L"Возраст должен быть положительным числом, повторите: ";
    }
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

    wcout << L"Введите группу: ";
    getline(wcin, group);
}

void RikhelgofStudent::DisplayStudent() const
{
    wcout << L"Имя: " << name << endl;
    wcout << L"Возраст: " << age << endl;
    wcout << L"Группа: " << group << endl;
}
