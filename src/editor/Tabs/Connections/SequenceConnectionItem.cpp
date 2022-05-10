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

using namespace Connections;

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
SequenceConnectionItem::SequenceConnectionItem(SequenceDiagramItem *nodeFrom,
                                               SequenceDiagramItem *nodeTo,
                                               ConnectionType connectionType
) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    qDebug() << __FILE__;
    qDebug() << "   creating a connection";
    _nodeFrom->addConnection(this, Caller);
    _nodeTo->addConnection(this, Receiver);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    setZValue(-1.0);
    setY(200);

    trackNodes();
    qDebug() << "one connection";
    qDebug() << __FILE__ << "   connection created>";
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
                        std::max<qreal>(value.toPointF().y(), _nodeFrom->centre().y()),
                        _nodeFrom->lineLength() + _nodeFrom->centre().y()
                )
        );
    }
    if (change == ItemPositionHasChanged) {
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
    // fixme: add constraints
    auto rect = QRectF(
            QPointF(_nodeFrom->centre().x(), -10),
            QPointF(_nodeTo->centre().x(), 10)
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
    painter->setPen(QPen(QColor(0, 0, 0, 120), .5, Qt::DashLine));
    painter->drawPolygon(lineShaper());

    auto rect = lineShaper();
    auto l = (rect.bottomLeft() + rect.topLeft()) / 2;
    auto r = (rect.bottomRight() + rect.topRight()) / 2;
    painter->setPen(QPen(QColor(255, 0, 0, 255), .3, Qt::SolidLine));
    painter->drawLine(l, r);

    painter->setPen(QPen(QColor(0, 255, 0, 255), .3, Qt::DashDotDotLine));
    painter->drawLine(line());

#endif

    painter->setRenderHint(QPainter::Antialiasing, true);
//    if (option->state & QStyle::State_Selected) {
//        painter->setPen(QPen(_color.darker(), 2.5, Qt::SolidLine));
//    }
    cLine = line();
    linend = cLine.p2();
    _arrowAngle = std::atan2(cLine.dy(), -cLine.dx());
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
    }
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void
SequenceConnectionItem::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print synchronous";
    auto arrowP1 = linend + QPointF(sin(_arrowAngle + M_PI - M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI - M_PI / 3) * scale);
    auto arrowP2 = linend + QPointF(sin(_arrowAngle + M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI / 3) * scale);
    _arrowHead << linend << arrowP1 << arrowP2;
    painter->setPen(QPen(QColor(0, 0, 0, 240), 1.0, Qt::SolidLine));
    painter->drawLine(line());
    painter->setBrush(_color);
    painter->drawPolygon(_arrowHead);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void
SequenceConnectionItem::paintAsynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print asynchronous";
    auto arrowP1 = linend + QPointF(sin(_arrowAngle + M_PI - M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI - M_PI / 3) * scale);
    auto arrowP2 = linend + QPointF(sin(_arrowAngle + M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI / 3) * scale);
    _arrowHead << linend << arrowP1 << linend << arrowP2 << linend;

    painter->setPen(QPen(QColor(0, 0, 0, 240), 1.0, Qt::SolidLine));
    painter->drawLine(line());
    painter->setBrush(_color);
    painter->drawPolygon(_arrowHead);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintReply(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print reply";
    qDebug() << "print asynchronous";
    auto arrowP1 = linend + QPointF(sin(_arrowAngle + M_PI - M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI - M_PI / 3) * scale);
    auto arrowP2 = linend + QPointF(sin(_arrowAngle + M_PI / 3) * scale,
                                    cos(_arrowAngle + M_PI / 3) * scale);
    auto l1 = QLineF(linend, arrowP1);
    auto l2 = QLineF(linend, arrowP2);


    QPen linepen(_color, 0.8, Qt::DashLine);
    linepen.setDashOffset(10);
    painter->setPen(linepen);
//    setPen(linepen);
    painter->drawLine(line());
    painter->drawLine(l1);
    painter->drawLine(l2);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintCreate(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print create";
    QPen linepen(_color, 1.1);
    linepen.setCapStyle(Qt::RoundCap);
    setPen(linepen);
    painter->drawLine(line());
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintDelete(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print delete";
//    QPen linepen(_color, 1.1);
//    linepen.setCapStyle(Qt::SquareCap);
    painter->setPen(QPen(_color, 1.1, Qt::SolidLine, Qt::RoundCap));
//    setPen(linepen);
    painter->drawLine(line());
}

