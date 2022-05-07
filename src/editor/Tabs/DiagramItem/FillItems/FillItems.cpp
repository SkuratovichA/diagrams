//
// Created by shchepa on 4.5.22.
//

#include "FillItems.h"

classParams::classParams(qreal x, qreal y, QString name,
                         QColor color, qreal width, qreal height,
                         QList<QString> attrs, QList<QString> methods)
                         : objectParams(x, y, name, color, width, height) {
    for (auto val : attrs) {
        _attrs.push_back(val);
    }
    for (auto val : methods) {
        _methods.push_back(val);
    }
};

actorParams::actorParams(qreal x, qreal y, QString name,
                         QColor color, qreal width, qreal height)
                         : objectParams(x, y, name, color, width, height) {

};