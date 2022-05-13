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

/**
 * Sequence connection dialog to choose one of the given connections
 */
class SequenceConnectionDialog : public QDialog {
Q_OBJECT

public:

    /**
     * @brief A constructor.
     * @param parent parent object
     */
    explicit SequenceConnectionDialog(QWidget *parent = nullptr);

    /**
     * @brief A Destructor
     */
    ~SequenceConnectionDialog();

public:
    /**
     * Type of a chosen message
     * @return ConnectionType
     */
    ConnectionType messageType();

private slots:
    /**
     * Slot activated when pushbutton is clicked
     */
    void on_pushButton_clicked();

private:
    Ui::SequenceConnectionDialog *ui; ///< ui
    ConnectionType index; ///< index of a selected connection casted to the ConnectionType
};

#endif // SEQUENCECONNECTIONDIALOG_H
