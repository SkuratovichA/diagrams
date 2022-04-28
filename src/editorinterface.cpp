#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>


////////////////////////////////
editorInterface::editorInterface(
        QWidget *parent,
        const QString &exampleName,
        newWindowType new_type
) :
        QMainWindow(parent),
        ui(new Ui::editorInterface) {
    (void) exampleName;
    QString text = "";
    switch (new_type) {
        case EXAMPLE_FILE:
            text = "example";
            break;
        case OPEN_FILE:
            this->filename = QFileDialog::getOpenFileName(parent,
                                                          tr("Open a file"),
                                                          QDir::homePath(),
                                                          this->filenameFilter);
            // TODO: add implementation for reading the diagram from the .json file
            assert(!"NOT IMPLEMENTED ERROR");
            break;
        case NO_FILE:
            text = "default text";
            break;
        default: {
            throw "Unrecognized keyword";
        }
    }
    // parse a text, create objects

    // here parser will do the stuff.
    // Then, new window will be opened.
    // Then, an editation will be performed.
    // Then, we all will die :)
    QMessageBox::information(parent, "title", "ebatcaa xo4u");

    ui->setupUi(this);
    this->setWindowTitle("editor");
}

editorInterface::~editorInterface() {
    delete ui;
}


void editorInterface::on_actionSave_triggered() {
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
    in << gaeRepresentation;
}

void editorInterface::on_actionSave_As_triggered() {
    // TODO: chose a file and save it
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(),
                                                    this->filenameFilter);
    if (fileName == nullptr || fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    QTextStream out(&file);

    // TODO write an internal representation to a file
    out << "Hello world";
    QMessageBox::information(this, "TODO", "save as");
}

void editorInterface::on_actionQuit_triggered() {
    //editorInterface::close();
}
////////////////////////////////

////////////////////////////////
// Zooming
//void editorInterface::on_actionZoom_In_triggered()
//{
//    // TODO: zoom in
//   QMessageBox::information(this, "TODO", "zoom in");
//}
//
//
//void editorInterface::on_actionZoom_Out_triggered()
//{
//    // TODO: zoom out
//   QMessageBox::information(this, "TODO", "zoom out");
//}
//
//
//void editorInterface::on_actionReset_Zoom_triggered()
//{
//    // TODO: reset zoom
//   QMessageBox::information(this, "TODO", "reset zoom   ");
//}
////////////////////////////////

////////////////////////////////


//////////////////////////////////
//// Editor
//void editorInterface::on_actionCopy_triggered()
//{
//    // TODO: copy the selected object
//    QMessageBox::information(this, "TODO", "copy");
//}
//
//
//void editorInterface::on_actionPaste_triggered()
//{
//    // TODO: paste the selected object
//    QMessageBox::information(this, "TODO", "paste");
//}
//
//
//void editorInterface::on_actionCut_triggered()
//{
//    // TODO: cut an object
//    QMessageBox::information(this, "TODO", "cut");
//}
//
//
//void editorInterface::on_actionUndo_triggered()
//{
//    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
//    QMessageBox::information(this, "TODO", "undo");
//}
//
//
//void editorInterface::on_actionRedo_triggered()
//{
//    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
//    QMessageBox::information(this, "TODO", "Redo");
//}
////////////////////////////////
