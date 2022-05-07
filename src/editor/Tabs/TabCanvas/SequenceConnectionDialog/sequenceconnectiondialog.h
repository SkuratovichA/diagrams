#ifndef SEQUENCECONNECTIONDIALOG_H
#define SEQUENCECONNECTIONDIALOG_H

#include <QDialog>
#include "../../DiagramItem/Connections/Connections.h"

namespace Ui {
    class SequenceConnectionDialog;
}

class SequenceConnectionDialog : public QDialog {
Q_OBJECT

public:

    explicit SequenceConnectionDialog(QWidget *parent = nullptr);
    ~SequenceConnectionDialog();

public:
    SequenceConnectionItem::SequenceConnectionType messageType();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SequenceConnectionDialog *ui;
    SequenceConnectionItem::SequenceConnectionType index;
};

#endif // SEQUENCECONNECTIONDIALOG_H
