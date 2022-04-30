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
    createActions();

    createToolBars();

    createStaticToolBar();
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
}


editorInterface::~editorInterface() {
    delete ui;
}

void editorInterface::tabSelected() {
    static TabCanvas *prevTab = nullptr;
    if (prevTab != nullptr) {
        disconnectSlots(prevTab);
    }
    qDebug() << "selected tabs and disconnected";

    connect( addEntityAction     , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(addEntity()));
    connect( addConnectionAction , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(addConnection()));
    connect( deleteAction        , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(remove()));
    connect( cutAction           , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(cut()));
    connect( copyAction          , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(copy()));
    connect( pasteAction         , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(paste()));
    connect( propertiesAction    , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(properties()));
    prevTab = dynamic_cast<TabCanvas *>(tabWidget->currentWidget());
//    connect( sendToBackAction    , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(sendToBack()));
//    connect( bringToFrontAction  , SIGNAL(triggered()), tabWidget->currentWidget(), SLOT(sendToFront()));

}

void editorInterface::disconnectSlots(TabCanvas* prevTab) {
    #define DISCONNECT(obj, memberslot)\
            disconnect((obj), SIGNAL(triggered()), prevTab, memberslot)\

    DISCONNECT(addEntityAction     , SLOT(addEntity()));
    DISCONNECT(addConnectionAction , SLOT(addConnection()));
    DISCONNECT(deleteAction        , SLOT(remove()));
    DISCONNECT(cutAction           , SLOT(cut()));
    DISCONNECT(copyAction          , SLOT(copy()));
    DISCONNECT(pasteAction         , SLOT(paste()));
    DISCONNECT(propertiesAction    , SLOT(properties()));
//    DISCONNECT(sendToBackAction    , SLOT(sendToBack()));
//    DISCONNECT(bringToFrontAction  , SLOT(sendToFront()));
}

void editorInterface::createTabs() {
    tabWidget = new QTabWidget(this);
    this->setCentralWidget(tabWidget);
    tabWidget->addTab(new TabCanvas(this, DiagramType::CLASS), "class diagram editor");
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");
}

void editorInterface::createToolBars() {
    #ifdef __APPLE__
#define KEY "Ctrl"
#else
#define KEY "Ctrl"
#endif
    ADD_SIGNAL(addEntityAction, "New &Entity", "+", KEY"N",     tabWidget->currentWidget(), SLOT(addEntity()));
    ADD_SIGNAL(addConnectionAction, "Connect", "->", KEY"L",    tabWidget->currentWidget(), SLOT(addConnection()));
    ADD_SIGNAL(deleteAction, "Delete", "-", KEY"D",             tabWidget->currentWidget(), SLOT(remove()));
    ADD_SIGNAL(cutAction, "Cut", "x", KEY"X",                   tabWidget->currentWidget(), SLOT(cut()));
    ADD_SIGNAL(copyAction, "Copy", "c", KEY"C",                 tabWidget->currentWidget(), SLOT(copy()));
    ADD_SIGNAL(pasteAction, "Paste", "v", KEY"V",               tabWidget->currentWidget(), SLOT(paste()));
    ADD_SIGNAL(propertiesAction, "Properties", "i", KEY"I",     tabWidget->currentWidget(), SLOT(properties()));
//    ADD_SIGNAL(sendToBackAction, "Send to back", "b", KEY"B",   tabWidget->currentWidget(), SLOT(sendToBack()));
//    ADD_SIGNAL(bringToFrontAction, "Send to front", "f", KEY"F",tabWidget->currentWidget(), SLOT(sendToFront()));

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

void editorInterface::createStaticToolBar() {
    ADD_SIGNAL(newTabAction, "New &Tab", "+T", KEY"T", this, SLOT(actionNewTab_triggered()));

    ADD_SIGNAL(deleteTabAction, "Delete &Tab", "+T", KEY"W", this, SLOT(actionDeleteTab_triggered()));

    ADD_SIGNAL(saveAction, "Save&", "S", KEY"S",       this, SLOT(actionSave_triggered()));
    ADD_SIGNAL(undoAction, "Undo", "<", KEY"z",        this, SLOT(actionUndo_triggered()));
    ADD_SIGNAL(redoAction, "Redo", ">", KEY"Z",        this, SLOT(actionRedo_triggered()));

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


void editorInterface::createActions() {

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

void editorInterface::actionNewTab_triggered() {
    qDebug() << "New tab";
    tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");
//    tabWidget->setCurrentIndex(tabWidget->count()-1);
}

void editorInterface::actionDeleteTab_triggered() {
    qDebug() << "Delete tab";
    //tabWidget->addTab(new TabCanvas(this, DiagramType::SEQUENCE), "sequence diagram editor");

    auto ci = tabWidget->currentIndex();
    if (ci == 0) {
        QMessageBox::warning(this, "Warning", "You cannot close the class diagram");
        return;
    }
    tabWidget->removeTab(ci);
    qDebug() << "tab deleted";

//    tabWidget->setCurrentIndex(0);
}



