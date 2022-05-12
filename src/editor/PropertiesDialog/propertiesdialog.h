// File: propertiesdialog.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 7.5.2022


#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include "../Tabs/DiagramItem/DiagramItem.h"

namespace Ui {
    class PropertiesDialog;
}

/**
 * @brief A widget to choose a class from the list. Used while adding a new object on the sequence diagram editor
 */
class PropertiesDialog : public QDialog {
Q_OBJECT

public:

    /**
     * @brief A constructor.
     * @param parent parent window
     * @param existingClasses existing classes to fill out the list
     */
    explicit PropertiesDialog(QWidget *parent, QList<QPair<ClassDiagramItem *, QString>> &existingClasses);

    /**
     * @brief A destructor.
     */
    ~PropertiesDialog();

public:
    /**
     * Getter.
     * @return position of a selected class name.
     */
    int positionOfSelectedClassName() const;
    QString selectedClassName() const;

private slots:
    /**
     * @brief Trigger to handle a selection.
     */
    void on_pushButton_clicked();

private:
    Ui::PropertiesDialog *ui; ///< ui
    QString _selectedClassName = QString(); ///< name of a selected class
    int _positionOfSelectedClassName = -42; ///< position of a selected class.
};

#endif // PROPERTIESDIALOG_H
