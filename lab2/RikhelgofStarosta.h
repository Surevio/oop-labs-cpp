#pragma once
#include "RikhelgofStudent.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

class RikhelgofStarosta : public RikhelgofStudent
{
private:
    std::wstring responsibility;
    int startYear;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<RikhelgofStudent>(*this);
        ar & responsibility;
        ar & startYear;
    }

public:
    RikhelgofStarosta();

    void SetStudent() override;
    void DisplayStudent() const override;
};
