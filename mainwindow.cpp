#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("diagrams");
}

mainWindow::~mainWindow()
{
    delete ui;
}


void mainWindow::on_create_clicked()
{
    // FIXME: closing editing window must not close a new window
    editor_window = new editorInterface(this);
    editor_window->show();
}


QString browseFile(QWidget *obj)
{
    QString filename = QFileDialog::getOpenFileName(obj, "Open a file", QDir::homePath());
    QMessageBox::information(obj, "..", filename);
    if (filename == nullptr) {
        return nullptr;
    }
    return filename;
}


void mainWindow::on_open_clicked()
{
    QString filename = browseFile(this);
}

