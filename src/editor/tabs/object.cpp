#include "object.h"

#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Object::Object() {
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
}

void Object::setText(const QString &text) {
    prepareGeometryChange();
    myText = text;
    update();
}

QString Object::text() const {
    return myText;
}

void Object::setTextColor(const QColor &color) {
    myTextColor = color;
    update();
}

QColor Object::textColor() const {
    return myTextColor;
}

void Object::setOutlineColor(const QColor &color) {
    myOutlineColor = color;
    update();
}

QColor Object::outlineColor() const {
    return myOutlineColor;
}

void Object::setBackgroundColor(const QColor &color) {
    myBackgroundColor = color;
    update();
}

QColor Object::backgroundColor() const {
    return myBackgroundColor;
}

QRectF Object::outlineRect() const {
    const int Padding = 8;
    QFontMetricsF metrics{qApp->font()};
    QRectF rect = metrics.boundingRect(myText);

    rect.adjust(-Padding, -Padding, +Padding, +Padding);

    rect.translate(-rect.center());

    return rect;
}

QRectF Object::boundingRect() const {
    const int Margin = 1;
    return QRectF(0, 0, 200, 200);
}

void Object::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        this->setScale(this->scale() * 1.2);
    }
}

void Object::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QString text = QInputDialog::getText(event->widget(),
                                         tr("Edit Text"), tr("Enter new text:"),
                                         QLineEdit::Normal, myText);


    prepareGeometryChange();
    myText = text;
    update();

}

QPainterPath Object::shape() const {
    QRectF rect = this->boundingRect();

    QPainterPath path;
    path.addRoundedRect(rect, rect.width() - 2, rect.height() - 2, Qt::RelativeSize);
    return path;
}

void Object::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget * /* widget */) {
    qDebug() << this->x() << this->y() << option->state;
    QPen pen(myOutlineColor);

    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(4);
    }

    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    painter->drawEllipse(30, 30, 45, 45);
    painter->drawLine(53, 75, 53, 140);
    painter->drawLine(53, 85, 80, 110);
    painter->drawLine(53, 85, 30, 110);
    painter->drawLine(53, 140, 80, 170);
    painter->drawLine(53, 140, 30, 170);
}
