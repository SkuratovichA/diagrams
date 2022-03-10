#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <filesystem>


////******************************************************************
//// create a separate file/class for working with files. Many funstions will need to read/modify/write/save files.
//QString loadFile(QWidget *thisobj, const QString filename)
//{
//    if (filename == nullptr) {
//        return nullptr;
//    }
//    QFile file(filename);

//    QMessageBox::information(thisobj, "title", "file: " + filename + "!");

//    if (file.open(QFile::ReadOnly)) {
//        // FIXME: what the fuck? :)
//        QMessageBox::warning(thisobj, "title", "file not opened");
//        return nullptr;
//    } else {
//        QMessageBox::information(thisobj, "title", "file opened");
//    }
//    QTextStream in(&file);
//    QString text = in.readAll();

//    file.close();
//    return text;
//}

QString browseFile(QWidget *obj)
{
    QString filename = QFileDialog::getOpenFileName(obj, "Open a file", QDir::homePath());
    QMessageBox::information(obj, "..", filename);
    if (filename == nullptr) {
        return nullptr;
    }
    return filename;
}

//******************************************************************


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
            QListWidgetItem *item = new QListWidgetItem(filename.fileName());
            ui->listWidget->addItem(item);
        }
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////
void mainWindow::on_create_clicked()
{
    // create a new file, so there will be need to SAVE AS
    if (openEditor() == false) {
        QMessageBox::warning(this, "title", "file cannot be loaded :(");
        return;
    }
}

// editor is opened with a given filename
// so read a filename, process it and show
bool mainWindow::openEditor(QString filename) {
    if (filename == nullptr) {
        return false;
    }
    QFile file(filename);

    QMessageBox::information(this, "title", "file: " + filename + "!");

    if (file.open(QFile::ReadOnly)) {
        // FIXME: what the fuck? :)
        QMessageBox::warning(this, "title", "file not opened");
        return false;
    } else {
        QMessageBox::information(this, "title", "file opened");
    }
    QTextStream in(&file);
    QString text = in.readAll();

    file.close();

    // here parser will do the stuff.
    // Then, new window will be opened.
    // Then, an editation will be performed.
    // Then, we all will die :)
    this->editor_window = new editorInterface(this);

    // TODO: pass a parameter to say a window there will be need to save a file with a path
    editor_window->show();
    return true;
}

// just open editor without a file, so when saving it
// there will be need to specify the path
bool mainWindow::openEditor()
{
    this->editor_window = new editorInterface(this);
    editor_window->show();

    // TODO: edit and so on. MUST SAVE WITH SAVE AS
    return true;
}

////////////////////////////////////////////////////////////////
// open file from a given list
void mainWindow::on_open_clicked()
{
    QString filename = browseFile(this);
    if (openEditor(filename) == false) {
        QMessageBox::warning(this, "title", "SHIT!");
    }
    //QString text = loadFile(this, filename);
    // hide();
}


// open default template
// when saving, the pathh will must be specified
void mainWindow::on_pushButton_clicked()
{
    QString filename = ui->listWidget->currentItem()->text();
    QMessageBox::information(this, "filename:", filename);
    // TODO: generalize
    QString directory = "/Users/suka/vut/sem4/icp/diagrams/examples/";
    filename = directory + filename; // it must work
    QMessageBox::information(this, "filename:", filename);

    if (openEditor(filename) == false) {
        QMessageBox::warning(this, "title", "FUCK!");
    }
}

