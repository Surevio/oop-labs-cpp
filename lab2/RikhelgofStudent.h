#pragma once
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

class RikhelgofStudent
{
protected:
    std::wstring name;
    int age;
    std::wstring group;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & name;
        ar & age;
        ar & group;
    }

public:
    RikhelgofStudent();
    virtual ~RikhelgofStudent() = default;

    virtual void SetStudent();
    virtual void DisplayStudent() const;
};
