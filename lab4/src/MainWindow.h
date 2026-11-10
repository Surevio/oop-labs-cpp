#pragma once

#include <QMainWindow>
#include "Container.h"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void onSaveToFile();
    void onLoadFromFile();
    void onClearData();
    void onEditData();

    Container m_container;
};
