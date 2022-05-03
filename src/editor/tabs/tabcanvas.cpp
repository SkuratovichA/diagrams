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
    QUndoCommand *connectionCommand = nullptr;
    switch (type) {
        case DiagramType::SEQUENCE:
//            connectionCommand = new AddActorConnectionCommand(editorScene);
            break;

        case DiagramType::CLASS:
//            connectionCommand = new AddClassConnectionCommand(fromNode, toNodes, editorScene);
            break;
        default:
            assert(!"This statement must not be reached");
            return;
     }
     undoStack->push(connectionCommand);
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

void TabCanvas::addMethod_triggered() {
    ClassDiagramItem *item = static_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() + item->rowHeight());

    auto line = item->createLine(0, item->height());
    item->pushMethodLine(line);

    auto text = item->createText(item->tabText(), item->height() + item->tabText(), "+ int example()");
    item->pushMethod(text);

    item->setHeight(item->height() + item->rowHeight());
    qDebug() << "add Method";
};

void TabCanvas::rmMethod_triggered() {
    ClassDiagramItem *item = static_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // one default METHODS
    auto size = item->getMethods().size();
    if (size < 2) {
        qDebug() << "No methods";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() - item->rowHeight());

    // delete line and text
    item->popMethod();
    item->popMethodsLine();

    item->setHeight(item->height() - item->rowHeight());
    qDebug() << "delete Method";
};

void TabCanvas::addAttr_triggered() {
    ClassDiagramItem *item = static_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() + item->rowHeight());

    long long inc = item->getAttrs().size() + 1;
    item->moveTexts(1, inc);
    item->moveLines(1, inc);

    auto line = item->createLine(0, inc * item->rowHeight());
    item->pushAttrLine(line);

    auto text = item->createText(item->tabText(), item->rowHeight() * inc + item->tabText(), "+ int word");
    item->pushAttr(text);

    item->setHeight(item->height() + item->rowHeight());
    qDebug() << "add Attr";
};

void TabCanvas::rmAttr_triggered() {
    ClassDiagramItem *item = static_cast<ClassDiagramItem *>(selectedObject());

    if (item == nullptr) {
        qDebug() << "No selected item";
        return;
    }

    auto size = item->getMethods().size();
    if (size < 1) {
        qDebug() << "No methods";
        return;
    }

    // resize item for one row
    item->setRect(0,0,item->width(), item->height() - item->rowHeight());

    // delete line and text
    item->popAttr();
    item->popAttrsLine();

    // move items
    long long inc = item->getAttrs().size() + 1;
    item->moveTexts(0, inc);
    item->moveLines(0, inc);

    //item->setPos(tmp_x, tmp_y);
    item->setHeight(item->height() - item->rowHeight());
    qDebug() << "delete Attr";
};

/**
 *
 */
void TabCanvas::properties() {
    addMethod = new QAction(tr("Add &Method"));
    connect(addMethod, SIGNAL(triggered()), this, SLOT(addMethod_triggered()));

    rmMethod = new QAction(tr("Delete &Method"));
    connect(rmMethod, SIGNAL(triggered()), this, SLOT(rmMethod_triggered()));

    addAttr = new QAction(tr("Add &Attribute"));
    connect(addAttr, SIGNAL(triggered()), this, SLOT(addAttr_triggered()));

    rmAttr = new QAction(tr("Delete &Attribute"));
    connect(rmAttr, SIGNAL(triggered()), this, SLOT(rmAttr_triggered()));

    editMenu = menuBar()->addMenu(tr("Properties"));
    editMenu->addAction(addMethod);
    editMenu->addAction(rmMethod);
    editMenu->addAction(addAttr);
    editMenu->addAction(rmAttr);

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
