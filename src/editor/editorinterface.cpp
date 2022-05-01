#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>

#include "tabs/tabcanvas.h"

#define ADD_SIGNAL(obj, name, icon, shortcut, receiver, memberslot) \
    do {                                                  \
        obj = new QAction(tr((name)), this);                          \
        /*obj->setIcon(icon);*/                                       \
        (obj)->setShortcut(tr(shortcut));                             \
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

    ui->setupUi(this);
    this->setWindowTitle("editor");

    createTabs();

    createToolBars();

    createStaticToolBar();
}

/**
 *
 */
editorInterface::~editorInterface() {
    delete ui;
}


/**
 *
 */
void editorInterface::createTabs() {
    tabWidget = new QTabWidget(this);
    this->setCentralWidget(tabWidget);
    tabWidget->addTab(new TabCanvas(this, DiagramType::CLASS), "class diagram editor");
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");
}

/**
 *
 */
void editorInterface::createToolBars() {

    ADD_SIGNAL(addEntityAction, "New &Entity", "+", "Ctrl+N", this, SLOT(actionAddEntity_triggered()));
    ADD_SIGNAL(addConnectionAction, "Connect", "->", "Ctrl+L", this, SLOT(actionAddConnection_triggered()));
    ADD_SIGNAL(deleteAction, "Delete", "-", "Ctrl+D", this, SLOT(actionRemove_triggered()));
    ADD_SIGNAL(cutAction, "Cut", "x", "Ctrl+X", this, SLOT(actionCut_triggered()));
    ADD_SIGNAL(copyAction, "Copy", "c", "Ctrl+C", this, SLOT(actionCopy_triggered()));
    ADD_SIGNAL(pasteAction, "Paste", "v", "Ctrl+V", this, SLOT(actionPaste_triggered()));
    ADD_SIGNAL(propertiesAction, "Properties", "i", "Ctrl+I", this, SLOT(actionProperties_triggered()));
//    ADD_SIGNAL(sendToBackAction, "Send to back", "b", "Ctrl+""B",   this, SLOT());
//    ADD_SIGNAL(bringToFrontAction, "Send to front", "f", "Ctrl+""F",this, SLOT());

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setFloatable(false);

    editToolBar->addAction(addEntityAction);
    editToolBar->addAction(addConnectionAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
//    editToolBar->addAction(bringToFrontAction);
//    editToolBar->addAction(sendToBackAction);
}

/*
 *
 */
void editorInterface::createStaticToolBar() {
    // FIXME: change chortcuts
    ADD_SIGNAL(newTabAction, "New &Tab", "+T", "Ctrl+T", this, SLOT(actionNewTab_triggered()));

    ADD_SIGNAL(deleteTabAction, "Delete &Tab", "+T", "Ctrl+W", this, SLOT(actionDeleteTab_triggered()));

    ADD_SIGNAL(saveAction, "Save&", "S", "Ctrl+S", this, SLOT(actionSave_triggered()));

    ADD_SIGNAL(undoAction, "Undo", "<", "Ctrl+z", this, SLOT(actionUndo()));
    ADD_SIGNAL(redoAction, "Redo", ">", "Ctrl+Z", this, SLOT(actionRedo()));

    editToolBar = addToolBar(tr("Actions"));
    editToolBar->setFloatable(false);
    editToolBar->setMovable(false);

    editToolBar->addAction(newTabAction);
    editToolBar->addAction(deleteTabAction);
    editToolBar->addAction(saveAction);
    editToolBar->addSeparator();
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
}

/**
 *
 * @return
 */
QString editorInterface::get_text_representation() {
    auto size = tabWidget->count();
    std::string prg;
    for (int i = 0; i < size; i++) {
        prg += reinterpret_cast<TabCanvas *>(tabWidget->widget(i))->get_string_representation();
    }
    return {prg.c_str()};
}

/**
 *
 */
void editorInterface::actionSave_triggered() {
    if (filename == nullptr || filename == "") {
        actionSave_As_triggered();
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

void editorInterface::actionSave_As_triggered() {
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

void editorInterface::actionQuit_triggered() {
    editorInterface::close();
}

void editorInterface::actionNewTab_triggered() {
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");
    //    tabWidget->setCurrentIndex(tabWidget->count()-1);
}

void editorInterface::actionDeleteTab_triggered() {
    auto ci = tabWidget->currentIndex();
    if (ci == 0) {
        QMessageBox::warning(this, "Warning", "You cannot close the class diagram");
        return;
    }
    tabWidget->removeTab(ci);
    qDebug() << "tab deleted";

    //    tabWidget->setCurrentIndex(0);
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
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->remove();
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
void editorInterface::actionProperties_triggered() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->properties();
}

/**
 *
 */
void editorInterface::actionUndo() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->undo();
}

/**
 *
 */
void editorInterface::actionRedo() {
    reinterpret_cast<TabCanvas *>(tabWidget->currentWidget())->redo();
}
