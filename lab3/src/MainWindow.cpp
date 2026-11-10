#include "MainWindow.h"
#include "DerivedItem.h"
#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <memory>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("Лабораторная работа 3 - Картотека"));
    resize(800, 500);

    auto* fileMenu = menuBar()->addMenu(QStringLiteral("Файл"));
    auto* saveAction = fileMenu->addAction(QStringLiteral("Сохранить"));
    auto* loadAction = fileMenu->addAction(QStringLiteral("Загрузить"));
    auto* clearAction = fileMenu->addAction(QStringLiteral("Очистить"));
    fileMenu->addSeparator();
    auto* exitAction = fileMenu->addAction(QStringLiteral("Выход"));

    connect(saveAction, &QAction::triggered, this, [this] { onSaveToFile(); });
    connect(loadAction, &QAction::triggered, this, [this] { onLoadFromFile(); });
    connect(clearAction, &QAction::triggered, this, [this] { onClearData(); });
    connect(exitAction, &QAction::triggered, this, [this] { close(); });

    // пара записей для примера, чтобы окно не было пустым
    m_container.addItem(std::make_shared<BaseItem>(L"Иван Иванов", 30));
    m_container.addItem(std::make_shared<DerivedItem>(L"Петр Петров", 35, L"Инженер", 125000));
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    const int menuBarHeight = menuBar()->height();
    const QRect tableRect(0, menuBarHeight, width(), height() - menuBarHeight);

    if (m_container.size() > 0) {
        m_container.paintTable(&painter, tableRect);
    } else {
        painter.setFont(QFont("Arial", 10));
        painter.drawText(tableRect, Qt::AlignCenter,
                         QStringLiteral("Нет данных. Загрузите файл через меню \"Файл\""));
    }
}

void MainWindow::onSaveToFile() {
    if (m_container.size() == 0) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Нет данных для сохранения"));
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this, QStringLiteral("Сохранить данные"),
        QDir::homePath() + QStringLiteral("/data.dat"),
        QStringLiteral("Файлы данных (*.dat);;Все файлы (*)"));
    if (filename.isEmpty()) return;

    if (!filename.endsWith(QStringLiteral(".dat"), Qt::CaseInsensitive)) {
        filename += QStringLiteral(".dat");
    }

    if (m_container.saveToFile(filename)) {
        QMessageBox::information(this, QStringLiteral("Готово"), QStringLiteral("Данные сохранены"));
    } else {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Не удалось сохранить файл"));
    }
}

void MainWindow::onLoadFromFile() {
    const QString filename = QFileDialog::getOpenFileName(
        this, QStringLiteral("Загрузить данные"), QDir::homePath(),
        QStringLiteral("Файлы данных (*.dat);;Все файлы (*)"));
    if (filename.isEmpty()) return;

    if (m_container.loadFromFile(filename)) {
        update();
        QMessageBox::information(this, QStringLiteral("Готово"), QStringLiteral("Данные загружены"));
    } else {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Не удалось загрузить файл"));
    }
}

void MainWindow::onClearData() {
    if (m_container.size() == 0) {
        QMessageBox::information(this, QStringLiteral("Картотека"), QStringLiteral("Картотека уже пуста"));
        return;
    }

    const auto reply = QMessageBox::question(
        this, QStringLiteral("Подтверждение"),
        QStringLiteral("Очистить все данные?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_container.clear();
        update();
    }
}
