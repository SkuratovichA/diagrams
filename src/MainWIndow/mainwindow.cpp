#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <filesystem>
#include <QDirIterator>

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Diagram");

    QDir directory(QDir::current()); // or you can use root()
    if (directory.isEmpty()) {
        return;
    }

    // create a list with examples
    QDirIterator it(directory.path(), QStringList() << "example*.json", QDir::Files, QDirIterator::Subdirectories);
    it.next();
    do {
        auto *item = new QListWidgetItem(it.fileName());
        ui->listWidget->addItem(item);
        it.next();
    } while (it.hasNext());

    editor_window = nullptr;
}

mainWindow::~mainWindow()
{
    delete ui;
}

/**
 * Create a new file.
 * Filename will not be provided - just create a window with a default interface
 */
void mainWindow::on_create_clicked()
{
    try {
        editor_window = new editorInterface(this, nullptr, editorInterface::NO_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}

/**
 * Open a file from a given list
 */
void mainWindow::on_open_clicked()
{
    try {
        editor_window = new editorInterface(this, nullptr, editorInterface::OPEN_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}

/**
 * Open a canvas with a default template
 * when saving, the path will must be specified
 */
void mainWindow::on_pushButton_clicked()
{
    if (ui->listWidget->currentItem() == nullptr) {
        QMessageBox::information(this, "No such file", "pizda");
        return;
    }
    QString example_name = ui->listWidget->currentItem()->text();
    try {
        editor_window = new editorInterface(this, example_name, editorInterface::EXAMPLE_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    editor_window->show();
}
