// File: SequenceConnections.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 07.05.2022

#define DEBUG 1

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QStyle>
#include <QPainter>

#include "Connections.h"
#include "../DiagramItem/DiagramItem.h"
#include "../DiagramItem/SequenceDiagramLifeLine/SequenceDiagramLifeLine.h"

using namespace Connections;

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
SequenceConnectionItem::SequenceConnectionItem(SequenceDiagramItem *nodeFrom,
                                               SequenceDiagramItem *nodeTo,
                                               messageParams *params,
                                               ConnectionType connectionType
) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    _nodeFrom->addConnection(this, Caller);
    _nodeTo->addConnection(this, Receiver);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    switch (_connectionType) {
        case Create:
            text = new msgText(this, QFlags<Qt::TextInteractionFlag>(0), 0, 0, "<<create>>");
            break;
        case Delete:
            text = new msgText(this, QFlags<Qt::TextInteractionFlag>(0), 0, 0, "<<delete>>");
            break;
        default:
            text = new msgText(this, getFlags(), 0, 0, params->msg());
    }

    setZValue(1.0);
    setY(params->y());

    trackNodes();
}

/**
 *
 */
SequenceConnectionItem::~SequenceConnectionItem() {
}

/**
 * Changes the y-position of the line.
 * @param change
 * @param value
 * @return
 */
QVariant SequenceConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        return QPointF(
                pos().x(),
                std::min(
                        std::max<qreal>(value.toPointF().y(), _nodeFrom->centre().y()) + 10, // upper bound
                        _nodeFrom->lineLength() - 20 // lower bound
                )
        );
    }
    if (change == ItemPositionHasChanged) {
        _nodeFrom->trackNodes();
        _nodeTo->trackNodes();
        trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

/** Changing the line according to the x-position of the two nodes it is connected to.
 *
 */
void SequenceConnectionItem::trackNodes() {
    setLine(QLineF(
            QPointF(_nodeFrom->centre().x(), 0),
            QPointF(_nodeTo->centre().x(), 0)));
}

/**
 *
 * @return
 */
QRectF SequenceConnectionItem::lineShaper() const {
    auto rect = QRectF(
            QPointF(std::min(_nodeTo->centre().x() - 30, _nodeFrom->centre().x() - 30), -20),
            QPointF(std::max(_nodeTo->centre().x() + 30, _nodeFrom->centre().x() + 30), 20)
    );
    return rect;
}

/**
 *
 * @return
 */
QPainterPath SequenceConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
//    painter->drawPolygon(lineShaper());
//    painter->setPen(QPen(QColor(0, 0, 0, 120), .5, Qt::DashLine));
//    painter->drawPolygon(lineShaper());
//    //painter->strokePath(shape(), QPen(QColor(0, 0, 0, 120), 1.5, Qt::DashLine));
//
//    auto rect = lineShaper();
//    auto l = (rect.bottomLeft() + rect.topLeft()) / 2;
//    auto r = (rect.bottomRight() + rect.topRight()) / 2;
//    painter->setPen(QPen(QColor(255, 0, 0, 255), .3, Qt::SolidLine));
//    painter->drawLine(l, r);
//
//    painter->setPen(QPen(QColor(0, 255, 0, 255), .3, Qt::DashDotDotLine));
//    painter->drawLine(line());
#endif

    painter->setRenderHint(QPainter::Antialiasing, true);
    switch (_connectionType) {
        case Synchronous:
            paintSynchronous(painter, option, widget);
            break;

        case Asynchronous:
            paintAsynchronous(painter, option, widget);
            break;

        case Reply:
            paintReply(painter, option, widget);
            break;

        case Create:
            paintCreate(painter, option, widget);
            break;

        case Delete:
            paintDelete(painter, option, widget);
            break;
        default:
            assert(false);
    }

    qreal newPos = (cLine.p1().x() + cLine.p2().x() - widthText()) / 2.0;
    text->setPos(newPos, -25);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPolygonF arrowHead;
    setLineOther();

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);

    arrowHead << pUp << pEnd << pDown;
    painter->setBrush(QBrush(Qt::black));
    painter->drawPolygon(arrowHead);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPolygonF arrowHead;
    setLineOther();

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);

    arrowHead << pUp << pEnd << pDown << pEnd;
    painter->drawPolygon(arrowHead);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPolygonF arrowHead;
    setLineReplay();

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 20) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::DashLine));
    painter->drawLine(cLine);

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    arrowHead << pUp << pEnd << pDown << pEnd;
    painter->drawPolygon(arrowHead);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setLineOther();

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);
    painter->setBrush(QBrush(Qt::green));
    painter->drawEllipse(posX, pEnd.y() - 10, 20, 20);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setLineOther();

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::red), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);
    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(posX, pEnd.y() - 7, 20, 14);
}

void msgText::focusOutEvent(QFocusEvent *event) {
    SequenceConnectionItem *item = dynamic_cast<SequenceConnectionItem *>(this->parent());
    QString msgMethod;
    QString classMethod;
    bool flag = false;
    long long idx;
    if (item == nullptr) {
        return;
    }



    idx = item->getText()->toPlainText().toStdString().find('(');
    msgMethod = item->getText()->toPlainText().mid(0, idx);
    for (auto method : item->nodeTo()->parentClassDiagramItem()->methods()) {
        if (method->toPlainText() == "METHODS") {
            continue;
        }

        classMethod = getMethod(method->toPlainText().toStdString());

        if (classMethod == "") {
            qDebug() << "HUINYA METHOD";
        }
        else if (classMethod == msgMethod) {
            flag = true;
            qDebug() << classMethod;
            qDebug() << msgMethod;
        }
    }

    if (!flag) {
        item->getText()->setDefaultTextColor(QColor(Qt::red));
    }
    else {
        item->getText()->setDefaultTextColor(QColor(Qt::black));
    }

    this->clearFocus();
}

