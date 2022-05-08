//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_OBJECTPARAMS_H
#define DIAGRAMS_OBJECTPARAMS_H

#include <QString>
#include <QList>
#include <QColor>
#include "../../Backend/Parse.h"

class relationsParams {
public:
    relationsParams(QString leftObj, QString leftNum,
                    QString rightObj, QString rightNum,
                    QString msg, int type) {
        _leftObj = leftObj;
        _leftNum = leftNum;
        _rightObj = rightObj;
        _rightNum = rightNum;
        _msg = msg;
        _type = type;
    };

private:
    QString _leftObj;
    QString _leftNum;
    QString _rightObj;
    QString _rightNum;
    int _type;
    QString _msg;
};

class objectParams {
public:
    objectParams(qreal x, qreal y, QString name,
                 QColor color, qreal width, qreal height);
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

    void fillColor(Color &c) {
        c.r = color().red();
        c.g = color().green();
        c.b = color().blue();
        c.a = color().alpha();
    }

    void fillCoords(std::vector<double> &v) {
        v.push_back(x());
        v.push_back(y());
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

    bool splitString(std::vector<attrs_t> &at, QList<QString> arr);

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
