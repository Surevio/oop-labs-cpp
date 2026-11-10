#include "DerivedItem.h"
#include <QDoubleValidator>
#include <QLineEdit>

QString DerivedItem::paintText() const {
    return QStringLiteral("%1 (%2 лет) - %3 (%4 руб.)")
        .arg(QString::fromStdWString(m_name))
        .arg(m_age)
        .arg(QString::fromStdWString(m_position))
        .arg(m_salary, 0, 'f', 2);
}

QString DerivedItem::getDisplayString() const {
    return QStringLiteral("%1 - %2 (%3 руб.)")
        .arg(QString::fromStdWString(m_name))
        .arg(QString::fromStdWString(m_position))
        .arg(m_salary, 0, 'f', 2);
}

void DerivedItem::setupEditForm(QFormLayout* layout, bool readOnly) {
    BaseItem::setupEditForm(layout, readOnly);
    if (!layout) return;

    auto* positionEdit = new QLineEdit(QString::fromStdWString(m_position));
    positionEdit->setReadOnly(readOnly);
    layout->addRow(QStringLiteral("Должность:"), positionEdit);

    auto* salaryEdit = new QLineEdit(QString::number(m_salary, 'f', 2));
    salaryEdit->setReadOnly(readOnly);
    auto* validator = new QDoubleValidator(0, 100000000, 2, salaryEdit);
    validator->setLocale(QLocale::c());  // точка как разделитель, как в toDouble()
    salaryEdit->setValidator(validator);
    layout->addRow(QStringLiteral("Зарплата:"), salaryEdit);
}

bool DerivedItem::loadFromEditForm(QFormLayout* layout) {
    if (!BaseItem::loadFromEditForm(layout) || layout->rowCount() < 4) return false;

    auto* positionItem = layout->itemAt(2, QFormLayout::FieldRole);
    auto* salaryItem = layout->itemAt(3, QFormLayout::FieldRole);
    if (!positionItem || !salaryItem) return false;

    auto* positionEdit = qobject_cast<QLineEdit*>(positionItem->widget());
    auto* salaryEdit = qobject_cast<QLineEdit*>(salaryItem->widget());
    if (!positionEdit || !salaryEdit) return false;

    bool ok = false;
    const double salary = salaryEdit->text().replace(',', '.').toDouble(&ok);
    if (!ok) return false;

    m_position = positionEdit->text().trimmed().toStdWString();
    m_salary = salary;
    return true;
}
