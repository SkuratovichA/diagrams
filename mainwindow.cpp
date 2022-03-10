#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <filesystem>

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("diagrams");

    // TODO: generalize
    QString dirname = "/Users/suka/vut/sem4/icp/diagrams/examples";
    QDir directory(dirname);

    foreach(QFileInfo filename, directory.entryInfoList()) {
        if (filename.isFile()) {
            ui->listWidget->addItem(filename.fileName());
        }
    }
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

QString loadFile(QWidget *thisobj, const QString filename)
{
    QFile file(filename);

    QMessageBox::information(thisobj, "title", "file: " + filename + "!");

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        // FIXME: what the fuck? :)
        QMessageBox::warning(thisobj, "title", "file not opened");
        return nullptr;
    } else {
        QMessageBox::information(thisobj, "title", "file opened");
    }
    QTextStream in(&file);
    QString text = in.readAll();

    file.close();
    return text;
}

void mainWindow::on_open_clicked()
{
    QString filename = browseFile(this);
    QString text = loadFile(this, filename);
    if (text == nullptr) {
        QMessageBox::warning(this, "Error", "Cannot read the file" + filename);
    }

    // here parser will do the stuff.
}

