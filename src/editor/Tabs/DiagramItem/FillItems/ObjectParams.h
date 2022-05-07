//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_OBJECTPARAMS_H
#define DIAGRAMS_OBJECTPARAMS_H

#include <QString>
#include <QList>
#include <QColor>

class objectParams {
public:
    objectParams(qreal x, qreal y, QString name,
                 QColor color, qreal width, qreal height) {
        _x = x;
        _y = y;
        _color = color;
        _width = width;
        _height = height;
        _name = name;
    };
    ~objectParams() = default;

    qreal x() const {
        return _x;
    }

    qreal y() const {
        return _y;
    }

    QColor color() const {
        return _color;
    }

    qreal width() const {
        return _width;
    }

    qreal height() const {
        return _height;
    }

    QString name() const {
        return _name;
    }
protected:
    qreal _x;
    qreal _y;
    QColor _color;
    qreal _width;
    qreal _height;
    QString _name;
};

class classParams : public objectParams {
public:
    classParams(qreal x, qreal y, QString name,
                QColor color, qreal width, qreal height,
                QList<QString> attrs, QList<QString> methods);
    ~classParams() = default;

    QList<QString> attrs() const {
        return _attrs;
    }

    QList<QString> methods() const {
        return _methods;
    }

private:
    QList<QString> _attrs;
    QList<QString> _methods;
};

class actorParams : public objectParams {
public:
    actorParams(qreal x, qreal y, QString name,
                QColor color, qreal width, qreal height);
    ~actorParams() = default;
};


#endif //DIAGRAMS_OBJECTPARAMS_H
