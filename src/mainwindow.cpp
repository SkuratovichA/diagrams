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
    qDebug() << "create Mainwindow : constructor mainWindow.cpp";
    qDebug() << "create Editorinterface ui : constructor mainWindow.cpp";
    ui->setupUi(this);
    this->setWindowTitle("diagram");

    QDir directory(QDir::current()); // or you can use root()
    //qDebug() << directory.absolutePath();
    if (directory.isEmpty()) {
        return;
    }

    // create a list with examples
    QDirIterator it(directory.path(), QStringList() << "example*.json", QDir::Files, QDirIterator::Subdirectories);
    it.next();
    do {
        //qDebug() << it.fileName();
        auto *item = new QListWidgetItem(it.fileName());
        ui->listWidget->addItem(item);
        it.next();
    } while(it.hasNext());

    editor_window = nullptr;
}

mainWindow::~mainWindow()
{
    qDebug() << "delete Mainwindow : destructor mainWindow.cpp";
    qDebug() << "delete Editorinterface ui : destructor mainWindow.cpp";
    delete ui;
}

////////////////////////////////////////////////////////////////////////////
// create a new file.
// filename will not be provided - just create a window with a default interface
void mainWindow::on_create_clicked()
{
    try {
        this->editor_window = new editorInterface(this, nullptr, editorInterface::NO_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    this->editor_window->show();
}

// open file from a given list
void mainWindow::on_open_clicked()
{
    try {
         this->editor_window = new editorInterface(this, nullptr, editorInterface::OPEN_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    this->editor_window->show();
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
        this->editor_window = new editorInterface(this, example_name, editorInterface::EXAMPLE_FILE);
    } catch (const char* msg) {
        QMessageBox::warning(this, "Error", msg);
        return;
    }
    this->editor_window->show();
}
