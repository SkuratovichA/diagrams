/** @file SequenceCanvas.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 07.05.2022
 */


#include <QColor>
#include <QRandomGenerator>
#include <QWidget>
#include <QUndoGroup>
#include <QMessageBox>

#include "TabCanvas.h"
#include "../../EditorInterface/editorinterface.h"
#include "SequenceConnectionDialog/sequenceconnectiondialog.h"
#include <regex>

SequenceCanvas::SequenceCanvas(QWidget *parent, QUndoGroup *parentGroup) : TabCanvas(parent, parentGroup) {
    parentInterface = dynamic_cast<editorInterface *>(parent);
    createScene();
    createSequenceContextMenu();

}

QPoint SequenceCanvas::generateCoords() const {
    auto x = QRandomGenerator::global()->bounded(600) + 20;
    auto y = 100;
    return QPoint(x, y);
}

bool SequenceCanvas::createFromFile(dgrmSeq_t seq) {
    ItemsBuffer buf;

    for (auto x: seq.actors) {
        buf.addActorItems(x);
    }

    for (auto x: buf.sequenceItems()) {
        SequenceDiagramItem *diagramItem = new SequenceDiagramItem(x);
        editorScene->addItem(diagramItem);
        diagramItem->setPos(x->x(), x->y());
        editorScene->update();
    }

    QList<SequenceDiagramItem *> items = getItems<SequenceDiagramItem>();
    for (auto x: seq.actions) {
        buf.addMessageItems(x);
    }

    SequenceDiagramItem *from;
    SequenceDiagramItem *to;
    for (auto x: buf.messageItems()) {
        for (auto y: items) {
            if (x->nameFrom() == y->name()) {
                from = y;
            }
            if (x->nameTo() == y->name()) {
                to = y;
            }
        }

        if (from == nullptr || to == nullptr) {
            return false;
        }

        SequenceConnectionItem *item = new SequenceConnectionItem(from,
                                                                  to,
                                                                  x,
                                                                  static_cast<Connections::ConnectionType>(x->type()));
        editorScene->addItem(item);
        editorScene->update();
    }

    return true;
}

bool SequenceCanvas::checkIdenticalNames() {
    QList<SequenceConnectionItem *> itemsCoonnections = getItems<SequenceConnectionItem>();

    for (auto x: itemsCoonnections) {
        auto inconsistentMethod =
                !ObjectParams::checkMethod(x)
                && x->connectionType() != ConnectionType::Create
                && x->connectionType() != ConnectionType::Delete;

        if (inconsistentMethod) {
            auto methodNames = x->nodeTo()->parentClassDiagramItem()->methods();
            auto methodsToShow = QString();
            // get all meethods to print them
            for (auto x: methodNames) {
                // really shitty condition
                if (x->toPlainText() != "METHODS") {
                    methodsToShow += x->toPlainText() + "\n";
                }
            }
            QMessageBox::warning(this, "No such method",
                                 QString("No method with the name \"")
                                 + x->getText()->toPlainText()
                                 + QString("\".\n")
                                 + QString("List of all available methods:\n")
                                 + methodsToShow);
            return false;
        }
    }

    return true;
}

bool SequenceCanvas::getStringRepresentation(Program &prg) {
    std::vector<Action> actions;
    ItemsBuffer buf;

    dgrmSeq_t obj;

    for (auto x: getItems<SequenceDiagramItem>()) {
        buf.fillActorItems(x);
    }

    for (auto x: getItems<SequenceConnectionItem>()) {
        buf.fillMessageItems(x);
    }

    if (!checkIdenticalNames()) {
        return false;
    }

    for (auto x: buf.sequenceItems()) {
        Actor act;
        act.name = x->name().toStdString();
        x->fillColor(act.color);
        x->fillCoords(act.coords);

        obj.actors.push_back(act);
    }

    for (auto x: buf.messageItems()) {
        Action action;
        action.msg = x->msg().toStdString();
        x->fillCoords(action.coords);
        action.from = x->nameFrom().toStdString();
        action.to = x->nameTo().toStdString();
        action.type = x->type();

        obj.actions.push_back(action);
    }

    prg.diagramSequence.push_back(obj);

    // add for sequence diagram

    return true;
}

void SequenceCanvas::createSequenceContextMenu() {
    ADD_SIGNAL(asynchronousMessage, "Asynchronous Message", "+", "+", this, SLOT(asynchronousMessage_triggered()));
    ADD_SIGNAL(synchronousMessage, "Synchronous Message", "+", "+", this, SLOT(synchronousMessage_triggered()));
    ADD_SIGNAL(returnMessage, "Return Message", "+", "+", this, SLOT(returnMessage_triggered()));
    ADD_SIGNAL(createMessage, "Create Message", "+", "+", this, SLOT(createMessage_triggered()));
    ADD_SIGNAL(deleteMessage, "Delete Message", "+", "+", this, SLOT(deleteMessage_triggered()));

    sequenceMenu = new QMenu();
    sequenceMenu->addAction(asynchronousMessage);
    sequenceMenu->addAction(synchronousMessage);
    sequenceMenu->addAction(returnMessage);
    sequenceMenu->addAction(createMessage);
    sequenceMenu->addAction(deleteMessage);
}

void SequenceCanvas::showContextMenu(const QPoint &pos) {
    auto *item = selectedObject<SequenceDiagramItem>();

    if (item == nullptr) {
        return;
    }

    if (dynamic_cast<SequenceDiagramItem *>(item) != nullptr) {
        sequenceMenu->exec(this->mapToGlobal(pos));
    } else {
        assert(!"implement sequence menu invocation");
    }
}

void SequenceCanvas::asynchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Aggregation);
    editorScene->update();
}

void SequenceCanvas::synchronousMessage_triggered() {
    auto line = (selectedObject<ClassConnectionItem>());
    line->setType(ClassConnectionItem::Composition);
    editorScene->update();
}

void SequenceCanvas::returnMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Generalization);
    editorScene->update();
}

void SequenceCanvas::createMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Association);
    editorScene->update();
}

void SequenceCanvas::deleteMessage_triggered() {
    auto line = selectedObject<ClassConnectionItem>();
    line->setType(ClassConnectionItem::Dependency);
    editorScene->update();
}

void SequenceCanvas::addEntity(ClassDiagramItem *classDiagramItemParent) {
    QPoint point = generateCoords();

    createActor = new SequenceDiagramItemParameters(point.x(), point.y(),
                                                    classDiagramItemParent->name(),
                                                    classDiagramItemParent->color());
    _undoStack->push(new AddSequenceCommand(editorScene,
                                            createActor,
                                            classDiagramItemParent)
    );

    delete createActor;
}

void SequenceCanvas::addConnection() {
    auto scd = SequenceConnectionDialog(this);
    scd.exec();

    auto nodes = getSelectedDiagramItems<SequenceDiagramItem>();

    auto emptySelect =
            (nodes == QPair<SequenceDiagramItem *, SequenceDiagramItem *>())
            || nodes.first == nodes.second;

    if (emptySelect) {
        return;
    }
    if (nodes.first == nullptr || nodes.second == nullptr) {
        return;
    }
    auto index = scd.messageType();
    if (index == ConnectionType::Undefined) {
        return;
    }

    paramsMessage = new messageParams(0, 200,
                                      "TEXT",
                                      nodes.first->name(),
                                      nodes.second->name(),
                                      index);

    _undoStack->push(new AddSequenceConnectionCommand(nodes.first,
                                                      nodes.second,
                                                      paramsMessage,
                                                      index,
                                                      editorScene));

    delete paramsMessage;
}

void SequenceCanvas::paste() {
    for (auto ptr: buffer->sequenceItems()) {
        auto *diagramItem = new SequenceDiagramItem(ptr);
        diagramItem->setPos(ptr->x(), ptr->y());
        editorScene->addItem(diagramItem);
    }
    editorScene->update();
}

void SequenceCanvas::copy() {
    SequenceDiagramItem *ptr;
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    buffer->clearBuffer();
    for (auto val: items) {
        ptr = dynamic_cast<SequenceDiagramItem *>(val);
        if (ptr != nullptr) {
            buffer->fillActorItems(ptr);
        }
    }
}

void SequenceCanvas::cut() {
    copy();
    QList<QGraphicsItem *> items = editorScene->selectedItems();
    for (auto val: items) {
        editorScene->removeItem(val);
    }
}

void SequenceCanvas::toBack() {
    _toZchange<SequenceCanvas>(false);
}

void SequenceCanvas::toFront() {
    _toZchange<SequenceCanvas>(true);
}
