// File: editorinterface.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 1.5.2022

// FIXME: https://doc.qt.io/qt-5/qkeysequence.html

#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QUndoView>

#include "../Tabs/TabCanvas/TabCanvas.h"
#include "../PropertiesDialog/propertiesdialog.h"

#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                            \
        obj = new QAction(tr((name)), this);                        \
        /*obj->setIcon(icon);*/                                     \
        (obj)->setShortcut(tr(shortcut));                           \
        connect((obj), SIGNAL(triggered()), receiver, memberslot);  \
    } while(0)

editorInterface::editorInterface(
        QWidget *parent,
        QString exampleName,
        newWindowType new_type
) :
        QMainWindow(parent),
        ui(new Ui::editorInterface) {
    setAttribute(Qt::WA_DeleteOnClose); // automatically delete itself when window is closed

    openDiagram = false;

    switch (new_type) {
        case EXAMPLE_FILE:
            filename = exampleName;
            openDiagram = true;
            break;
        case OPEN_FILE:
            filename = QFileDialog::getOpenFileName(parent,
                               tr("Open a file"),
                                  QDir::homePath(),
                                 filenameFilter);
            openDiagram = true;
            break;
        case NO_FILE:
            break;
        default: {
            throw "Unrecognized keyword";
        }
    }

    ui->setupUi(this);
    undoStack = new QUndoGroup(this);

    setWindowTitle("editor");

    createUndoView();
    createDynamicToolBar();

    if (openDiagram) {
        readFile();
        return;
    }

    createTabs();
    setCentralWidget(tabWidget);
}

editorInterface::~editorInterface() {
    delete undoView;
    delete ui;
}

void editorInterface::newTabSelected() {

    //static QWidget *prevWidget = nullptr;
    qDebug() << "wee wee (1)";
    if (prevWidget != nullptr) {
        // moved from the sequence canvas, hence, there is a need to update all class connecitons
        auto previousSequenceCanvas = dynamic_cast<SequenceCanvas *>(prevWidget);
        if (previousSequenceCanvas != nullptr) {
            for (SequenceDiagramItem *sequenceItem: previousSequenceCanvas->getItems<SequenceDiagramItem>()) {
                auto newName = sequenceItem->name();
                if (newName != sequenceItem->parentClassDiagramItem()->name()) {
                    sequenceItem->parentClassDiagramItem()->setName(newName);
                    qreal Pos = (sequenceItem->parentClassDiagramItem()->boundingRect().width() - sequenceItem->parentClassDiagramItem()->width()) / 2;
                    sequenceItem->parentClassDiagramItem()->setPos(Pos, -40);

                }
            }
        }
        // moved from something (does not matter) to the sequence canvas. There is a need to update a sequence canvas
        auto destSequenceCanvas = dynamic_cast<SequenceCanvas *>(tabWidget->currentWidget());
        if (destSequenceCanvas != nullptr) {
            for (SequenceDiagramItem *sequenceItem: destSequenceCanvas->getItems<SequenceDiagramItem>()) {
                auto newName = sequenceItem->parentClassDiagramItem()->name();
                if (newName != sequenceItem->name()) {
                    sequenceItem->setName(newName);
                    qreal Pos = (sequenceItem->parentClassDiagramItem()->boundingRect().width() - sequenceItem->parentClassDiagramItem()->width()) / 2;
                    sequenceItem->parentClassDiagramItem()->setPos(Pos, -40);
                }
            }
        }
    }

    undoStack->setActiveStack(reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->undoStack());
    prevWidget = tabWidget->currentWidget();
    // update of the scene does not work
    dynamic_cast<TabCanvas *>(tabWidget->currentWidget())->updateScene();
}

void editorInterface::createUndoView() {
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void editorInterface::createTabs() {
    tabWidget = new QTabWidget(this);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(newTabSelected()));
    tabWidget->addTab(new ClassCanvas(this, undoStack), "class Diagram");
    tabWidget->addTab(new SequenceCanvas(this, undoStack), "sequence Diagram");
}

void editorInterface::createDynamicToolBar() {
    ADD_SIGNAL(addEntityAction, "New &Entity", "+", "Ctrl+N", this, SLOT(actionAddEntity_triggered()));
    ADD_SIGNAL(addConnectionAction, "Connect", "->", "Ctrl+L", this, SLOT(actionAddConnection_triggered()));
    ADD_SIGNAL(deleteAction, "Delete", "-", "Ctrl+D", this, SLOT(actionRemove_triggered()));
    ADD_SIGNAL(cutAction, "Cut", "x", "Ctrl+X", this, SLOT(actionCut_triggered()));
    ADD_SIGNAL(copyAction, "Copy", "c", "Ctrl+C", this, SLOT(actionCopy_triggered()));
    ADD_SIGNAL(pasteAction, "Paste", "v", "Ctrl+V", this, SLOT(actionPaste_triggered()));
    ADD_SIGNAL(newTabAction, "New &Tab", "+T", "Ctrl+T", this, SLOT(actionNewTab_triggered()));
    ADD_SIGNAL(deleteTabAction, "Delete &Tab", "+T", "Ctrl+W", this, SLOT(actionDeleteTab_triggered()));
    ADD_SIGNAL(saveAction, "&Save", "S", "Ctrl+S", this, SLOT(actionSave_triggered()));
    ADD_SIGNAL(saveAsAction, "Save &As", "S", "Ctrl+Shift+S", this, SLOT(actionSaveAs_triggered()));
    ADD_SIGNAL(zoomInAction, "Zoom &In", "Ctrl+", "Ctrl+Plus", this, SLOT(actionZoomIn_triggered()));
    ADD_SIGNAL(zoomOutAction, "Zoom &Out", "Ctrl-", "Ctrl+Minus", this, SLOT(actionZoomOut_triggered()));
    ADD_SIGNAL(sendToBackAction, "Send to back", "b", "Ctrl+B", this, SLOT(actionBack_triggered()));
    ADD_SIGNAL(bringToFrontAction, "Send to front", "f", "Ctrl+F", this, SLOT(actionFront_triggered()));

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));

#ifdef __APPLE__
    QToolBar *a = addToolBar(tr("Edit1"));
    addToolBarBreak(Qt::TopToolBarArea);
    QToolBar *b = addToolBar(tr("Edit2"));
    a->addAction(saveAction);
    a->addAction(saveAsAction);
    a->addAction(cutAction);
    a->addAction(pasteAction);
    a->addAction(copyAction);
    a->addAction(undoAction);
    a->addAction(redoAction);
    b->addAction(addEntityAction);
    b->addAction(addConnectionAction);
    b->addAction(deleteAction);
    b->addAction(newTabAction);
    b->addAction(deleteTabAction);

    b->addAction(sendToBackAction);
    b->addAction(bringToFrontAction);

    a->addAction(zoomInAction);
    a->addAction(zoomOutAction);

    a->setOrientation(Qt::Horizontal);
    b->setOrientation(Qt::Horizontal);
#else
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    actionMenu = menuBar()->addMenu(tr("&Action"));
    actionMenu->addAction(addEntityAction);
    actionMenu->addAction(addConnectionAction);
    actionMenu->addAction(deleteAction);
    actionMenu->addAction(sendToBackAction);
    actionMenu->addAction(bringToFrontAction);

    menuBar()->addAction(newTabAction);
    menuBar()->addAction(deleteTabAction);
    menuBar()->addAction(zoomInAction);
    menuBar()->addAction(zoomOutAction);
#endif
}

bool editorInterface::getTextRepresentation(Program &prg) {
    auto size = tabWidget->count();
    for (int i = 0; i < size; i++) {
        tabWidget->setCurrentWidget(tabWidget->widget(i));
        if (! reinterpret_cast<TabCanvas *>(tabWidget->widget(i))->getStringRepresentation(prg) ) {
            return false;
        }
    }

    tabWidget->setCurrentWidget(tabWidget->widget(0));

    return true;
}

void editorInterface::connectItemsDiagrams() {
    auto sequenceTab = reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget());
    QList<QPair<ClassDiagramItem *, QString>> classStringPairs = reinterpret_cast<ClassCanvas *>(tabWidget->widget(
            0))->getClassStringPairs();

    for (auto x: sequenceTab->getItems<SequenceDiagramItem>()) {
        for (auto y : classStringPairs) {
            if (x->name() == y.first->name()) {
                x->setParent(y.first);
            }
        }
    }
}

void editorInterface::readFile() {
    int idx;
    tabWidget = new QTabWidget(this);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(newTabSelected()));
    setCentralWidget(tabWidget);

    Program prg;
    prg.parseFile(filename.toStdString());
    //ItemsBuffer arrBufs[prg.diagramSequence.size() + 1];

    // class diagram
    idx = tabWidget->addTab(new ClassCanvas(this, undoStack), "class Diagram");
    reinterpret_cast<ClassCanvas *>(tabWidget->widget(idx))->createFromFile(prg.diagramClass);

    // sequence diagram
    for (int c = 0; c < prg.diagramSequence.size(); c++) {
        idx = tabWidget->addTab(new SequenceCanvas(this, undoStack), "sequence Diagram");
        tabWidget->setCurrentWidget(tabWidget->widget(idx));
        reinterpret_cast<SequenceCanvas *>(tabWidget->widget(idx))->createFromFile(prg.diagramSequence[c]);
        connectItemsDiagrams();
    }

    tabWidget->setCurrentWidget(tabWidget->widget(0));
}

void editorInterface::writeToFile() {
    Program prg;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    if ( !getTextRepresentation(prg) ) {
        return;
    }

    prg.fillFile(filename.toStdString());
    file.close();
}

/**
 * A trigger for "Save" action.
 *
 * If the file is not specified, the action "Save As" will be triggered.
 * File will be opened as a stream and the text in the required format will be written.
 */
void editorInterface::actionSave_triggered() {
    if (filename == nullptr || filename.isEmpty()) {
        actionSaveAs_triggered();
        return;
    }

    writeToFile();
}

/**
 * A trigger for "Save As" action.
 *
 * File will be opened as a stream and the text in the required format will be written.
 */
void editorInterface::actionSaveAs_triggered() {
    filename = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(),
                                            filenameFilter);

    if (filename == nullptr || filename.isEmpty()) {
        return;
    }

    writeToFile();
}

/**
 * A trigger for "New Tab" action.
 *
 * Create a new tab for sequence diagram.
 */
void editorInterface::actionNewTab_triggered() {
    tabWidget->addTab(new SequenceCanvas(this, undoStack), "sequence Diagram editor");
}

/**
 * A trigger for "Delete Tab" action.
 *
 * Delete the currently active diagram.
 * @note You can not delete a class diagram.
 */
void editorInterface::actionDeleteTab_triggered() {
    auto ci = tabWidget->currentIndex();
    if (ci == 0) {
        QMessageBox::warning(this, "Warning", "You cannot close the class Diagram");
        return;
    }
    tabWidget->removeTab(ci);
}

/**
 * A trigger for "Add Entity" action.
 *
 * @details Create a new entity on the active tab.
 * If an active tab contains diagram class, will be created a default class.
 * Otherwise all entities from class diagram will be read to QList.
 * Then the user will be offered the option to create an object
 * from the list in the sequence diagram.
 */
void editorInterface::actionAddEntity_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->addEntity();
    } else {
        auto sequenceTab = reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget());

        // all clases from the class tab
        QList<QPair<ClassDiagramItem *, QString>> classStringPairs = reinterpret_cast<ClassCanvas *>(tabWidget->widget(
                0))->getClassStringPairs();

        QList<QPair<ClassDiagramItem *, QString>> sequenceClassStringPairs;
        for (auto actor: sequenceTab->getItems<SequenceDiagramItem>()) {
            sequenceClassStringPairs.push_back(
                    QPair<ClassDiagramItem *, QString>(
                            actor->parentClassDiagramItem(),
                            actor->parentClassDiagramItem()->name())
            );
        }
        for (auto x: classStringPairs) {
            if (sequenceClassStringPairs.contains(x)) {
                classStringPairs.removeOne(x);
            }
        }

        PropertiesDialog propertiesDialog(this, classStringPairs);
        propertiesDialog.exec();
        auto selectedClassName = propertiesDialog.selectedClassName();
        int positionOfSelectedClassName = propertiesDialog.positionOfSelectedClassName();

        if (positionOfSelectedClassName == -42) {
            return;
        }

        sequenceTab->addEntity(classStringPairs.at(positionOfSelectedClassName).first);
    }
}

/**
 * A trigger for "Add Connection" action.
 *
 * Depending on the active diagram will be created connection
 * between the selected objects.
 */
void editorInterface::actionAddConnection_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->addConnection();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->addConnection();
    }
}

/**
 * A trigger for "Remove" action.
 *
 * Depending on the active diagram will be removed all selected entities.
 */
void editorInterface::actionRemove_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->removeEntity();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->removeEntity();
    }
}

/**
 * A trigger for "Cut" action.
 *
 * Depending on the active diagram will be cut all selected
 * entities to the local buffer.
 * @note Cutting can not be applied to connections.
 */
void editorInterface::actionCut_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->cut();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->cut();
    }
}

/**
 * A trigger for "Copy" action.
 *
 * Depending on the active diagram will be copied all selected
 * entities to the local buffer.
 * @note Copying can not be applied to connections.
 */
void editorInterface::actionCopy_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->copy();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->copy();
    }
}

/**
 * A trigger for "Paste" action.
 *
 * Depending on the active diagram will be pasted all selected
 * entities from the local buffer.
 */
void editorInterface::actionPaste_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->paste();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->paste();
    }
}

/**
 * A trigger for "Zoom In" action.
 *
 * Zoom in the current tab 1.2 times.
 */
void editorInterface::actionZoomIn_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->zoomIn();
}

/**
 * A trigger for "Zoom out" action.
 *
 * Zoom out the current tab 1.2 times.
 */
void editorInterface::actionZoomOut_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->zoomOut();
}

/**
 * A trigger for "Send to back" action.
 *
 * Decrease Z value of the selected items and send them to back.
 */
void editorInterface::actionBack_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->toBack();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->toBack();
    }
}

/**
 * A trigger for "Send to front" action.
 *
 * Increase Z value of the selected items and send them to front.
 */
void editorInterface::actionFront_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->toFront();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->toFront();
    }
}
