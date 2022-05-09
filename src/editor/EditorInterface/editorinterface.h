// File: editorinterface.h
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 1.5.2022


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

/*! An editorInterface class */
class editorInterface : public QMainWindow {
Q_OBJECT

public:
    /** An enum type.
     *  Represent a selected option by the user.
     */
    enum newWindowType {
        NO_FILE,     /**< just open a new canvas with default template */

        OPEN_FILE,   /**< open a dialogue file window to open a file and edit it */

        EXAMPLE_FILE /**< load one of the provided examples to open */
    };

    // name of an example
    // Must be provided only if newType is EXAMPLE_FILE
    // default option for window is to create it
    // without a type - just a plain canvas
    explicit editorInterface(QWidget *parent = nullptr,
                             QString exampleName = "",
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
    bool getTextRepresentation(Program &prg);
    void readFile();
    void writeToFile();
    void connectItemsDiagrams();

private:
    QTabWidget *tabWidget;
    QWidget *prevWidget = nullptr;

    QUndoGroup *undoStack = nullptr;
    QUndoView *undoView = nullptr;

    Ui::editorInterface *ui;
    bool openDiagram;
    QString filename = nullptr;
    QString filenameFilter = "Diagram Files (*.gae)";

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
    QAction *newTabAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *deleteTabAction;
    QAction *undoAction;
    QAction *redoAction;

    QMenu *editMenu;
    QMenu *actionMenu;
    QMenu *fileMenu;
};

#endif // EDITORINTERFACE_H
