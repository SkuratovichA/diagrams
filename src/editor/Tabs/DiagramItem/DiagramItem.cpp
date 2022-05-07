#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QInputEvent>

ClassTextAttr::ClassTextAttr(ClassDiagramItem *p, QString text, qreal x, qreal y,
                             QFlags<Qt::TextInteractionFlag> flags)
        : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(x, y);
    _parent = p;
}

ClassTextAttr::~ClassTextAttr() {

}

void ClassTextAttr::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }

    qreal maxLen = 0;

    for (auto item: parent()->attrs()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }
    for (auto item: parent()->methods()) {
        maxLen = item->boundingRect().width() > maxLen ? item->boundingRect().width() : maxLen;
    }

    if (maxLen + 30 < parent()->width()) {
        maxLen = parent()->width() - 30;
    } else if (maxLen + 20 > parent()->width()) {
        maxLen = parent()->width() + 20;
    } else {
        return;
    }

    parent()->setRect(0, 0, maxLen, parent()->height());
    for (auto item: parent()->attrsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    for (auto item: parent()->methodsLines()) {
        item->setLine(0, 0, maxLen, 0);
    }
    parent()->setWidth(maxLen);

    qreal midW = parent()->_head->boundingRect().width();
    qreal midO = parent()->width();
    parent()->_head->setPos((midO - midW) / 2, -40);
    for (auto x: this->parent()->connections()) {
        x->trackNodes();
    }
}

SequenceDiagramItem::SequenceDiagramItem(actorParams *params, ClassDiagramItem *parentCLassDiagramItem_)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Actor,
                      params->color()
) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;

    setPen(QPen(QColor(1, 0, 0, 0)));
    _head = new NameObject(this, _flags, -3, -40, params->name());
    _parentClassDiagramItem = parentCLassDiagramItem_;

    auto actorRect = new QGraphicsRectItem(0, 0, params->width(), params->height(), this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(color()));

    auto lifetime = new ActorLifetime(this, QPointF(width() / 2, height()));
    setRect(boundingBox());
}

QVariant SequenceDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(value.toPointF().x(), pos().y());
    return QGraphicsItem::itemChange(change, value);
}

/**
 *
 * @param connection
 */
void SequenceDiagramItem::addConnection(SequenceConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 *
 * @param connection
 */
void SequenceDiagramItem::removeConnection(SequenceConnectionItem *connection) {
    _connections.remove(connection);
}

/**
 *
 * @param item
 */
ClassDiagramItem::ClassDiagramItem(classParams *params)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Class,
                      params->color()) {

    QGraphicsLineItem *lineAttr;
    ClassTextAttr *textAttr;
    _flags = Qt::TextInteractionFlag::TextEditable |
             Qt::TextInteractionFlag::TextSelectableByMouse |
             Qt::TextInteractionFlag::TextSelectableByKeyboard;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    int line = 1;

    // name of the class
    setPen(QPen(color()));

    _head = new NameObject(this, _flags, 20, -40, params->name()); // i do not know why coordinates 5, -40

    textAttr = new ClassTextAttr(this, "ATTRIBUTES", tabText(), tabText(), Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));

    for (auto attr_name: params->attrs()) {
        lineAttr = createLine(0, rowHeight() * line);
        _attrsLines.push_back(lineAttr);
        textAttr = new ClassTextAttr(this, attr_name, tabText(), rowHeight() * line + tabText(), _flags);
        _attrs.push_back(textAttr);

        line++;
    }

    lineAttr = createLine(0, rowHeight() * line);
    _methodsLines.push_back(lineAttr);

    textAttr = new ClassTextAttr(this, "METHODS", tabText(), rowHeight() * line++ + tabText(), Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));
    _methods.push_back(textAttr);

    for (auto method_name: params->methods()) {
        lineAttr = createLine(0, rowHeight() * line);
        _methodsLines.push_back(lineAttr);
        textAttr = new ClassTextAttr(this, method_name, tabText(), rowHeight() * line + tabText(), _flags);
        _methods.push_back(textAttr);

        line++;
    }

    setRect(boundingBox());
    setBrush(QBrush(QColor(255, 255, 255, 255)));
}

/**
 *
 * @param connection
 */
void ClassDiagramItem::addConnection(ClassConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 *
 * @param connection
 */
void ClassDiagramItem::removeConnection(ClassConnectionItem *connection) {
    _connections.remove(connection);
}

void ClassDiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}

/**
 *
 * @param change
 * @param value
 * @return
 */
QVariant ClassDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (auto *connection: _connections) {
            connection->trackNodes();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

/**
 *
 */
ClassDiagramItem::~ClassDiagramItem() {
            foreach (ClassConnectionItem *connection, _connections) {
            delete connection;
            qDebug() << "Connection deleted (diagramItem.cpp)";
        }
}

/**
 *
 * @param parent
 * @param flags
 * @param x
 * @param y
 */
NameObject::NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str)
        : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(x, y);
    setFont(QFont("Courier", 20));
    setTextInteractionFlags(flags);
    topLevelItem();
}

/**
 *
 * @param event
 */
void NameObject::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }
    auto *tmp1 = dynamic_cast<ClassDiagramItem *>(parent());
    auto *tmp2 = dynamic_cast<SequenceDiagramItem *>(parent());
    qreal midO = tmp1 == nullptr ? tmp2->width() : tmp1->width();
    qreal midW = boundingRect().width();
    setPos((midO - midW) / 2, -40);
}

NameObject::~NameObject() {

}

/**
 *
 * @param parent
 * @param startPoint
 */
ActorLifetime::ActorLifetime(QGraphicsItem *parent, QPointF startPoint) : QGraphicsLineItem(parent) {
    auto x = startPoint.x();
    auto y = startPoint.y();
    auto endPoint = QPoint(x, y + 400);
    auto lline = new QGraphicsLineItem(QLineF(startPoint, endPoint), this);
    lline->setPen(QPen(QColor(Qt::black), 2.0, Qt::DashLine));
}

//void ActorLifetime::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//
//}

/**
 *
 */
ActorLifetime::~ActorLifetime() {

}
