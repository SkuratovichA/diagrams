#include "DiagramItem.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QInputEvent>

/**
 * A constructor.
 *
 * This constructor creates a text connected with its parent.
 *
 * @param p parent item
 * @param text text represented by a string
 * @param pos position of the text
 * @param flags flags for text that make it editable
 */
ClassTextAttr::ClassTextAttr(ClassDiagramItem *p, QString text, QPointF pos,
                             QFlags<Qt::TextInteractionFlag> flags)
        : QGraphicsTextItem(text, p) {
    setTextInteractionFlags(flags);
    setPos(pos);
    _parent = p;
}

/**
 * A destructor.
 */
ClassTextAttr::~ClassTextAttr() {

}

/**
 * Handle an event of the text editing to align it.
 * In the case of entering "Key_Enter" the text editing is ended.
 *
 * @param event key event
 */
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

/**
 * A constructor.
 *
 * This constructor creates an entity for the sequence diagram.
 * It is a simple rectangle with a child item, which represents
 * the name of the object.
 *
 * @param params class with all necessary information to create an object.
 */
SequenceDiagramItem::SequenceDiagramItem(actorParams *params)
        : DiagramItem(params->width(),
                      params->height(),
                      DiagramItem::Actor,
                      params->color()) {

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    QFlags<Qt::TextInteractionFlag> _flags = Qt::TextInteractionFlag::TextEditable |
                                             Qt::TextInteractionFlag::TextSelectableByMouse |
                                             Qt::TextInteractionFlag::TextSelectableByKeyboard;

    setPen(QPen(QColor(1, 0, 0, 0)));
    _head = new NameObject(this, _flags, QPointF(-3, -40), params->name());

    auto setpen = [this](QGraphicsLineItem *l) {l->setPen(QPen(color(), 3.0));};
//    auto legL = new QGraphicsLineItem(35.0, 70.0, 10.0, 110.0, this);
//    setpen(legL);
//    auto legR = new QGraphicsLineItem(35.0, 70.0, 60.0, 110.0, this);
//    setpen(legR);
//    auto handL = new QGraphicsLineItem(35.0, 45.0, 0.0, 35.0, this);
//    setpen(handL);
//    auto handR = new QGraphicsLineItem(35.0, 45.0, 70.0, 35.0, this);
//    setpen(handR);
//    auto body = new QGraphicsLineItem(35.0, 30.0, 35.0, 70.0, this);
//    setpen(body);
//    auto head = new QGraphicsEllipseItem(20.0, 0.0, 30.0, 30.0, this);
//    auto textAttr = new ClassTextAttr(this, params->name(), tabText(), rowHeight() + tabText(), _flags);
    auto actorRect = new QGraphicsRectItem(0, 0, params->width(), params->height(), this);
    actorRect->setPen(QPen(QColor(Qt::black), 3.0));
    actorRect->setBrush(QBrush(color()));

    auto lifetime = new ActorLifetime(this, QPointF(width() / 2, height()));
    setRect(boundingBox());
}

QVariant SequenceDiagramItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(value.toPointF().x(), pos().y());
    return QGraphicsItem::itemChange( change, value );
}

/**
 *
 * @param connection
 */
void SequenceDiagramItem::addConnection(ActorConnectionItem *connection) {
    _connections.insert(connection);
}

/**
 *
 * @param connection
 */
void SequenceDiagramItem::removeConnection(ActorConnectionItem *connection) {
    _connections.remove(connection);
}

/**
 * A constructor.
 *
 * This constructor creates an entity for the class diagram.
 * TODO description
 *
 * @param params class with all necessary information to create an object.
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

    _head = new NameObject(this, _flags, QPointF(20, -40), params->name()); // i do not know why coordinates 5, -40

    textAttr = new ClassTextAttr(this, "ATTRIBUTES", QPointF(tabText(), tabText()), Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));

    for (auto attr_name: params->attrs()) {
        lineAttr = createLine(0, rowHeight() * line);
        _attrsLines.push_back(lineAttr);
        textAttr = new ClassTextAttr(this, attr_name, QPointF(tabText(), rowHeight() * line + tabText()), _flags);
        _attrs.push_back(textAttr);

        line++;
    }

    lineAttr = createLine(0, rowHeight() * line);
    _methodsLines.push_back(lineAttr);

    textAttr = new ClassTextAttr(this, "METHODS", QPointF(tabText(), rowHeight() * line++ + tabText()), Qt::NoTextInteraction);
    textAttr->setFont(QFont("Times", 10, QFont::Bold));
    _methods.push_back(textAttr);

    for (auto method_name: params->methods()) {
        lineAttr = createLine(0, rowHeight() * line);
        _methodsLines.push_back(lineAttr);
        textAttr = new ClassTextAttr(this, method_name, QPointF(tabText(), rowHeight() * line + tabText()), _flags);
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
 * A destructor.
 */
ClassDiagramItem::~ClassDiagramItem() {
            foreach (ClassConnectionItem *connection, _connections) {
            delete connection;
            qDebug() << "Connection deleted (diagramItem.cpp)";
        }
}

/**
 * A constructor.
 *
 * This constructor creates a text connected with its parent.
 *
 * @param parent parent item
 * @param flags flags for text that make it editable
 * @param pos position of the text
 * @param str text represented by a string
 */
NameObject::NameObject(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, QPointF pos, QString str)
        : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(pos);
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

