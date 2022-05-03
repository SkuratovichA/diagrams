#include <QGraphicsView>
#include <QUndoStack>
#include <QUndoGroup>
#include "tabcanvas.h"
#include "diagram/Commands.h"

using namespace SceneType;

class editorInterface;

/**
*
* @param parent
* @param type
*/
TabCanvas::TabCanvas(QWidget *parent, DiagramType type, QUndoGroup *parentGroup) {
    undoStack = new QUndoStack(parentGroup);
    createScene();
    if (type != DiagramType::CLASS && type != DiagramType::SEQUENCE) {
        throw std::runtime_error("Unknown diagram type");
    }
    this->type = type;
}

//void DiagramWindow::createActions()
//{
//    exitAction = new QAction(tr("E&xit"), this);
//    exitAction->setShortcut(tr("Ctrl+Q"));
//    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
//
//    addNodeAction = new QAction(tr("Add &Attr"), this);
//    addNodeAction->setIcon(QIcon(":/images/node.png"));
//    addNodeAction->setShortcut(tr("Ctrl+N"));
//    connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
//
//    addLinkAction = new QAction(tr("Add &Method"), this);
//    addLinkAction->setIcon(QIcon(":/images/link.png"));
//    addLinkAction->setShortcut(tr("Ctrl+L"));
//    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));
//
//    addNodeAction = new QAction(tr("Add &Attr"), this);
//    addNodeAction->setIcon(QIcon(":/images/node.png"));
//    addNodeAction->setShortcut(tr("Ctrl+N"));
//    connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
//
//    addLinkAction = new QAction(tr("Add &Method"), this);
//    addLinkAction->setIcon(QIcon(":/images/link.png"));
//    addLinkAction->setShortcut(tr("Ctrl+L"));
//    connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));
//
//    deleteAction = new QAction(tr("&Delete"), this);
//    deleteAction->setIcon(QIcon(":/images/delete.png"));
//    deleteAction->setShortcut(tr("Del"));
//    connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));
//
//    cutAction = new QAction(tr("Cu&t"), this);
//    cutAction->setIcon(QIcon(":/images/cut.png"));
//    cutAction->setShortcut(tr("Ctrl+X"));
//    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
//
//    copyAction = new QAction(tr("&Copy"), this);
//    copyAction->setIcon(QIcon(":/images/copy.png"));
//    copyAction->setShortcut(tr("Ctrl+C"));
//    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
//
//    pasteAction = new QAction(tr("&Paste"), this);
//    pasteAction->setIcon(QIcon(":/images/paste.png"));
//    pasteAction->setShortcut(tr("Ctrl+V"));
//    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
//
//    bringToFrontAction = new QAction(tr("Bring to &Front"), this);
//    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
//    connect(bringToFrontAction, SIGNAL(triggered()),
//            this, SLOT(bringToFront()));
//
//    sendToBackAction = new QAction(tr("&Send to Back"), this);
//    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
//    connect(sendToBackAction, SIGNAL(triggered()),
//            this, SLOT(sendToBack()));
//
//    propertiesAction = new QAction(tr("P&roperties..."), this);
//    connect(propertiesAction, SIGNAL(triggered()),
//            this, SLOT(properties()));
//}
//
//void DiagramWindow::createMenus()
//{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(exitAction);
//
//    editMenu = menuBar()->addMenu(tr("&Edit"));
//    editMenu->addAction(addNodeAction);
//    editMenu->addAction(addLinkAction);
//    editMenu->addAction(deleteAction);
//    editMenu->addSeparator();
//    editMenu->addAction(cutAction);
//    editMenu->addAction(copyAction);
//    editMenu->addAction(pasteAction);
//    editMenu->addSeparator();
//    editMenu->addAction(bringToFrontAction);
//    editMenu->addAction(sendToBackAction);
//    editMenu->addSeparator();
//    editMenu->addAction(propertiesAction);
//}
//
//void DiagramWindow::createToolBars()
//{
//    editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(addNodeAction);
//    editToolBar->addAction(addLinkAction);
//    editToolBar->addAction(deleteAction);
//    editToolBar->addSeparator();
//    editToolBar->addAction(cutAction);
//    editToolBar->addAction(copyAction);
//    editToolBar->addAction(pasteAction);
//    editToolBar->addSeparator();
//    editToolBar->addAction(bringToFrontAction);
//    editToolBar->addAction(sendToBackAction);
//}

/**
 *
 */
TabCanvas::~TabCanvas() {
    delete layout;
}

/**
 *
 */
void TabCanvas::createScene() {
    editorScene = new EditorScene(this);

    editorScene->setSceneRect(QRect(0, 0, 800, 800));
    connect(editorScene, &EditorScene::itemMoved, this, &TabCanvas::moveEntity);
    auto *view = new QGraphicsView(editorScene);

    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing
                               | QPainter::TextAntialiasing);

    setCentralWidget(view);
}

/**
 *
 */
void TabCanvas::moveEntity(ActorDiagramItem *movedItem, const QPointF &startPosition) {
    undoStack->push(new MoveCommand(movedItem, startPosition));
}

/**
 *
 */
void TabCanvas::removeEntity() {
    if (editorScene->selectedItems().isEmpty()) {
        return;
    }
    QUndoCommand *deleteCommand = new DeleteCommand(editorScene);
    undoStack->push(deleteCommand);
}

/**
 *
 */
void TabCanvas::addEntity() {
    QUndoCommand *addCommand = nullptr;
    switch (type) {
        case DiagramType::SEQUENCE:
            addCommand = new AddActorCommand(editorScene);
            break;
        case DiagramType::CLASS:
            addCommand = new AddClassCommand(editorScene);
            break;
        default:
            assert(!"This statement must not be reached");
            return;
    }
    undoStack->push(addCommand);
}

/**
 *
 */
void TabCanvas::addConnection() {
    // FIXME: create a function to select one or more nodes
    // then according to the type pass the parameters.
    // Probably, there will be a need to remove the first node and put it into the separate variabble
    // Then, call a function to connect n nodes.

//    switch (type) {
//        case DiagramType::SEQUENCE:
//            addCommand = new AddActorCommand(editorScene);
//            break;
//
//        case DiagramType::CLASS:
//            addCommand = new AddClassCommand(editorScene);
//            break;
//        default:
//            assert(!"This statement must not be reached");
//            return;
//     }
}

/**
 *
 * @return
 */
QGraphicsItem *TabCanvas::selectedObject() {
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<QGraphicsItem *>(items.first());
    } else {
        return 0;
    }
}

/**
 *
 */
void TabCanvas::paste() {
    qDebug() << "paste";
    // FIXME: paste
}

/**
 *
 */
void TabCanvas::cut() {
    qDebug() << "cut";
    // FIXME: implement me
}

/**
 *
 */
void TabCanvas::copy() {
    // TODO: implement me
#if 0
    qDebug() << "copy";
    Object *tmp = selectedObject();
        if(!tmp)
        return;

    QString str = QString("ActorDiagramItem %1 %2 %3 %4")
                    .arg(tmp->x())
                    .arg(tmp->y())
                    .arg(tmp->scale())
#endif
}

void TabCanvas::addAttr_triggered() {
    QGraphicsItem * item = selectedObject();

    qDebug() << item;
    qDebug() << "add attr";
};

/**
 *
 */
void TabCanvas::properties() {
    addAttr = new QAction(tr("Add &Attr"));

    //addAttr->setIcon(QIcon(":/images/node.png"));
    //addAttr->setShortcut(tr("Ctrl+T"));
    connect(addAttr, SIGNAL(triggered()), this, SLOT(addAttr_triggered()));

    editMenu = menuBar()->addMenu(tr("Add &Attr"));
    editMenu->addAction(addAttr);
    //editToolBar = addToolBar(tr("Edit"));
    //editToolBar->addAction(addAttr);


    qDebug() << "properties TabCanvas";
}

/**
 *
 */
void TabCanvas::sendToBack() {
    qDebug() << "send to back";
}

/**
 *
 */
void TabCanvas::sendToFront() {
    qDebug() << "send to front";
}

/**
 *
 * @return
 */
std::string TabCanvas::getStringRepresentation() {
    // FIXME: in the end
    return {"hello"};
}

/** When tab is changed, there is a need to manually set the current undo stack.
 *
 * @return undo stack
 */
QUndoStack *TabCanvas::getUndoStack() {
    return undoStack;
}
