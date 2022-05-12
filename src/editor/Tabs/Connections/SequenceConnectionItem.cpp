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
                                               ConnectionType connectionType
) {
    _nodeFrom = nodeFrom;
    _nodeTo = nodeTo;
    _connectionType = connectionType;

    //qDebug() << __FILE__;
    qDebug() << "   creating a connection" << _nodeFrom << _nodeTo;
    _nodeFrom->addConnection(this, Caller);
    _nodeTo->addConnection(this, Receiver);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    setZValue(1.0);
    setY(200);

    trackNodes();
    //qDebug() << "one connection";
    //qDebug() << __FILE__ << "   connection created>";
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
//     _nodeFrom->trackNodes();
//     _nodeTo->trackNodes();
}

/**
 *
 * @return
 */
QRectF SequenceConnectionItem::lineShaper() const {
    //qDebug() << "line shaper";
    auto rect = QRectF(
            QPointF(std::min(_nodeTo->centre().x() - 30, _nodeFrom->centre().x() - 30), -20),
            QPointF(std::max(_nodeTo->centre().x() + 30, _nodeFrom->centre().x() + 30), 20)
    );
    //qDebug() << "rect = " << rect;
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
    painter->drawPolygon(lineShaper());
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

//    if (option->state & QStyle::State_Selected) {
//        painter->setPen(QPen(_color.darker(), 2.5, Qt::SolidLine));
//    }
    //setPen()
    painter->setRenderHint(QPainter::Antialiasing, true);

    //_arrowAngle = std::atan2(cLine.dy(), -cLine.dx());
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
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void SequenceConnectionItem::paintSynchronous(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug() << "print synchronous";
    QPolygonF _arrowHead_p;

    cLine = line();

    if (cLine.p1().x() < cLine.p2().x()) {
        qDebug() << "p1 < p2";
        cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() - 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() - 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() - 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
    }
    else {
        qDebug() << "p2 < p1";
        cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() + 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() + 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() + 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
    }

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);

    QPolygonF arrowHead;
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
    qDebug() << "print asynchronous";
    QPolygonF _arrowHead_p;
    cLine = line();

    if (cLine.p1().x() < cLine.p2().x()) {
        qDebug() << "p1 < p2";
        cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() - 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() - 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() - 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
    }
    else {
        qDebug() << "p2 < p1";
        cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() + 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() + 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() + 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
    }

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);

    QPolygonF arrowHead;
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
    qDebug() << "print reply";
    qDebug() << "print asynchronous";

    cLine = line();

    if (cLine.p1().x() < cLine.p2().x()) {
        qDebug() << "p1 < p2";
        cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() - 10, cLine.p2().y()));

        pUp = QPointF(cLine.p1().x() + 15, cLine.p1().y() + 8);
        pDown = QPointF(cLine.p1().x() + 15, cLine.p1().y() - 8);
        pEnd = cLine.p1();
    }
    else {
        qDebug() << "p2 < p1";
        cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() + 10, cLine.p2().y()));

        pUp = QPointF(cLine.p1().x() - 15, cLine.p1().y() + 8);
        pDown = QPointF(cLine.p1().x() - 15, cLine.p1().y() - 8);
        pEnd = cLine.p1();
    }

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 20) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::DashLine));
    painter->drawLine(cLine);

    QPolygonF arrowHead;
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
    qDebug() << "print create";
    QPolygonF _arrowHead_p;
    cLine = line();

    if (cLine.p1().x() < cLine.p2().x()) {
        qDebug() << "p1 < p2";
        cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() - 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() - 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() - 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
        posX = pEnd.x();
    }
    else {
        qDebug() << "p2 < p1";
        cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() + 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() + 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() + 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
        posX = pEnd.x() - 20;
    }

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
    qDebug() << "print delete";
    QPolygonF _arrowHead_p;
    cLine = line();

    if (cLine.p1().x() < cLine.p2().x()) {
        qDebug() << "p1 < p2";
        cLine = QLineF(QPointF(cLine.p1().x() + 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() - 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() - 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() - 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
        posX = pEnd.x();
    }
    else {
        qDebug() << "p2 < p1";
        cLine = QLineF(QPointF(cLine.p1().x() - 10, cLine.p1().y()),
                       QPointF(cLine.p2().x() + 10, cLine.p2().y()));

        pUp = QPointF(cLine.p2().x() + 15, cLine.p2().y() + 8);
        pDown = QPointF(cLine.p2().x() + 15, cLine.p2().y() - 8);
        pEnd = cLine.p2();
        posX = pEnd.x() - 20;
    }

    if (std::abs(cLine.p1().x() - cLine.p2().x()) < 15) {
        return;
    }

    painter->setPen(QPen(QColor(Qt::red), 1.0, Qt::SolidLine));
    painter->drawLine(cLine);
    painter->setPen(QPen(QColor(Qt::black), 1.0, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(posX, pEnd.y() - 7, 20, 14);
}

