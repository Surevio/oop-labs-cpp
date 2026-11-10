#include "RikhelgofGroup.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

// порядок register_type при сохранении и загрузке должен совпадать,
// иначе архив не восстановит старосту по указателю на базовый класс
template<class Archive>
static void registerTypes(Archive& ar)
{
    ar.template register_type<RikhelgofStudent>();
    ar.template register_type<RikhelgofStarosta>();
}

void RikhelgofGroup::addStudent()
{
    auto newStudent = make_shared<RikhelgofStudent>();
    newStudent->SetStudent();
    Students.push_back(newStudent);
    wcout << L"Студент добавлен" << endl;
}

void RikhelgofGroup::addStarosta()
{
    auto newStarosta = make_shared<RikhelgofStarosta>();
    newStarosta->SetStudent();
    Students.push_back(newStarosta);
    wcout << L"Староста добавлен" << endl;
}

void RikhelgofGroup::displayAllStudents() const
{
    if (Students.empty()) {
        wcout << L"Группа пуста" << endl;
        return;
    }
    for (const auto& student : Students) {
        student->DisplayStudent();
        wcout << endl;
    }
}

void RikhelgofGroup::readFromFile(const wstring& filename)
{
    // path из wstring, чтобы открывались файлы с русскими именами
    ifstream inFile(filesystem::path(filename));
    if (!inFile) {
        wcerr << L"Не удалось открыть файл" << endl;
        return;
    }

    try {
        vector<shared_ptr<RikhelgofStudent>> loaded;
        boost::archive::text_iarchive ia(inFile);
        registerTypes(ia);
        ia >> loaded;
        Students.swap(loaded);
        wcout << L"Данные загружены, записей: " << Students.size() << endl;
    }
    catch (const exception& e) {
        wcerr << L"Ошибка при загрузке: " << e.what() << endl;
    }
}

void RikhelgofGroup::writeToFile(const wstring& filename) const
{
    ofstream outFile(filesystem::path(filename));
    if (!outFile) {
        wcerr << L"Не удалось открыть файл" << endl;
        return;
    }

    try {
        boost::archive::text_oarchive oa(outFile);
        registerTypes(oa);
        oa << Students;
        wcout << L"Данные сохранены" << endl;
    }
    catch (const exception& e) {
        wcerr << L"Ошибка при сохранении: " << e.what() << endl;
    }
}

void RikhelgofGroup::clear()
{
    Students.clear();
    wcout << L"Данные очищены" << endl;
}
