// File: ObjectParams.h
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#ifndef DIAGRAMS_OBJECTPARAMS_H
#define DIAGRAMS_OBJECTPARAMS_H

#include <QString>
#include <QList>
#include <QColor>
#include <QWidget>
#include "../../Backend/Parse.h"

class relationsParams {
public:
    relationsParams(QString leftObj, QString leftNum,
                    QString rightObj, QString rightNum,
                    QString msg, int type, int order) {
        _leftObj = leftObj;
        _leftNum = leftNum;
        _rightObj = rightObj;
        _rightNum = rightNum;
        _msg = msg;
        _type = type;
        _order = order;
    };

    QString leftObj() {
        return _leftObj;
    }

    QString leftNum() {
        return _leftNum;
    }

    QString rightObj() {
        return _rightObj;
    }

    QString rightNum() {
        return _rightNum;
    }

    int order() {
        return _order;
    }

    int type() {
        return _type;
    }

    QString msg() {
        return _msg;
    }

private:
    QString _leftObj;
    QString _leftNum;
    QString _rightObj;
    QString _rightNum;
    int _type;
    QString _msg;
    int _order;
};

class objectParams {
public:
    objectParams(qreal x, qreal y, QString name, QColor color);
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
    actorParams(qreal x, qreal y, QString name, QColor color);
    //~actorParams() = default;
};

class messageParams {
public:
    messageParams(qreal x, qreal y, QString msg, QString nameFrom, QString nameTo, int type);

    qreal x() {
        return _x;
    }

    qreal y() {
        return _y;
    }

    QString msg() {
        return _msg;
    }

    QString nameFrom() {
        return _nameFrom;
    }

    QString nameTo() {
        return _nameTo;
    }

    void fillCoords(std::vector<double> &v) {
        v.push_back(x());
        v.push_back(y());
    }

    int type() {
        return _type;
    }


private:
    qreal _x;
    qreal _y;
    QString _msg;
    QString _nameFrom;
    QString _nameTo;
    int _type;
};


#endif //DIAGRAMS_OBJECTPARAMS_H
