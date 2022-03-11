#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>


////////////////////////////////
editorInterface::editorInterface(
        QWidget *parent,
        QString exampleName,
        newWindowType new_type
        ) :
    QMainWindow(parent),
    ui(new Ui::editorInterface)
{
    (void) exampleName;
    QString text = "";
    if (new_type == EXAMPLE_FILE) {
        text = "example";
    } else if (new_type == OPEN_FILE) {
        this->filename = QFileDialog::getOpenFileName(parent,
                                                      tr("Open a file"),
                                                      QDir::homePath(),
                                                      this->filenameFilter);
        if (this->filename.isEmpty()) {
            throw "Cannot open a file";
        }
        QFile file(this->filename);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(parent, "title", file.errorString());
            throw "Cannot open a file";
        }
        QTextStream in(&file);
        text = in.readAll();
        file.close();
    } else if (new_type == NO_FILE) {
        text = "default text";
    } else {
        throw "Unrecognized keyword";
    }

    // parse a text, craete objects

    // here parser will do the stuff.
    // Then, new window will be opened.
    // Then, an editation will be performed.
    // Then, we all will die :)
    // TODO: implement stuff. Both Save As and Save can be used.
    QMessageBox::information(parent, "title", "text loaded");

    ui->setupUi(this);
    this->setWindowTitle("editor");
}

editorInterface::~editorInterface()
{
    delete ui;
}
////////////////////////////////


////////////////////////////////
// Creating
void editorInterface::on_actionNew_Project_triggered()
{
    // make "unsaved", "saved"
    ui->tabWidget->addTab(new QWidget(), "New Diagram");
}

void editorInterface::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Import a file", QDir::homePath());
    if (filename == nullptr || filename.isEmpty()) {
        return;
    }
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "Error", file.errorString());
        return;
    }

    // TODO: parse a file, create an intermediate representation
    // open a tab
    // import a diagram
    //

    ui->tabWidget->addTab(new QWidget(), filename);
}

////////////////////////////////
//Selection
void editorInterface::on_actionAll_triggered()
{
    // TODO: select all objects and lines
    QMessageBox::information(this, "TODO", "select everything");
}


void editorInterface::on_actionAll_Objects_triggered()
{
    // TODO: select all objects
    QMessageBox::information(this, "TODO", "select all objects");
}


void editorInterface::on_actionAll_Lines_triggered()
{
    // TODO: select all lines
    QMessageBox::information(this, "TODO", "select all lines");
}
////////////////////////////////


////////////////////////////////
// Zooming
void editorInterface::on_actionZoom_In_triggered()
{
    // TODO: zoom in
   QMessageBox::information(this, "TODO", "zoom in");
}


void editorInterface::on_actionZoom_Out_triggered()
{
    // TODO: zoom out
   QMessageBox::information(this, "TODO", "zoom out");
}


void editorInterface::on_actionReset_Zoom_triggered()
{
    // TODO: reset zoom
   QMessageBox::information(this, "TODO", "reset zoom   ");
}
////////////////////////////////


////////////////////////////////
// Manipulate tabs
void editorInterface::on_actionNew_Diagram_triggered()
{
    // TODO: open a new tab with no diagrams
    ui->tabWidget->addTab(new QWidget(), "new diagram");
    //ui->tabWidget->addTab(new QWidget(), "new Diagram");
    QMessageBox::information(this, "TODO", "new diagram");
}

void editorInterface::on_tabWidget_tabCloseRequested(int index)
{
    // if modified, ask for saving
    ui->tabWidget->removeTab(index);
}

void editorInterface::on_actionSave_triggered()
{
    if (this->filename == nullptr || this->filename == "") {
        this->on_actionSave_As_triggered();
        return;
    }

    QFile file(this->filename);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Error", file.errorString());
        return;
    }
    QTextStream in(&file);
    QString gaeRepresentation = "TODO: convert an internal representation into a string\n";
    in <<  gaeRepresentation;
}

void editorInterface::on_actionSave_As_triggered()
{
    // TODO: chose a file and save it
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(), this->filenameFilter);
    if (fileName == nullptr || fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),file.errorString());
        return;
    }
    QTextStream out(&file);

    // TODO write an internal representation to a file
    out << "Hello world";
    QMessageBox::information(this, "TODO", "save as");
}
////////////////////////////////


////////////////////////////////
// Editor
void editorInterface::on_actionCopy_triggered()
{
    // TODO: copy the selected object
    QMessageBox::information(this, "TODO", "copy");
}


void editorInterface::on_actionPaste_triggered()
{
    // TODO: paste the selected object
    QMessageBox::information(this, "TODO", "paste");
}


void editorInterface::on_actionCut_triggered()
{
    // TODO: cut an object
    QMessageBox::information(this, "TODO", "cut");
}


void editorInterface::on_actionUndo_triggered()
{
    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
    QMessageBox::information(this, "TODO", "undo");
}


void editorInterface::on_actionRedo_triggered()
{
    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
    QMessageBox::information(this, "TODO", "Redo");
}
////////////////////////////////

void editorInterface::on_actionQuit_triggered()
{
    //editorInterface::close();
}

