#pragma once
#include "RikhelgofStudent.h"
#include "RikhelgofStarosta.h"
#include <memory>
#include <string>
#include <vector>

class RikhelgofGroup
{
private:
    std::vector<std::shared_ptr<RikhelgofStudent>> Students;

public:
    void addStudent();
    void addStarosta();
    void displayAllStudents() const;
    void readFromFile(const std::wstring& filename);
    void writeToFile(const std::wstring& filename) const;
    void clear();
};
