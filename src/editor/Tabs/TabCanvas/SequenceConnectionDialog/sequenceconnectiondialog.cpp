#include "sequenceconnectiondialog.h"
#include "ui_sequenceconnectiondialog.h"

#include <QDebug>

using namespace Connections;

SequenceConnectionDialog::SequenceConnectionDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SequenceConnectionDialog) {
    ui->setupUi(this);
    ui->listWidget->addItem("synchronos message");
    ui->listWidget->addItem("asynchronos message");
    ui->listWidget->addItem("reply message");
    ui->listWidget->addItem("create message");
    ui->listWidget->addItem("delete message");
}

SequenceConnectionDialog::~SequenceConnectionDialog() {
    delete ui;
}

ConnectionType SequenceConnectionDialog::messageType() {
    return index;
}

void SequenceConnectionDialog::on_pushButton_clicked() {
    index = static_cast<ConnectionType>(ui->listWidget->currentRow());
    qDebug() << index << " index!!!!!!!! (current row)";
    close();
}