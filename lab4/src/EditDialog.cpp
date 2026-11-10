#include "EditDialog.h"
#include "AddEditDialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <algorithm>

EditDialog::EditDialog(QWidget* parent, Container* container)
    : QDialog(parent), m_container(container) {
    setWindowTitle(QStringLiteral("Редактирование картотеки"));
    resize(760, 520);

    auto* rootLayout = new QVBoxLayout(this);
    auto* bodyLayout = new QHBoxLayout;

    m_listWidget = new QListWidget(this);
    auto* fieldsWidget = new QWidget(this);
    m_fieldsLayout = new QFormLayout(fieldsWidget);

    bodyLayout->addWidget(m_listWidget, 1);
    bodyLayout->addWidget(fieldsWidget, 2);
    rootLayout->addLayout(bodyLayout);

    auto* addButton = new QPushButton(QStringLiteral("Добавить"), this);
    m_editButton = new QPushButton(QStringLiteral("Изменить"), this);
    m_deleteButton = new QPushButton(QStringLiteral("Удалить"), this);
    auto* closeButton = new QPushButton(QStringLiteral("Закрыть"), this);

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(m_editButton);
    buttonsLayout->addWidget(m_deleteButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(closeButton);
    rootLayout->addLayout(buttonsLayout);

    connect(m_listWidget, &QListWidget::currentRowChanged, this, [this] { updateFields(); });
    connect(addButton, &QPushButton::clicked, this, [this] { openAddEditDialog(false); });
    connect(m_editButton, &QPushButton::clicked, this, [this] { openAddEditDialog(true); });
    connect(m_deleteButton, &QPushButton::clicked, this, [this] { onDeleteItem(); });
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    updateItemList();
    if (m_listWidget->count() > 0) m_listWidget->setCurrentRow(0);
}

void EditDialog::updateItemList() {
    m_listWidget->clear();
    int index = 0;

    m_container->forEach([&](const std::shared_ptr<BaseItem>& item) {
        m_listWidget->addItem(QStringLiteral("%1. %2").arg(index + 1).arg(item->getDisplayString()));
        ++index;
    });

    updateFields();
}

void EditDialog::clearFields() {
    while (m_fieldsLayout->rowCount() > 0) m_fieldsLayout->removeRow(0);
    auto* emptyLabel = new QLabel(QStringLiteral("Выберите сотрудника в списке"));
    emptyLabel->setWordWrap(true);
    m_fieldsLayout->addRow(emptyLabel);
}

void EditDialog::updateFields() {
    const auto selectedItem = m_container->getItem(m_listWidget->currentRow());
    m_editButton->setEnabled(selectedItem != nullptr);
    m_deleteButton->setEnabled(selectedItem != nullptr);

    if (selectedItem) {
        selectedItem->setupEditForm(m_fieldsLayout, true);
    } else {
        clearFields();
    }
}

void EditDialog::openAddEditDialog(bool editMode) {
    const int editIndex = editMode ? m_listWidget->currentRow() : -1;
    if (editMode && editIndex < 0) return;

    // новое окно на каждый вызов, чтобы заголовок и кнопки соответствовали режиму
    AddEditDialog dialog(this, m_container, editIndex);
    if (dialog.exec() != QDialog::Accepted) return;

    updateItemList();
    // после изменения выделяем ту же строку, после добавления последнюю
    m_listWidget->setCurrentRow(editMode ? editIndex : m_listWidget->count() - 1);
}

void EditDialog::onDeleteItem() {
    const int currentRow = m_listWidget->currentRow();
    if (currentRow < 0) return;

    const auto reply = QMessageBox::question(
        this, QStringLiteral("Подтверждение"),
        QStringLiteral("Удалить выбранного сотрудника?"),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    m_container->removeItem(currentRow);
    updateItemList();

    const int count = m_listWidget->count();
    if (count > 0) m_listWidget->setCurrentRow(std::min(currentRow, count - 1));
}
