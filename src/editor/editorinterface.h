#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H

#include <QMainWindow>
#include "tabs/tabcanvas.h"


namespace Ui {
class editorInterface;
}

class editorInterface : public QMainWindow
{
    Q_OBJECT

public:
    enum newWindowType {
        // just open a new canvas with default template
        NO_FILE,

        // open a dialogue file window to open a file and edit it
        OPEN_FILE,

        // load one of the provided examples to open
        EXAMPLE_FILE
    };

    explicit editorInterface(QWidget *parent = nullptr,
                             // name of an example
                             // Must be provided only if newType is EXAMPLE_FILE
                             const QString& exampleName = nullptr,

                             // default option for window is to create it
                             // without a type - just a plain canvas
                             newWindowType newType = NO_FILE
                             );

    ~editorInterface();

    //void loadFile(const QString &filename);

private slots:
    void actionZoom_In_triggered();
    void actionZoom_Out_triggered();
    void actionReset_Zoom_triggered();

    void actionCopy_triggered();
    void actionPaste_triggered();
    void actionCut_triggered();

    void actionUndo_triggered();
    void actionRedo_triggered();
    void actionSave_triggered();

    void actionDeleteTab_triggered();
    void actionNewTab_triggered();

    void actionSave_As_triggered();
    void actionQuit_triggered();
    void tabSelected();

private:
    void createTabs();
    void createToolBars();
    void createActions();
    void disconnectSlots(TabCanvas *prevTab);
    void createStaticToolBar();

private:
    QToolBar *editToolBar;
    QTabWidget *tabWidget;

    Ui::editorInterface *ui;
    QString filename = "";
    QString filenameFilter = "Diagram Files (*.gae)";


    /**
     * actions
     */
private:
    QAction *addEntityAction;
    QAction *addConnectionAction;
    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *propertiesAction;
    QAction *sendToBackAction;

    QAction *newTabAction;
    QAction *deleteTabAction;
    QAction *saveAction;
    QAction *undoAction;
    QAction *redoAction;
};

#endif // EDITORINTERFACE_H
