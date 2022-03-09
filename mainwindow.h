#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editorinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void on_create_clicked();

    void on_open_clicked();

private:
    Ui::mainWindow *ui;
    editorInterface *editor_window;
};
#endif // MAINWINDOW_H
