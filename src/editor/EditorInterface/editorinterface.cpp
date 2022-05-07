// File: editorinterface.cpp
// Author: Skuratovich ALiaksandr
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

/**
 *
 * @param parent
 * @param exampleName
 * @param new_type
 */
editorInterface::editorInterface(
        QWidget *parent,
        const QString &exampleName,
        newWindowType new_type
) :
        QMainWindow(parent),
        ui(new Ui::editorInterface) {
    setAttribute(Qt::WA_DeleteOnClose); // automatically delete itself when window is closed

    exampleName;
#if 0
    //    QString text = "";
    //
    //    switch (new_type) {
    //        case EXAMPLE_FILE:
    //            text = "example";
    //            break;
    //        case OPEN_FILE:
    //            this->filename = QFileDialog::getOpenFileName(parent,
    //                                                          tr("Open a file"),
    //                                                          QDir::homePath(),
    //                                                          this->filenameFilter);
    //            // TODO: add implementation for reading the Diagram from the .json file
    //            assert(!"NOT IMPLEMENTED ERROR");
    //            break;
    //        case NO_FILE:
    //            text = "default text";
    //            break;
    //        default: {
    //            throw "Unrecognized keyword";
    //        }
    //    }
#endif
    qDebug() << "create editorinterface: constructor editorInterface";

    ui->setupUi(this);
    qDebug() << "create undo stack: constructor editorInterface";
    undoStack = new QUndoGroup(this);

    this->setWindowTitle("editor");

    createUndoView();
    createDynamicToolBar();

    createTabs();
    setCentralWidget(tabWidget);
}

/**
 *
 */
editorInterface::~editorInterface() {
    qDebug() << "delete QUndoView undowView : destructor editorInterface";
    delete undoView;
    qDebug() << "delete ui : destructor editorInterface";
    delete ui;
}

/**
 * Change the active stack in the stackGroup to display ins members (history) for the tab.
 */
void editorInterface::newTabSelected() {
    undoStack->setActiveStack(reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->undoStack());
}

/**
 *
 */
void editorInterface::createUndoView() {
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

/**
 *
 */
void editorInterface::createTabs() {
    tabWidget = new QTabWidget(this);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(newTabSelected()));
    tabWidget->addTab(new ClassCanvas(this, undoStack), "class Diagram");
    tabWidget->addTab(new SequenceCanvas(this, undoStack), "sequence Diagram");
}

/**
 *
 */
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
    ADD_SIGNAL(zoomInAction, "Zoom &In", "Ctrl+", "Ctrl+", this, SLOT(actionZoomIn_triggered()));
    ADD_SIGNAL(zoomOutAction, "Zoom &Out", "Ctrl-", "Ctrl-", this, SLOT(actionZoomOut_triggered()));
    ADD_SIGNAL(sendToBackAction, "Send to back", "b", "Ctrl+B", this, SLOT(actionBack_triggered()));
    ADD_SIGNAL(bringToFrontAction, "Send to front", "f", "Ctrl+F", this, SLOT(actionFront_triggered()));
    qDebug() << "dynamic toolbar added";

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));

#ifdef __APPLE__
    QToolBar *a = addToolBar(tr("Edit1"));
    addToolBarBreak(Qt::TopToolBarArea);
    QToolBar *b = addToolBar(tr("Edit2"));
    a->addAction(saveAction);
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

/**
 *
 * @return
 */
void editorInterface::get_text_representation() {
    auto size = tabWidget->count();
    qDebug() << size;
    for (int i = 0; i < size; i++) {
        reinterpret_cast<TabCanvas *>(tabWidget->widget(i))->getStringRepresentation(prg);
    }
}

/**
 *
 */
void editorInterface::actionSave_triggered() {
    if (filename == nullptr || filename == "") {
        actionSaveAs_triggered();
        return;
    }

    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);
    get_text_representation();
    //prg.fill_file();

    //qDebug() << prg;
    //in << prg;
    file.close();
}

/**
 *
 */
void editorInterface::actionSaveAs_triggered() {
    filename = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(),
                                            filenameFilter);

    qDebug() << filename << "save here";
    if (filename == nullptr || filename.isEmpty()) {
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    QTextStream in(&file);
    get_text_representation();
    //prg.fill_file();

    //qDebug() << prg;
    //in << prg;
    file.close();
}

//void editorInterface::actionQuit_triggered() {
//    editorInterface::close();
//}

void editorInterface::actionNewTab_triggered() {
    tabWidget->addTab(new SequenceCanvas(this, undoStack), "sequence Diagram editor");
}

void editorInterface::actionDeleteTab_triggered() {
    auto ci = tabWidget->currentIndex();
    if (ci == 0) {
        QMessageBox::warning(this, "Warning", "You cannot close the class Diagram");
        return;
    }
    tabWidget->removeTab(ci);
}

/**
 *
 */
void editorInterface::actionAddEntity_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->addEntity();
    } else {
        auto sequenceTab = reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget());
        // all strings with class names to put in na widget
        QList<QString> existingClassesNames;

        // all clasess, to connect with a seuqence item
        auto classStringPairs = reinterpret_cast<ClassCanvas *>(tabWidget->widget(0))->getClassStringPairs();

        for (auto cs : classStringPairs) {
            existingClassesNames.append(cs.second);
        }

        PropertiesDialog propertiesDialog(this, existingClassesNames);
        propertiesDialog.exec();

        auto selectedClassName = propertiesDialog.selectedClassName();
        int positionOfSelectedClassName = propertiesDialog.positionOfSelectedClassName();

        // empty
        if (positionOfSelectedClassName == -42) {
            return;
        }

        sequenceTab->addEntity(classStringPairs.at(positionOfSelectedClassName).first);
    }
}

/**
 *
 */
void editorInterface::actionAddConnection_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->addConnection();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->addConnection();
    }
}

/**
 *
 */
void editorInterface::actionRemove_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->removeEntity();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->removeEntity();
    }
}

/**
 *
 */
void editorInterface::actionCut_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->cut();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->cut();
    }
}

/**
 *
 */
void editorInterface::actionCopy_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->copy();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->copy();
    }
}

/**
 *
 */
void editorInterface::actionPaste_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->paste();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->paste();
    }
}

/**
 *
 */
void editorInterface::actionZoomIn_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->zoomIn();
}

/**
 *
 */
void editorInterface::actionZoomOut_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->zoomOut();
}

/**
 *
 */
void editorInterface::actionBack_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->toBack();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->toBack();
    }
}

/**
 *
 */
void editorInterface::actionFront_triggered() {
    if (tabWidget->currentIndex() == 0) {
        reinterpret_cast<ClassCanvas *>(tabWidget->currentWidget())->toFront();
    } else {
        reinterpret_cast<SequenceCanvas *>(tabWidget->currentWidget())->toFront();
    }
}
