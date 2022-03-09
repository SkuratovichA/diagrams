#include "editorinterface.h"
#include "ui_editorinterface.h"

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
