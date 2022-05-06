// File: editorinterface.cpp
// Author: Skuratovich ALiaksandr
// Date: 7.5.2022

#include <QListWidgetItem>
#include <QMessageBox>

#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

PropertiesDialog::PropertiesDialog(QWidget *parent, QList<QString> existingClasses) :
        QDialog(parent),
        ui(new Ui::PropertiesDialog) {
    ui->setupUi(this);

    for (auto classname: existingClasses) {
        auto *item = new QListWidgetItem(classname);
        ui->listWidget->addItem(item);
    }
}

PropertiesDialog::~PropertiesDialog() {
    delete ui;
}

void PropertiesDialog::on_pushButton_clicked() {
    if (ui->listWidget->count() == 0) {
        QMessageBox::warning(this, "", "No classes in the class diagram tab. Create at least one.");
        goto close;
    }
    _selectedClassName = ui->listWidget->currentItem()->text();
    close:
    this->close();
}

QString PropertiesDialog::selectedClassName() {
    return _selectedClassName;
}

