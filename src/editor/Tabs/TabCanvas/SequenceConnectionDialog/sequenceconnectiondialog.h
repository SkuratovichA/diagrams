// File: sequenceconnectiondialog.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#ifndef SEQUENCECONNECTIONDIALOG_H
#define SEQUENCECONNECTIONDIALOG_H

#include <QDialog>
#include "../../Connections/Connections.h"

using namespace Connections;

namespace Ui {
    class SequenceConnectionDialog;
}

class SequenceConnectionDialog : public QDialog {
Q_OBJECT

public:

    explicit SequenceConnectionDialog(QWidget *parent = nullptr);
    ~SequenceConnectionDialog();

public:
    ConnectionType messageType();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SequenceConnectionDialog *ui;
    ConnectionType index;
};

#endif // SEQUENCECONNECTIONDIALOG_H
