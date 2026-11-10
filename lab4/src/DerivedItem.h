#pragma once

#include "BaseItem.h"
#include <boost/serialization/base_object.hpp>
#include <string>
#include <utility>

// сотрудник с должностью и зарплатой
class DerivedItem : public BaseItem {
public:
    DerivedItem() = default;
    DerivedItem(std::wstring name, int age, std::wstring position, double salary)
        : BaseItem(std::move(name), age), m_position(std::move(position)), m_salary(salary) {}

    QString getDisplayString() const override;
    void setupEditForm(QFormLayout* layout, bool readOnly) override;
    bool loadFromEditForm(QFormLayout* layout) override;

protected:
    QString paintText() const override;

private:
    std::wstring m_position;
    double m_salary = 0.0;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar & boost::serialization::base_object<BaseItem>(*this);
        ar & m_position;
        ar & m_salary;
    }
};
