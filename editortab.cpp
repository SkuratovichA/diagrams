#include "editortab.h"
#include "ui_editortab.h"

editorTab::editorTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editorTab)
{
    ui->setupUi(this);
}

editorTab::~editorTab()
{
    delete ui;
}
