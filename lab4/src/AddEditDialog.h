#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QTabWidget>
#include <memory>
#include "Container.h"

// окно добавления (editIndex = -1) или изменения сотрудника
class AddEditDialog : public QDialog {
public:
    AddEditDialog(QWidget* parent, Container* container, int editIndex = -1);

private:
    bool isEditMode() const { return m_editIndex >= 0; }
    void onItemTypeChanged(int tabIndex);
    void onSaveItem();
    QFormLayout* currentLayout() const;

    Container* m_container = nullptr;
    int m_editIndex = -1;
    std::shared_ptr<BaseItem> m_item;  // редактируемая копия, в картотеку попадает только после проверки

    QTabWidget* m_tabWidget = nullptr;
    QFormLayout* m_baseFormLayout = nullptr;
    QFormLayout* m_derivedFormLayout = nullptr;
};
