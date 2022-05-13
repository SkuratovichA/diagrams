/** @file mainwindow.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 30.04.2022
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
//#include <filesystem>
#include <QDirIterator>

mainWindow::mainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::mainWindow) {
    ui->setupUi(this);
    setWindowTitle("Diagram");



    QDir directory(QDir::current());
    if (directory.isEmpty()) {
        return;
    }

    QDir d = QFileInfo("../examples/").absoluteDir();
    QString absolute=d.absolutePath();
    qDebug() << absolute;

    // create a list with examples
    QDirIterator it(directory.path(), QStringList() << "example*.gae", QDir::Files, QDirIterator::Subdirectories);
    it.next();
    do {
        auto *item = new QListWidgetItem(it.fileName());
        ui->listWidget->addItem(item);
        it.next();
    } while (it.hasNext());

    editor_window = nullptr;
}

mainWindow::~mainWindow() {
    delete ui;
}

void mainWindow::on_create_clicked() {
    try {
        editor_window = new editorInterface(this, nullptr, editorInterface::NO_FILE);
    } catch (const char *msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}

void mainWindow::on_open_clicked() {
    try {
        editor_window = new editorInterface(this, nullptr, editorInterface::OPEN_FILE);
    } catch (const char *msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}

void mainWindow::on_pushButton_clicked() {
    if (ui->listWidget->currentItem() == nullptr) {
        QMessageBox::information(this, "No such file", "pizda");
        return;
    }
    QString example_name = ui->listWidget->currentItem()->text();
    try {
        editor_window = new editorInterface(this, example_name, editorInterface::EXAMPLE_FILE);
    } catch (const char *msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}