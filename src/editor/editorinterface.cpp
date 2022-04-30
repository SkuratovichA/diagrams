#include "editorinterface.h"
#include "ui_editorinterface.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>

#include "tabs/tabcanvas.h"


////////////////////////////////
editorInterface::editorInterface(
        QWidget *parent,
        const QString &exampleName,
        newWindowType new_type
) :
        QMainWindow(parent),
        ui(new Ui::editorInterface) {
    setAttribute(Qt::WA_DeleteOnClose); // automatically delete itself when window is closed

    (void) exampleName;
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
//

    ui->setupUi(this);
    this->setWindowTitle("editor");

    createTabs();
    createActions();
    createToolBars();
}

editorInterface::~editorInterface() {
    delete ui;
}

void editorInterface::createTabs() {
    tabWidget = new QTabWidget(this);
    this->setCentralWidget(tabWidget);
    tabWidget->addTab(new TabCanvas(this, DiagramType::CLASS), "class diagram editor");
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");
}

void editorInterface::createToolBars() {
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
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);
}

void editorInterface::createActions() {
#ifdef __APPLE__
#define KEY "Cmd"
#else
#define KEY "Ctrl"
#endif
#define ADD_SIGNAL(obj, name, icon, shortcut, memberslot) \
    do {                                                  \
        obj = new QAction(tr((name)), this);                          \
        /*obj->setIcon(icon);*/                                       \
        (obj)->setShortcut(tr(shortcut));                             \
        connect((obj), SIGNAL(triggered()), tabWidget->currentWidget(), memberslot);\
    } while(0)

    ADD_SIGNAL(addEntityAction, "New &Entity", "+", KEY"N", SLOT(addEntity()));
    ADD_SIGNAL(addConnectionAction, "Connect", "->", KEY"L", SLOT(addConnection()));
    ADD_SIGNAL(deleteAction, "Delete", "-", KEY"D", SLOT(remove()));
    ADD_SIGNAL(cutAction, "Cut", "x", KEY"X", SLOT(cut()));
    ADD_SIGNAL(copyAction, "Copy", "c", KEY"C", SLOT(copy()));
    ADD_SIGNAL(pasteAction, "Paste", "v", KEY"V", SLOT(paste()));
    ADD_SIGNAL(propertiesAction, "Properties", "i", KEY"I", SLOT(properties()));
    ADD_SIGNAL(sendToBackAction, "Send to back", "b", KEY"B", SLOT(sendToBack()));
    ADD_SIGNAL(bringToFrontAction, "Send to front", "f", KEY"F", SLOT(sendToFront()));
#undef ADD_SIGNAL
}


// Zooming
void editorInterface::actionZoom_In_triggered() {
    // TODO: zoom in
    QMessageBox::information(this, "TODO", "zoom in");
}


void editorInterface::actionZoom_Out_triggered() {
    // TODO: zoom out
    QMessageBox::information(this, "TODO", "zoom out");
}


void editorInterface::actionReset_Zoom_triggered() {
    // TODO: reset zoom
    QMessageBox::information(this, "TODO", "reset zoom   ");
}


void editorInterface::actionCopy_triggered() {
    // TODO: copy the selected object
    QMessageBox::information(this, "TODO", "copy");
}


void editorInterface::actionPaste_triggered() {
    // TODO: paste the selected object
    QMessageBox::information(this, "TODO", "paste");
}


void editorInterface::actionCut_triggered() {
    // TODO: cut an object
    QMessageBox::information(this, "TODO", "cut");
}


void editorInterface::actionUndo_triggered() {
    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
    QMessageBox::information(this, "TODO", "undo");
}


void editorInterface::actionRedo_triggered() {
    // TODO: https://stackoverflow.com/questions/14998836/implementing-undo-redo-functionality-in-qt
    QMessageBox::information(this, "TODO", "Redo");
}


void editorInterface::actionSave_triggered() {
    if (this->filename == nullptr || this->filename == "") {
        this->actionSave_As_triggered();
        return;
    }

    QFile file(this->filename);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Error", file.errorString());
        return;
    }
    QTextStream in(&file);
    QString gaeRepresentation = "TODO: convert an internal representation into a string\n";
    in << gaeRepresentation;
}


void editorInterface::actionSave_As_triggered() {
    // TODO: chose a file and save it
#if 0
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Address Book"), QDir::homePath(),
                                                    this->filenameFilter);
    if (fileName == nullptr || fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    QTextStream out(&file);

    // TODO write an internal representation to a file
    out << "Hello world";
    QMessageBox::information(this, "TODO", "save as");
#endif
}


void editorInterface::actionQuit_triggered() {
    editorInterface::close();
}
