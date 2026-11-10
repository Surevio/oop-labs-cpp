#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QListWidget>
#include <QPushButton>
#include "Container.h"

// окно со списком сотрудников и карточкой выбранного
class EditDialog : public QDialog {
public:
    EditDialog(QWidget* parent, Container* container);

private:
    void openAddEditDialog(bool editMode);
    void onDeleteItem();
    void updateItemList();
    void updateFields();
    void clearFields();

    Container* m_container = nullptr;
    QListWidget* m_listWidget = nullptr;
    QFormLayout* m_fieldsLayout = nullptr;
    QPushButton* m_editButton = nullptr;
    QPushButton* m_deleteButton = nullptr;
};
