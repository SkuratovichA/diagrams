#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
    class PropertiesDialog;
}

class PropertiesDialog : public QDialog {
Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent = nullptr, QList<QString> existingClasses = QList<QString>());
    ~PropertiesDialog();

public:
    QString selectedClassName();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PropertiesDialog *ui;
    QString _selectedClassName = QString();
};

#endif // PROPERTIESDIALOG_H
