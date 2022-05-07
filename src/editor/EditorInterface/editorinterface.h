#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H

#include <QMainWindow>
#include <QUndoGroup>
#include <QUndoView>
#include "../Tabs/TabCanvas/TabCanvas.h"
#include "../Tabs/DiagramItem/DiagramItem.h"
#include "../PropertiesDialog/propertiesdialog.h"
#include "../Backend/Parse.h"

namespace Ui {
    class editorInterface;
}

class editorInterface : public QMainWindow {
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

    // name of an example
    // Must be provided only if newType is EXAMPLE_FILE
    // default option for window is to create it
    // without a type - just a plain canvas
    explicit editorInterface(QWidget *parent = nullptr,
                             const QString &exampleName = nullptr,
                             newWindowType newType = NO_FILE);

    ~editorInterface();

    //void loadFile(const QString &filename);

private slots:

    void actionSaveAs_triggered();

    void actionSave_triggered();

    void actionDeleteTab_triggered();

    void actionNewTab_triggered();

    void actionAddEntity_triggered();

    void actionAddConnection_triggered();

    void actionRemove_triggered();

    void actionCut_triggered();

    void actionCopy_triggered();

    void actionPaste_triggered();

    void actionZoomIn_triggered();

    void actionZoomOut_triggered();

    void actionBack_triggered();

    void actionFront_triggered();

    //void actionQuit_triggered();

    void newTabSelected();

private:
    void createUndoView();

    void createTabs();

    void createDynamicToolBar();

    void get_text_representation();

private:
    QTabWidget *tabWidget;
    Program prg;

    QUndoGroup *undoStack = nullptr;
    QUndoView *undoView = nullptr;

    Ui::editorInterface *ui;
    QString filename = nullptr;
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
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;

    QMenu *editMenu;
    QMenu *actionMenu;
    QMenu *fileMenu;

    QAction *newTabAction;
    QAction *saveAction;
    QAction *deleteTabAction;
    QAction *undoAction;
    QAction *redoAction;
};

#endif // EDITORINTERFACE_H