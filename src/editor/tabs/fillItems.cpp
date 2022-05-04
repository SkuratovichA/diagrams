//
// Created by shchepa on 4.5.22.
//

#include "fillItems.h"

objectParams::objectParams(qreal x, qreal y, qreal scale, QString name,
        QColor color, qreal width, qreal height) {

    _x = x;
    _y = y;
    _scale = scale;
    _color = color;
    _width = width;
    _height = height;
    _name = name;
};

classParams::classParams(qreal x, qreal y, qreal scale, QString name,
                         QColor color, qreal width, qreal height,
                         QList<QString> attrs, QList<QString> methods)
                         : objectParams(x, y, scale, name, color, width, height) {

    for (auto val : attrs) {
        _attrs.push_back(val);
    }
    for (auto val : methods) {
        _methods.push_back(val);
    }
};

actorParams::actorParams(qreal x, qreal y, qreal scale, QString name,
                         QColor color, qreal width, qreal height)
                         : objectParams(x, y, scale, name, color, width, height) {

};