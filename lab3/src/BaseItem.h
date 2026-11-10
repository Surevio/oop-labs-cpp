#pragma once

#include <QFontMetrics>
#include <QFormLayout>
#include <QPainter>
#include <QString>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <utility>

// базовый сотрудник картотеки: имя и возраст
class BaseItem {
public:
    BaseItem() = default;
    BaseItem(std::wstring name, int age) : m_name(std::move(name)), m_age(age) {}
    virtual ~BaseItem() = default;

    // рисует строку и сдвигает yPosition на следующую строку
    virtual void paint(QPainter* painter, int& yPosition) const;
    virtual QSize getTextSize(const QFontMetrics& metrics) const;
    virtual QString getDisplayString() const;
    virtual void setupEditForm(QFormLayout* layout, bool readOnly);
    virtual bool loadFromEditForm(QFormLayout* layout);

    const std::wstring& getName() const { return m_name; }
    int getAge() const { return m_age; }

protected:
    virtual QString paintText() const;

    std::wstring m_name;
    int m_age = 0;

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar & m_name;
        ar & m_age;
    }
};
