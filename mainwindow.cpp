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
    return;
}


void mainWindow::on_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
    QMessageBox::information(this, "..", filename);

}

