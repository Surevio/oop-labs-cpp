#include "AddEditDialog.h"
#include "DerivedItem.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

AddEditDialog::AddEditDialog(QWidget* parent, Container* container, int editIndex)
    : QDialog(parent), m_container(container), m_editIndex(editIndex) {
    setWindowTitle(isEditMode() ? QStringLiteral("Изменение сотрудника")
                                : QStringLiteral("Добавление сотрудника"));
    resize(500, 340);

    auto* mainLayout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget(this);

    auto* baseTab = new QWidget;
    m_baseFormLayout = new QFormLayout(baseTab);
    m_tabWidget->addTab(baseTab, QStringLiteral("Базовый сотрудник"));

    auto* derivedTab = new QWidget;
    m_derivedFormLayout = new QFormLayout(derivedTab);
    m_tabWidget->addTab(derivedTab, QStringLiteral("Сотрудник с должностью"));

    mainLayout->addWidget(m_tabWidget);

    auto* saveButton = new QPushButton(isEditMode() ? QStringLiteral("Сохранить")
                                                    : QStringLiteral("Добавить"), this);
    auto* cancelButton = new QPushButton(QStringLiteral("Отмена"), this);
    auto* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, [this] { onSaveItem(); });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    if (isEditMode()) {
        const auto original = m_container->getItem(m_editIndex);
        // работаем с копией, чтобы отмена или ошибка ввода не меняли запись
        if (auto derived = std::dynamic_pointer_cast<DerivedItem>(original)) {
            m_item = std::make_shared<DerivedItem>(*derived);
        } else if (original) {
            m_item = std::make_shared<BaseItem>(*original);
        }

        const bool isDerived = std::dynamic_pointer_cast<DerivedItem>(m_item) != nullptr;
        m_tabWidget->setCurrentIndex(isDerived ? 1 : 0);
        m_tabWidget->setTabEnabled(isDerived ? 0 : 1, false);  // тип записи при изменении не меняется
        if (m_item) m_item->setupEditForm(currentLayout(), false);
    } else {
        onItemTypeChanged(0);
        connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int i) { onItemTypeChanged(i); });
    }
}

QFormLayout* AddEditDialog::currentLayout() const {
    return m_tabWidget->currentIndex() == 0 ? m_baseFormLayout : m_derivedFormLayout;
}

void AddEditDialog::onItemTypeChanged(int tabIndex) {
    while (m_baseFormLayout->rowCount() > 0) m_baseFormLayout->removeRow(0);
    while (m_derivedFormLayout->rowCount() > 0) m_derivedFormLayout->removeRow(0);

    if (tabIndex == 0) {
        m_item = std::make_shared<BaseItem>();
    } else {
        m_item = std::make_shared<DerivedItem>();
    }
    m_item->setupEditForm(currentLayout(), false);
}

void AddEditDialog::onSaveItem() {
    if (!m_item || !m_container) return;

    if (!m_item->loadFromEditForm(currentLayout()) || m_item->getName().empty() || m_item->getAge() <= 0) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"),
                             QStringLiteral("Заполните все поля корректно"));
        return;
    }

    if (isEditMode()) {
        m_container->replaceItem(m_editIndex, m_item);
    } else {
        m_container->addItem(m_item);
    }
    accept();
}
