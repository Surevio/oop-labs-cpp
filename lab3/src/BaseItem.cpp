#include "BaseItem.h"
#include <QIntValidator>
#include <QLineEdit>

QString BaseItem::paintText() const {
    return QStringLiteral("%1 (%2 лет) - Базовый сотрудник")
        .arg(QString::fromStdWString(m_name))
        .arg(m_age);
}

void BaseItem::paint(QPainter* painter, int& yPosition) const {
    if (!painter) return;

    painter->save();
    painter->setFont(QFont("Arial", 10));
    painter->setPen(Qt::black);
    painter->drawText(20, yPosition, paintText());
    yPosition += 25;
    painter->restore();
}

QSize BaseItem::getTextSize(const QFontMetrics& metrics) const {
    return metrics.boundingRect(paintText()).size();
}

QString BaseItem::getDisplayString() const {
    return QString::fromStdWString(m_name) + QStringLiteral(" - Базовый сотрудник");
}

void BaseItem::setupEditForm(QFormLayout* layout, bool readOnly) {
    if (!layout) return;

    while (layout->rowCount() > 0) {
        layout->removeRow(0);
    }

    auto* nameEdit = new QLineEdit(QString::fromStdWString(m_name));
    nameEdit->setReadOnly(readOnly);
    layout->addRow(QStringLiteral("Имя:"), nameEdit);

    auto* ageEdit = new QLineEdit(QString::number(m_age));
    ageEdit->setReadOnly(readOnly);
    ageEdit->setValidator(new QIntValidator(0, 150, ageEdit));
    layout->addRow(QStringLiteral("Возраст:"), ageEdit);
}

bool BaseItem::loadFromEditForm(QFormLayout* layout) {
    if (!layout || layout->rowCount() < 2) return false;

    auto* nameItem = layout->itemAt(0, QFormLayout::FieldRole);
    auto* ageItem = layout->itemAt(1, QFormLayout::FieldRole);
    if (!nameItem || !ageItem) return false;

    auto* nameEdit = qobject_cast<QLineEdit*>(nameItem->widget());
    auto* ageEdit = qobject_cast<QLineEdit*>(ageItem->widget());
    if (!nameEdit || !ageEdit) return false;

    bool ok = false;
    const int age = ageEdit->text().toInt(&ok);
    if (!ok) return false;

    m_name = nameEdit->text().trimmed().toStdWString();
    m_age = age;
    return true;
}
