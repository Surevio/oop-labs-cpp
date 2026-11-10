#include "RikhelgofStarosta.h"
#include <iostream>
#include <limits>

using namespace std;

RikhelgofStarosta::RikhelgofStarosta() : responsibility(L""), startYear(0) {}

void RikhelgofStarosta::SetStudent()
{
    RikhelgofStudent::SetStudent();

    wcout << L"Введите обязанности: ";
    getline(wcin >> ws, responsibility);

    wcout << L"Введите год начала срока: ";
    while (!(wcin >> startYear)) {
        wcin.clear();
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        wcout << L"Год должен быть числом, повторите: ";
    }
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
}

void RikhelgofStarosta::DisplayStudent() const
{
    RikhelgofStudent::DisplayStudent();
    wcout << L"Обязанности: " << responsibility << endl;
    wcout << L"Год начала срока: " << startYear << endl;
}
