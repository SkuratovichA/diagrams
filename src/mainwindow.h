#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>

#include "editor/editorinterface.h"

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

    // void newFile();

    // void open();
    // void save();
    // void saveAs();
    // void about();
    // void documentWasModified();
// #ifndef QT_NO_SESSIONMANAGER
    // void commitData(QSessionManager &);
// #endif

    void on_pushButton_clicked();

private:
    Ui::mainWindow *ui;
    editorInterface *editor_window;

    // void createActions();
    // void readSettings();
    // void writeSettings();
    // void maybeSave();
    // bool saveFile(const QString &filename);
    // QString srtippedName(const QString &fullFileName);

    QString curFile;
//    QPlainTextEdit *diagramEdit;
};
#endif // MAINWINDOW_H
