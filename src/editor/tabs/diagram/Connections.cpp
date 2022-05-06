// File: Connections.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 02.05.2022

#define DEBUG 1

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QStyle>
#include <QPainter>

#include "Connections.h"
#include "DiagramItem.h"

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
ClassConnectionItem::ClassConnectionItem(ClassDiagramItem *fromNode,
                                         ClassDiagramItem *toNode,
                                         ClassConnectionType connectionType,
                                         uint32_t order,
                                         QColor color
) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    _nodeFrom = fromNode;
    _nodeTo = toNode;
    _single = fromNode == toNode;

    _nodeFrom->addConnection(this);
    _nodeTo->addConnection(this);
    _connectionType = connectionType;
    _color = color;
    _order = order;

    msg = new msgText(this, getFlags(), 20, -40, "MSG");

    setZValue(-1.0);
    trackNodes();
}

msgText::msgText(QGraphicsItem *parent, QFlags<Qt::TextInteractionFlag> flags, qreal x, qreal y, QString str)
 : QGraphicsTextItem(str, parent) {
    _parent = parent;
    setPos(x, y);
    setFont(QFont("Courier", 10));
    setTextInteractionFlags(flags);
    topLevelItem();
}

void msgText::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        setPlainText(toPlainText().remove('\n'));
        clearFocus();
        return;
    }
}

/**
 *
 * @param x1y1
 * @param x2y2
 * @param collision
 * @return
 */
inline int getOctant(QPointF const &x1y1, QPointF const &x2y2, bool *collision = nullptr) {
    auto x2 = x2y2.x() - x1y1.x(); // x2
    auto y2 = x2y2.y() - x1y1.y(); // y2
    qreal x1 = 0.0;
    qreal y1 = 0.0;

    x1 = y1 = 0; // x1
    if (x2 < x1) { // 3..6
        if (y2 < y1) { // 3,4
            if (collision != nullptr && x2 == y2) {
                *collision = true;
            }
            return x2 > y2 ? 3 : 4;
        } else {
            if (collision != nullptr && -x2 == y2) {
                *collision = true;
            }
            return -x2 > y2 ? 5 : 6;
        }
    } else { // 2..8
        if (y2 < y1) { // 1,2
            if (collision != nullptr && -x2 == y2) {
                *collision = true;
            }
            return -x2 >= y2 ? 2 : 1;
        } else { // 7,8
            if (collision != nullptr && x2 == y2) {
                *collision = true;
            }
            return x2 < y2 ? 7 : 8;
        }
    }
}

/**
 *
 * @return
 */
QPair<QPointF, QPointF> ClassConnectionItem::edgePoints() const {
    auto xyFrom = _nodeFrom->socket(_order);
    auto xyTo = _nodeTo->socket(_order);

    auto xFrom = xyFrom.x();
    auto yFrom = xyFrom.y();
    auto xTo = xyTo.x();
    auto yTo = xyTo.y();

    auto octant = getOctant(_nodeFrom->centre(), _nodeTo->centre());
    auto a = yTo - yFrom;
    auto b = xFrom - xTo;
    auto c = -(a * xFrom + b * yFrom);

    auto toHeightHalf = _nodeTo->height() / 2;
    auto toWidthHalf = _nodeTo->width() / 2;
    auto fromHeightHalf = _nodeFrom->height() / 2;
    auto fromWidthHalf = _nodeFrom->width() / 2;
    switch (octant) {
        case 1:
            // try to compute interseptions with boxes edges.
            xTo = _nodeTo->centre().x() - toWidthHalf;
            yTo = -(c + a * xTo) / b;
            // if a point is not placed to the centre of the box
            // compute interseptions like there was #2 octant.
            // Basically, the idea is the same for all the octants.
            // If one octant cannot find the interseptions. E.g. line is found using the formula,
            // the interseption point does not lie on the edge we want ->
            // use the same formula for finding an interseption point for n+1 octant
            if (yTo > _nodeTo->bottomLeft().y()) {
                yTo = _nodeTo->centre().y() + toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }
            xFrom = _nodeFrom->centre().x() + fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomRight().y()) {
                yFrom = _nodeFrom->centre().y() + fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            } else if (yFrom < _nodeFrom->topRight().y()) {
                yFrom = _nodeFrom->centre().y() - fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }
            break;
        case 2:
            yTo = _nodeTo->centre().y() + toHeightHalf;
            xTo = -(c + b * yTo) / a;
            if (xTo < _nodeTo->topLeft().x()) {
                xTo = _nodeTo->centre().x() - toWidthHalf;
                yTo = -(c + a * xTo) / b; // using analytical geometry, compute the coords
            }

            yFrom = _nodeFrom->centre().y() - fromHeightHalf;
            xFrom = -(c + b * yFrom) / a;
            if (xFrom > _nodeFrom->topRight().x()) { // work as with an octant #1
                xFrom = _nodeFrom->centre().x() + fromWidthHalf;
                yFrom = -(c + a * xFrom) / b;
            }
            break;
        case 3:
            yTo = _nodeTo->centre().y() + toHeightHalf;
            xTo = -(c + b * yTo) / a;
            if (xTo < _nodeTo->topLeft().x()) {
                qDebug() << "lala";
                xTo = _nodeTo->centre().x() + toWidthHalf;
                yTo = -(c + a * xTo) / b;
            }

            yFrom = _nodeFrom->centre().y() - fromHeightHalf;
            xFrom = -(c + b * yFrom) / a;
            if (xFrom < _nodeFrom->topLeft().x()) { // use ali
                xFrom = _nodeFrom->centre().x() - fromWidthHalf;
                yFrom = -(c + a * xFrom) / b;
            }
            break;
        case 8:
            xTo = _nodeTo->centre().x() - toWidthHalf;
            yTo = -(c + a * xTo) / b; // using analytical geometry, compute the coords
            if (octant == 1 && yTo > _nodeTo->bottomLeft().y()) {
                yTo = _nodeTo->centre().y() + toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }

            xFrom = _nodeFrom->centre().x() + fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomRight().y()) { // use #2
                qDebug() << "jebe";
                yFrom = _nodeFrom->centre().y() + fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }
            break;
        case 4:
            xTo = _nodeTo->centre().x() + toWidthHalf;
            yTo = -(c + a * xTo) / b;
            if (yTo > _nodeTo->bottomRight().y()) {
                yTo = _nodeTo->centre().y() + toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }

            xFrom = _nodeFrom->centre().x() - fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomLeft().y()) {
                yFrom = _nodeFrom->centre().y() - fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }
            break;
        case 5:
            xTo = _nodeTo->centre().x() + toWidthHalf;
            yTo = -(c + a * xTo) / b;
            if (yTo < _nodeTo->topRight().y()) { // use as it is 6
                qDebug() << "5 thinks she is 6";
                yTo = _nodeTo->centre().y() - toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }

            xFrom = _nodeFrom->centre().x() - fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomLeft().y()) {
                yFrom = _nodeFrom->centre().y() + fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }
            break;
        case 6:
            yTo = _nodeTo->centre().y() - toHeightHalf;
            xTo = -(c + b * yTo) / a;
            if (xTo > _nodeTo->topRight().x()) { // 6 thinks it is #5
                xTo = _nodeTo->centre().x() + toWidthHalf;
                yTo = -(c + a * xTo) / b;
            }
            yFrom = _nodeFrom->centre().y() + fromHeightHalf;
            xFrom = -(c + b * yFrom) / a;
            if (xFrom < _nodeFrom->topLeft().x()) {
                xFrom = _nodeFrom->centre().x() - fromWidthHalf;
                yFrom = -(c + a * xFrom) / b;
            }
            break;
        case 7:
            yTo = _nodeTo->centre().y() - toHeightHalf;
            xTo = -(c + b * yTo) / a;
            if (xTo < _nodeTo->topLeft().x()) {
                xTo = _nodeTo->centre().x() - toWidthHalf;
                yTo = -(c + a * xTo) / b;
            }
            yFrom = _nodeFrom->centre().y() + fromHeightHalf;
            xFrom = -(c + b * yFrom) / a;
            if (xFrom < _nodeFrom->topLeft().x()) {
                xFrom = _nodeFrom->centre().x() - fromWidthHalf;
                yFrom = -(c + a * xFrom) / b;
            }
            break;
    }
    return {QPointF(xFrom, yFrom), QPointF(xTo, yTo)};
}

/**
 *
 */
void ClassConnectionItem::trackNodes() {
    auto edgePs = edgePoints();
    if (_orientation) {
        setLine(QLineF(edgePs.first, edgePs.second));
    } else {
        setLine(QLineF(edgePs.second, edgePs.first));
    }
}

/**
 *
 * @return
 */
QPolygonF ClassConnectionItem::lineShaper() const {
    QPolygonF poly;
    if (_single) {
        QPolygonF poly;
        auto margin = std::min<qreal>(_nodeTo->height(), _nodeTo->width())/3;
        auto yAbove = _nodeTo->bottomRight().y() - margin    - 17;
        auto xRight = _nodeTo->bottomRight().x() + margin    + 17;
        auto xLeft = _nodeTo->bottomRight().x()  - margin    - 17;
        auto yBelow = _nodeTo->bottomRight().y() + margin    + 17;
        auto x = _nodeTo->bottomRight().x();
        auto y = _nodeTo->bottomRight().y();
        poly << QPointF(x, yAbove) << QPointF(xRight, yAbove)
             << QPointF(xRight, yBelow) << QPointF(xLeft, yBelow) << QPoint(xLeft, y) << _nodeTo->bottomRight() << QPointF(x, yAbove);
        return poly;
    }

    auto const fromUpperLeft = _nodeFrom->pos();
    auto const fromUpperRight = fromUpperLeft + QPointF(_nodeFrom->width(), 0);
    auto const fromLowerLeft = fromUpperLeft + QPointF(0, _nodeFrom->height());
    auto const fromLowerRight = fromLowerLeft + QPointF(_nodeFrom->width(), 0);

    auto const toUpperLeft = _nodeTo->pos();
    auto const toUpperRight = toUpperLeft + QPointF(_nodeTo->width(), 0);
    auto const toLowerLeft = toUpperLeft + QPointF(0, _nodeTo->height());
    auto const toLowerRight = toUpperRight + QPointF(0, _nodeTo->height());

#define ovlp(point) \
        (fromUpperLeft.x() < (point).x() && fromUpperLeft.y() < (point).y() \
        && fromUpperRight.x() > (point).x() && fromUpperRight.y() < (point).y() \
        && fromLowerLeft.y() > (point).y())

    auto ovlpEdges = ovlp(toUpperLeft) + ovlp(toUpperRight) + ovlp(toLowerLeft) + ovlp(toLowerRight);

    auto boundSize = 17.0;

    QPointF bLeft(std::min<qreal>(toUpperLeft.x(), fromUpperLeft.x()) - boundSize,
                  std::min<qreal>(toUpperLeft.y(), fromUpperLeft.y()) - boundSize);
    QPointF bRight(std::max<qreal>(toLowerRight.x(), fromLowerRight.x()) + boundSize,
                   std::max<qreal>(toLowerRight.y(), fromLowerRight.y()) + boundSize);

    QPointF topAdjuster;
    QPointF bottomAdjuster;
    bool collision = false;
    auto const octant = getOctant(_nodeFrom->socket(_order), _nodeTo->socket(_order), &collision);
    switch (ovlpEdges) {
        case 4:
        case 3:
        case 2:
        case 1: // only one overlapped edge - do not print bb
            poly << QPolygonF(QRectF(bLeft, bRight)); // bounding rectangle
            break;
        default:
            auto ePs = edgePoints();
            auto const farLeft = ePs.first;
            auto const farRight = ePs.second;

            switch (octant) {
                case 1:
                case 8:
                    if (collision) {
                        bottomAdjuster = QPointF(0, -boundSize);
                        topAdjuster = QPointF(-boundSize, 0);
                    } else {
                        topAdjuster = QPointF(-boundSize, boundSize);
                        bottomAdjuster = QPointF(-boundSize, -boundSize);
                    }
                    break;
                case 4:
                case 5:
                    if (collision) {
                        topAdjuster = QPointF(0, +boundSize);
                        bottomAdjuster = QPointF(+boundSize, +0);
                    } else {
                        topAdjuster = QPointF(boundSize, boundSize);
                        bottomAdjuster = QPointF(boundSize, -boundSize);
                    }
                    break;
                case 2:
                case 3:
                    if (collision) {
                        topAdjuster = QPointF(-boundSize, 0);
                        bottomAdjuster = QPointF(0, boundSize);
                    } else {
                        topAdjuster = QPointF(-boundSize, boundSize);
                        bottomAdjuster = QPointF(boundSize, boundSize);
                    }
                    break;
                case 6:
                case 7:
                    if (collision) {
                        topAdjuster = QPointF(0, -boundSize);
                        bottomAdjuster = QPointF(0, boundSize);
                    } else {
                        topAdjuster = QPointF(-boundSize, -boundSize);
                        bottomAdjuster = QPointF(boundSize, -boundSize);
                    }
                    break;
            }

            poly << farLeft + topAdjuster << farLeft + bottomAdjuster << farRight - topAdjuster
                 << farRight - bottomAdjuster;
            // **************
    }
    return poly;
#undef ovlp
}

/**
 *
 * @return
 */
QPainterPath ClassConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

/**
 *
 */
void ClassConnectionItem::drawLine(QPainter *painter, const QStyleOptionGraphicsItem *option) const {
    painter->setPen(QPen(_color, 2, Qt::SolidLine));
    painter->setRenderHint(QPainter::Antialiasing, true);
    auto points = edgePoints();
    QPolygonF poly;
    poly.clear();
    auto cLine = line();
    QColor clr(_color);
    QPointF linend;

    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(_color.darker(), 2.5, Qt::SolidLine));
    }
    QPointF arrowP1;
    QPointF arrowP2;
    QPointF arrowP3;
    qreal angle;
    QLineF sl1;
    QLineF sl2;
    QLineF sl3;
    QLineF sl4;

    if (!_single) {
        angle = std::atan2(-cLine.dy(), cLine.dx());
        linend = cLine.p1();
    } else {
        auto margin = std::min<qreal>(_nodeTo->height(), _nodeTo->width())/3;
        auto yAbove = _nodeTo->bottomRight().y() - margin;
        auto xRight = _nodeTo->bottomRight().x() + margin;
        auto xLeft = _nodeTo->bottomRight().x()  - margin;
        auto yBelow = _nodeTo->bottomRight().y() + margin;
        auto x = _nodeTo->bottomRight().x();
        auto y = _nodeTo->bottomRight().y();
        sl1 = QLineF(QPointF(x, yAbove),
                     QPointF(xRight, yAbove));
        sl2 = QLineF(QPointF(xRight, yAbove),
                     QPointF(xRight, yBelow));
        sl3 = QLineF(QPointF(xRight, yBelow),
                     QPointF(xLeft, yBelow));
        sl4 = QLineF(QPointF(xLeft, yBelow),
                     QPointF(xLeft, y));
        angle = -M_PI/2;
        linend = QPointF(xLeft, y);
    }
    auto const scale = 20;
    arrowP1 = linend + QPointF(sin(angle + M_PI - M_PI / 3) * scale,
                               cos(angle + M_PI - M_PI / 3) * scale);
    arrowP2 = linend + QPointF(sin(angle + M_PI / 3) * scale,
                               cos(angle + M_PI / 3) * scale);
    arrowP3 = linend + QPointF(sin(angle + M_PI / 2) * scale * 1.7,
                                   cos(angle + M_PI / 2) * scale * 1.7);

    switch (_connectionType) {
        case Association:
            break;
        case Aggregation:
            clr = QColor(Qt::white);
        case Composition:
            poly << linend << arrowP1 << arrowP3 << arrowP2;
            break;
        case Dependency:
            painter->setPen(QPen(_color, 2, Qt::DashLine));
            clr = QColor(Qt::white);
        case Generalization:
            poly << linend << arrowP1 << arrowP2;
            break;
    }
    painter->setBrush(clr);
    if (_single) {
        painter->drawLine(sl1);
        painter->drawLine(sl2);
        painter->drawLine(sl3);
        painter->drawLine(sl4);
        painter->drawPolygon(poly);
    } else {
        painter->drawLine(cLine);
    }
    painter->drawPolygon(poly);
}

/**
 *
 * @param painter
 * @param option
 * @param widget
 */
void ClassConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    drawLine(painter, option);
    auto p = (_nodeTo->socket(order()) + _nodeFrom->socket(order()))/2;
    QGraphicsItem *text = this->childItems().first();
    qreal widthText = text->boundingRect().width();
    text->setPos(p.x() - widthText/2.0, p.y() - 30);
}

/**
 *
 */
ClassConnectionItem::~ClassConnectionItem() {
    qDebug() << "removing connections from 'fromNode' (Connections.cpp)";
    if (_nodeFrom != nullptr) {
        _nodeFrom->removeConnection(this);
    }
    qDebug() << "\tremoved";

    qDebug() << "removing connections from 'toNode' (Conneictions.cpp)";
    if (_nodeTo != nullptr) {
        _nodeTo->removeConnection(this);
    }
    qDebug() << "\tremoved";
}

/**
 *
 * @param fromNode
 * @param toNode
 * @param connectionType
 */
ActorConnectionItem::ActorConnectionItem(SequenceDiagramItem *fromNode,
                                         SequenceDiagramItem *toNode,
                                         ActorConnectionType
                                         connectionType){
    nodeFrom = fromNode;
    nodeTo = toNode;

    nodeFrom->addConnection(this);
    nodeTo->addConnection(this);
    //setPen(QPen(Qt::black));
    auto wid1 = nodeFrom->width()/2.0;
    auto wid2 = nodeTo->width()/2.0;
    //auto line = QGraphicsLineItem(wid1, 400, wid2, 400, this);
    //setLine(wid1, 200, wid2, 200);
    //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    //setZValue(-1.0);
    
    //trackNodes();
}

QVariant ActorConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange)
        return QPointF(pos().x(), value.toPointF().y());
    return QGraphicsItem::itemChange( change, value );
}

void ActorConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawLine(nodeFrom->width()/2.0, y(), nodeTo->width()/2.0, y());
    qDebug() << "paint" << nodeFrom->width()/2.0 << y() << nodeTo->width()/2.0 << y();
}

/**
 *
 */
ActorConnectionItem::~ActorConnectionItem() {
    if (nodeFrom != nullptr) {
        nodeFrom->removeConnection(this);
    }
    if (nodeTo != nullptr) {
        nodeTo->removeConnection(this);
    }
}

///**
// *
// * @param color
// */
//void ActorConnectionItem::setColor(const QColor &color) {
//    setPen(QPen(color, 1.0));
//}

/**
 *
 * @return
 */
QColor ActorConnectionItem::color() const {
    return pen().color();
}

/**
 *
 */
void ActorConnectionItem::trackNodes() {
    setLine(QLineF(nodeFrom->pos(), nodeTo->pos()));
}

/**
 *
 * @return
 */
SequenceDiagramItem *ActorConnectionItem::fromNode() const {
    return nodeFrom;
}

/**
 *
 * @return
 */
SequenceDiagramItem *ActorConnectionItem::toNode() const {
    return nodeTo;
}

void ClassConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}
