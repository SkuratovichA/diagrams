// File: editorinterface.cpp
// Author: Skuratovich ALiaksandr
// Date: 7.5.2022

#include <QListWidgetItem>
#include <QMessageBox>

#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"
#include "../Tabs/DiagramItem/DiagramItem.h"

PropertiesDialog::PropertiesDialog(QWidget *parent, QList<QPair<ClassDiagramItem * , QString>> &existingClasses) :
        QDialog(parent),
        ui(new Ui::PropertiesDialog) {
    ui->setupUi(this);

    for (auto classAddressClassname: existingClasses) {
        auto *item = new QListWidgetItem(classAddressClassname.second);
        ui->listWidget->addItem(item);
    }
}

PropertiesDialog::~PropertiesDialog() {
    delete ui;
}

void PropertiesDialog::on_pushButton_clicked() {
    if (ui->listWidget->count() == 0) {
        QMessageBox::warning(this, "", "No classes in the class diagram tab. Create at least one.");
        goto close; // hehe
    }
    _selectedClassName = ui->listWidget->currentItem()->text();
    _positionOfSelectedClassName = ui->listWidget->currentIndex().row();
    close:
    this->close();
}

QString PropertiesDialog::selectedClassName() const {
    return _selectedClassName;
}

int PropertiesDialog::positionOfSelectedClassName() const {
    return _positionOfSelectedClassName;
}


