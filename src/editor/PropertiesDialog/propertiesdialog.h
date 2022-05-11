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

class PropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent, QList<QPair<ClassDiagramItem *, QString>> &existingClasses);
    ~PropertiesDialog();

public:
    int positionOfSelectedClassName() const;
    QString selectedClassName() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::PropertiesDialog *ui;
    QString _selectedClassName = QString();
    int _positionOfSelectedClassName = -42;
};

#endif // PROPERTIESDIALOG_H
