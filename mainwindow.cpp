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
void mainWindow::on_create_clicked()
{
    // create a new file, so there will be need to SAVE AS
    if (!openEditor()) {
        QMessageBox::warning(this, "title", "file cannot be loaded :(");
        return;
    }
}

// editor is opened with a given filename
// so read a filename, process it and show
bool mainWindow::openEditor(const QString& filename) {
    if (filename == nullptr) {
        return false;
    }
    QFile file(filename);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "title", "file not opened");
        return false;
    } else {
        QMessageBox::information(this, "title", "file opened zaebumba");
    }
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    // here parser will do the stuff.
    // Then, new window will be opened.
    // Then, an editation will be performed.
    // Then, we all will die :)
    // TODO: implement stuff. Both Save As and Save can be used.

    this->editor_window = new editorInterface(this);
    editor_window->show();
    return true;
}

// just open editor without a file, so when saving it
// there will be need to specify the path
bool mainWindow::openEditor()
{
    this->editor_window = new editorInterface(this);
    editor_window->show();
    // TODO: pass a parameter to say a window there will be need to save a file with a path
    // edit and so on. MUST SAVE WITH SAVE AS
    return true;
}

////////////////////////////////////////////////////////////////
// open file from a given list
void mainWindow::on_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(obj, "Open a file", QDir::homePath());
    if (!openEditor(filename)) {
        QMessageBox::warning(this, "title", "SHIT!");
    }
}


// open default template
// when saving, the path will must be specified
void mainWindow::on_pushButton_clicked()
{
    QString filename = ui->listWidget->currentItem()->text();
    QMessageBox::information(this, "filename:", filename);
    // TODO: generalize andrei fix that
    QString directory = "/Users/suka/vut/sem4/icp/diagrams/examples/";
    filename = directory + filename; // it must work

    if (!openEditor(filename)) {
        QMessageBox::warning(this, "title", "cannot open template!");
    }
}

