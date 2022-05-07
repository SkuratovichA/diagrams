#ifndef SEQUENCECONNECTIONDIALOG_H
#define SEQUENCECONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class SequenceConnectionDialog;
}

class SequenceConnectionDialog : public QDialog {
Q_OBJECT

public:
    enum messageTypes {
        SYNCHRONOUS,
        ASYNCHRONOUS,
        REPLY,
        CREATE,
        DELETE
    };
    explicit SequenceConnectionDialog(QWidget *parent = nullptr);
    ~SequenceConnectionDialog();

public:
    messageTypes messageType();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SequenceConnectionDialog *ui;
    messageTypes index;
};

#endif // SEQUENCECONNECTIONDIALOG_H
