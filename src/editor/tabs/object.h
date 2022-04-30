#ifndef Object_H
#define Object_H

#include <QString>
#include <QColor>
#include <QGraphicsItem>
#include <QApplication>
#include <QSet>
#include <QKeyEvent>

class Object : public QGraphicsItem
{
    Q_DECLARE_TR_FUNCTIONS(Object);

public:
    Object();

    void setText(const QString &text);
    QString text() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

////    void addLink(Link *link);
////    void removeLink(Link *link);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

protected:
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
//    QVariant itemChange(GraphicsItemChange change,
//                        const QVariant &value);

private:
    QRectF outlineRect() const;
//    int roundness(double size) const;

    //QSet<Link *> myLinks;
    QString myText;
    QColor myTextColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
};

#endif // Object_H
