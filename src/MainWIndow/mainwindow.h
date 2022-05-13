// File: mainwindow.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 24.04.2022

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>

#include "../editor/EditorInterface/editorinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui {class mainWindow;}
QT_END_NAMESPACE

class mainWindow : public QMainWindow {
Q_OBJECT

public:
    /**
     * @brief A Constructor.
     * @param parent A parent object
     */
    mainWindow(QWidget *parent = nullptr);

    /**
     * @brief A destructor.
     */
    ~mainWindow();

private slots:
    /**
     * @brief Create a new file.
     * @details If filename will not be provided - just create a window with a default interface
     */
    void on_create_clicked();

    /**
     * @brief Open a file from a given list
     */
    void on_open_clicked();

    /**
     * @brief Open a canvas with a default template when saving, the path will must be specified
     */
    void on_pushButton_clicked();

private:
    Ui::mainWindow *ui;
    editorInterface *editor_window;

    QString curFile;
};

#endif // MAINWINDOW_H
