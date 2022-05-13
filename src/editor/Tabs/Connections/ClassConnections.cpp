/** @file Connections.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 02.05.2022
 */


#define DEBUG 0

#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QStyle>
#include <QPainter>

#include "Connections.h"
#include "../DiagramItem/DiagramItem.h"
#include <QTextCursor>

ClassConnectionItem::ClassConnectionItem(ClassDiagramItem *fromNode,
                                         ClassDiagramItem *toNode,
                                         relationsParams *params,
                                         ClassConnectionType type,
                                         uint32_t order,
                                         QColor color
) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    _nodeFrom = fromNode;
    _nodeTo = toNode;
    _single = fromNode == toNode;
    _nodeFrom->addConnection(this, CommandType::Add, CommandType::Connection);
    _nodeTo->addConnection(this, CommandType::Add, CommandType::Connection);

    _connectionType = type;
    _color = color;
    _order = order;
    leftNum = new msgText(this, getFlags(), 0, 0, params->leftNum());
    msg = new msgText(this, getFlags(), 0, 0, params->msg());
    rightNum = new msgText(this, getFlags(), 0, 0, params->rightNum());
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
        return;
    }
}

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

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom + 10, yFrom - 50);
                rightNum->setPos(xTo - 50, yTo + 10);
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

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom + 10, yFrom - 50);
                rightNum->setPos(xTo - 50, yTo + 10);
            }
            break;
        case 3:
            yTo = _nodeTo->centre().y() + toHeightHalf;
            xTo = -(c + b * yTo) / a;
            if (xTo < _nodeTo->topLeft().x()) {
                xTo = _nodeTo->centre().x() + toWidthHalf;
                yTo = -(c + a * xTo) / b;
            }

            yFrom = _nodeFrom->centre().y() - fromHeightHalf;
            xFrom = -(c + b * yFrom) / a;
            if (xFrom < _nodeFrom->topLeft().x()) { // use ali
                xFrom = _nodeFrom->centre().x() - fromWidthHalf;
                yFrom = -(c + a * xFrom) / b;
            }

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom - 50, yFrom - 50);
                rightNum->setPos(xTo + 10, yTo + 10);
            }

            break;
        case 8:
            xTo = _nodeTo->centre().x() - toWidthHalf;
            yTo = -(c + a * xTo) / b; // using analytical geometry, compute the coords

            if (yTo < nodeTo()->topLeft().y()) {
                yTo = _nodeTo->centre().y() - toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }

            xFrom = _nodeFrom->centre().x() + fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomRight().y()) { // use #2
                yFrom = _nodeFrom->centre().y() + fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom + 10, yFrom + 15);
                rightNum->setPos(xTo - 50, yTo - 50);
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
            if (yFrom < _nodeFrom->topLeft().y()) {
                yFrom = _nodeFrom->centre().y() - fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom - 50, yFrom - 50);
                rightNum->setPos(xTo + 10, yTo + 10);
            }

            break;
        case 5:
            xTo = _nodeTo->centre().x() + toWidthHalf;
            yTo = -(c + a * xTo) / b;
            if (yTo < _nodeTo->topRight().y()) { // use as it is 6
                yTo = _nodeTo->centre().y() - toHeightHalf;
                xTo = -(c + b * yTo) / a;
            }

            xFrom = _nodeFrom->centre().x() - fromWidthHalf;
            yFrom = -(c + a * xFrom) / b;
            if (yFrom > _nodeFrom->bottomLeft().y()) {
                yFrom = _nodeFrom->centre().y() + fromHeightHalf;
                xFrom = -(c + b * yFrom) / a;
            }

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom - 50, yFrom + 15);
                rightNum->setPos(xTo + 10, yTo - 50);
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

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom - 50, yFrom + 15);
                rightNum->setPos(xTo - 10, yTo - 50);
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

            if (_single) {
                leftNum->setPos(xFrom + 40, yFrom + 60);
                rightNum->setPos(xTo + 80, yTo + 10);
            } else {
                leftNum->setPos(xFrom + 10, yFrom + 15);
                rightNum->setPos(xTo - 50, yTo - 50);
            }

            break;
    }
    return {QPointF(xFrom, yFrom), QPointF(xTo, yTo)};
}

void ClassConnectionItem::trackNodes() {
    auto edgePs = edgePoints();
    if (!_orientation) {
        _orientation = !_orientation;
        std::swap(_nodeFrom, _nodeTo);
    }
    setLine(QLineF(edgePs.first, edgePs.second));
}

QPolygonF ClassConnectionItem::lineShaper() const {
    QPolygonF poly;
    if (_single) {
        QPolygonF poly;
        auto margin = std::min<qreal>(_nodeTo->height(), _nodeTo->width()) / 3;
        auto yAbove = _nodeTo->bottomRight().y() - margin - 17;
        auto xRight = _nodeTo->bottomRight().x() + margin + 17;
        auto xLeft = _nodeTo->bottomRight().x() - margin - 17;
        auto yBelow = _nodeTo->bottomRight().y() + margin + 17;
        auto x = _nodeTo->bottomRight().x();
        auto y = _nodeTo->bottomRight().y();
        poly << QPointF(x, yAbove) << QPointF(xRight, yAbove)
             << QPointF(xRight, yBelow) << QPointF(xLeft, yBelow) << QPoint(xLeft, y) << _nodeTo->bottomRight()
             << QPointF(x, yAbove);
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
    }
    return poly;
#undef ovlp
}

QPainterPath ClassConnectionItem::shape() const {
    QPainterPath path;
    path.addPolygon(lineShaper());
    return path;
}

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
        // Single connection line
        auto margin = std::min<qreal>(_nodeTo->height(), _nodeTo->width()) / 3;
        auto yAbove = _nodeTo->bottomRight().y() - margin;
        auto xRight = _nodeTo->bottomRight().x() + margin;
        auto xLeft = _nodeTo->bottomRight().x() - margin;
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
        angle = -M_PI / 2;
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
            poly << linend << arrowP1 << arrowP3 << arrowP2;
            break;
        case Composition:
            poly << linend << arrowP1 << arrowP3 << arrowP2;
            break;
        case Dependency:
            painter->setPen(QPen(_color, 2, Qt::DashLine));
            clr = QColor(Qt::white);
            poly << linend << arrowP1 << arrowP2;
            break;
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

void ClassConnectionItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget) {
#if DEBUG
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0.5, Qt::DotLine));
    painter->drawPolygon(lineShaper());
#endif
    drawLine(painter, option);
    QPointF pText = (_nodeTo->socket(order()) + _nodeFrom->socket(order())) / 2;
    QPair<QPointF, QPointF> ep = edgePoints();

    qreal widthText = getMsg()->boundingRect().width();
    qreal widthLeft = getLeftNum()->boundingRect().width();
    qreal widthRight = getRightNum()->boundingRect().width();

    QPointF pLeft = QPointF(
            ep.first.x() + _nodeFrom->width() + 20,
            _nodeFrom->pos().y() + _nodeFrom->height() / 2.0 + 20);
    QPointF pRight = QPointF(
            _nodeTo->pos().x() - 20,
            _nodeTo->pos().y() + _nodeTo->height() / 2.0 + 20);

    if (_single) {
        leftSetPos(QPointF(_nodeTo->pos().x() + _nodeTo->width() - 86,
                           _nodeTo->pos().y() + _nodeTo->height() + 20));
        msgSetPos(QPointF(pText.x() - widthText / 2.0 + _nodeTo->width() - 55,
                          pText.y() + _nodeTo->height() - 40));
        rightSetPos(QPointF(_nodeFrom->pos().x() + _nodeFrom->width() + 10,
                            _nodeFrom->pos().y() + _nodeFrom->height() - 65));
    } else {
        msgSetPos(QPointF(pText.x() - widthText / 2.0, pText.y() - 30));
    }
}

ClassConnectionItem::~ClassConnectionItem() {
}
void ClassConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        setSelected(true);
    }
}