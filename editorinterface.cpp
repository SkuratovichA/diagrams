#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>

editorInterface::editorInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::editorInterface)
{
    ui->setupUi(this);
    this->setWindowTitle("editor");
}

editorInterface::~editorInterface()
{
    delete ui;
}

void editorInterface::on_actionNew_Project_triggered()
{
   QMessageBox::information(this, "TODO", "new project");
}


void editorInterface::on_actionOpen_triggered()
{
   QMessageBox::information(this, "TODO", "open");
}


void editorInterface::on_actionSave_triggered()
{
   QMessageBox::information(this, "TODO", "save");
}


void editorInterface::on_actionAll_triggered()
{
   QMessageBox::information(this, "TODO", "select everything");
}


void editorInterface::on_actionAll_Objects_triggered()
{
   QMessageBox::information(this, "TODO", "select all objects");
}


void editorInterface::on_actionAll_Lines_triggered()
{
   QMessageBox::information(this, "TODO", "select all lilnes");
}


void editorInterface::on_actionZoom_In_triggered()
{
   QMessageBox::information(this, "TODO", "zoom in");
}


void editorInterface::on_actionZoom_Out_triggered()
{
   QMessageBox::information(this, "TODO", "zoom out");
}


void editorInterface::on_actionReset_Zoom_triggered()
{
   QMessageBox::information(this, "TODO", "reset zoom   ");
}


void editorInterface::on_actionNew_Diagram_triggered()
{
   QMessageBox::information(this, "TODO", "new diagram");
}


void editorInterface::on_actionImport_diagram_triggered()
{
   QMessageBox::information(this, "TODO", "import ");
}


void editorInterface::on_actionSave_As_triggered()
{
   QMessageBox::information(this, "TODO", "save as");
}


void editorInterface::on_actionCopy_triggered()
{
   QMessageBox::information(this, "TODO", "copy");
}


void editorInterface::on_actionPaste_triggered()
{
   QMessageBox::information(this, "TODO", "paste");
}


void editorInterface::on_actionCut_triggered()
{
   QMessageBox::information(this, "TODO", "cut");
}


void editorInterface::on_actionUndo_triggered()
{
   QMessageBox::information(this, "TODO", "undo");
}


void editorInterface::on_actionRedo_triggered()
{
   QMessageBox::information(this, "TODO", "Redo");
}

