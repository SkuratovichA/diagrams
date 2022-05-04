//
// Created by shchepa on 4.5.22.
//

#include "fillItems.h"

classParams::classParams(qreal x, qreal y, qreal scale, QList<QString> attrs,
            QList<QString> methods, QString name) {

    _x = x;
    _y = y;
    _scale = scale;
    for (auto val : attrs) {
        _attrs.push_back(val);
    }
    for (auto val : methods) {
        _methods.push_back(val);
    }
    _name = name;
};