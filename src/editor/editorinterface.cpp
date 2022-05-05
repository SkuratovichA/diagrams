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

#include "tabs/tabcanvas.h"

#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                          \
        obj = new QAction(tr((name)), this);                      \
        /*obj->setIcon(icon);*/                                   \
        (obj)->setShortcut(tr(shortcut));                         \
        connect((obj), SIGNAL(triggered()), receiver, memberslot);\
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

    (void) exampleName;
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
    //            // TODO: add implementation for reading the diagram from the .json file
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
    //createStaticToolBar();

//    qDebug() << "before creating tabs";
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
    undoStack->setActiveStack(reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->getUndoStack());
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
    tabWidget->addTab(new TabCanvas(this, DiagramType::CLASS, undoStack), "class diagram editor");
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE, undoStack), "sequence diagram editor");
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

    //ADD_SIGNAL(sendToBackAction, "Send to back", "b", "Ctrl+""B",   this, SLOT());
    //ADD_SIGNAL(bringToFrontAction, "Send to front", "f", "Ctrl+""F",this, SLOT());
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
QString editorInterface::get_text_representation() {
    auto size = tabWidget->count();
    std::string prg;
    for (int i = 0; i < size; i++) {
        prg += reinterpret_cast<TabCanvas *>(tabWidget->widget(i))->getStringRepresentation();
    }
    return {prg.c_str()};
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
    auto prg = get_text_representation();
    qDebug() << prg;
    in << prg;
    file.close();
}

/**
 *
 */
void editorInterface::actionSaveAs_triggered() {
    filename = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(),
                                            filenameFilter);
    if (filename == nullptr || filename.isEmpty()) {
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    QTextStream in(&file);
    auto prg = get_text_representation();
    qDebug() << prg;
    in << prg;
    file.close();
}

//void editorInterface::actionQuit_triggered() {
//    editorInterface::close();
//}

void editorInterface::actionNewTab_triggered() {
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE, undoStack), "sequence diagram editor");
}

void editorInterface::actionDeleteTab_triggered() {
    auto ci = tabWidget->currentIndex();
    if (ci == 0) {
        QMessageBox::warning(this, "Warning", "You cannot close the class diagram");
        return;
    }
    tabWidget->removeTab(ci);
}

/**
 *
 */
void editorInterface::actionAddEntity_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->addEntity();
}

/**
 *
 */
void editorInterface::actionAddConnection_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->addConnection();
}

/**
 *
 */
void editorInterface::actionRemove_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->removeEntity();
}

/**
 *
 */
void editorInterface::actionCut_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->cut();
}

/**
 *
 */
void editorInterface::actionCopy_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->copy();
}

/**
 *
 */
void editorInterface::actionPaste_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->paste();
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
