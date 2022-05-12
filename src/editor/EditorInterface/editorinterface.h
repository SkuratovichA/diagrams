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
    /**
     * @brief Selected option by the user.
     */
    enum newWindowType {
        NO_FILE,     ///< just open a new canvas with default template
        OPEN_FILE,   ///< open a dialogue file window to open a file and edit it
        EXAMPLE_FILE ///< load one of the provided examples to open
    };

    /**
     * @brief A constructor.
     * @details This constructor creates an interface for diagram editing, a toolbar with actions
     * and tabs for each type of diagram.
     * @param parent
     * @param exampleName name of the file with example diagrams
     * @param new_type
     */
    explicit editorInterface(QWidget *parent = nullptr,
                             QString exampleName = "",
                             newWindowType newType = NO_FILE);

    /**
     * A destructor.
     */
    ~editorInterface();
private slots:

    /**
     * @brief A trigger for "Save" action.
     *
     * @details If the file is not specified, the action "Save As" will be triggered.
     * File will be opened as a stream and the text in the required format will be written.
     */
    void actionSaveAs_triggered();

    /**
     * @brief A trigger for "Save As" action.
     * @details File will be opened as a stream and the text in the required format will be written.
     */
    void actionSave_triggered();

    /**
     * @brief A trigger for "Delete Tab" action.
     * Delete a currently active diagram.
     * @note It is not possible to delete a class diagram editor window.
     */
    void actionDeleteTab_triggered();

    /**
     * @brief A trigger for "New Tab" action.
     * Create a new sequence diagram editor tab.
     */
    void actionNewTab_triggered();

    /**
     * @brief A trigger for "Add Entity" action.
     * @details Create a new entity on the active tab.
     * If an active tab contains diagram class, a default class will be created.
     * Otherwise all entities from class diagram will be added to QList.
     * Then the user will be offered the option to create an object
     * from the list in the sequence diagram.
     */
    void actionAddEntity_triggered();

    /**
     * @brief A trigger for "Add Connection" action.
     * @details  Depending on the active diagram a connection
     * between the selected objects will be created.
     */
    void actionAddConnection_triggered();

    /**
     * @brief A trigger for "Remove" action.
     * @details If user is removing a class, he gets a dialog window asking if he wants to remove all other classes.
     * If user is removing anything else, items will just be removed.
     */
    void actionRemove_triggered();

    /**
     * @brief A trigger for "Cut" action.
     * @details Depending on the active diagram all selected
     * entities will be cut to the local buffer.
     * @note Cutting can not be applied to connections.
     */
    void actionCut_triggered();

    /**
     * @brief A trigger for "Copy" action.
     * @details Depending on the active diagram will be copied all selected
     * entities to the local buffer.
     * @note Copying can not be applied to connections.
     */
    void actionCopy_triggered();

    /**
     * @brief A trigger for "Paste" action.
     * Depending on the active diagram will be pasted all selected
     * entities from the local buffer.
     */
    void actionPaste_triggered();

    /**
     * @brief A trigger for "Zoom In" action.
     * Zoom in the current tab 1.2 times.
     */
    void actionZoomIn_triggered();

    /**
     * @brief A trigger for "Zoom out" action.
     * Zoom out the current tab 1.2 times.
     */
    void actionZoomOut_triggered();

    /**
     * @brief A trigger for "Send to back" action.
     * Decrease Z value of the selected items and send them to back.
     */
    void actionBack_triggered();

    /**
     * @brief A trigger for "Send to front" action.
     * Increase Z value of the selected items and send them to front.
     */
    void actionFront_triggered();

    /**
     * @brief Change the active stack in the stackGroup to display ins members (history) for the tab.
     */
    void newTabSelected();
private:

    /**
     * @brief Create a window with a displayed undo stack.
     */
    void createUndoView();

    /**
     * @brief Create 2 default tabs, one for the class diagram and another for the sequence diagram.
     */
    void createTabs();

    /**
     * @brief Create and connect all signals for interaction with tabs.
     */
    void createDynamicToolBar();

    /**
     * @brief Get string representation from diagrams.
     * @param prg Text representation, that will be filled.
     * @return success or error in case of inconsistency
     */
    bool getTextRepresentation(Program &prg);

    /**
     * @brief Load a diagrams from the file.
     */
    void readFile();

    /**
     * @brief Write diagrams to the file.
     */
    void writeToFile();

    /**
     * @brief Create connections.
     */
    void connectItemsDiagrams();

private:
    QTabWidget *tabWidget; ///< tab with diagram editors. The first tab is for classes and cannot be removed
    QWidget *prevWidget = nullptr; ///< previous widget. Used for updating diagrams/names.
    QUndoGroup *undoStack = nullptr; ///< undo group for undo stacks for every tab
    QUndoView *undoView = nullptr; ///< undo view so see undo history

    Ui::editorInterface *ui; ///< editor interface ui
    bool openDiagram; ///< flag specifies if diagram is read from a file
    QString filename = nullptr; ///< filename where to read a diagram from
    QString filenameFilter = "Diagram Files (*.gae)"; ///< filename extension for the project

private:
    QAction *addEntityAction; ///< add an entity
    QAction *addConnectionAction; ///< add a connection
    QAction *deleteAction; ///< delete entity/connection
    QAction *cutAction; ///< cut entity action
    QAction *copyAction; ///< copy entity action
    QAction *pasteAction; ///< paste entity action
    QAction *zoomInAction; ///< zoom in action
    QAction *zoomOutAction; ///< zoom out action
    QAction *bringToFrontAction; ///< bring to front action
    QAction *sendToBackAction; ///< send to back action
    QAction *newTabAction; ///< create new tab action
    QAction *saveAction; ///< save all diagrams
    QAction *saveAsAction; ///< save as (with creating a new file)
    QAction *deleteTabAction; ///< delete a tab (class editor cannot be deleted)
    QAction *undoAction; ///< undo action
    QAction *redoAction; ///< redo action

    QMenu *editMenu; ///< edit menu
    QMenu *actionMenu; ///< action menu
    QMenu *fileMenu; ///< file menu (chose a file to save)
};

#endif // EDITORINTERFACE_H
