#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <filesystem>


//******************************************************************


mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("diagrams");

    // TODO: generalize andrei will do this
    QString dirname = "/Users/suka/vut/sem4/icp/diagrams/examples";
    QDir directory(dirname);
    if (directory.isEmpty()) {
        return;
    }

    // create a list with examples
    foreach(QFileInfo filename, directory.entryInfoList()) {
        if (filename.isFile()) {
            auto *item = new QListWidgetItem(filename.fileName());
            ui->listWidget->addItem(item);
        }
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////
// create a new file.
// filename will not be provided - just create a window with a default interface
void mainWindow::on_create_clicked()
{
    // create a new file, so there will be need to SAVE AS
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

// open a canvas with a default template
// when saving, the path will must be specified
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
