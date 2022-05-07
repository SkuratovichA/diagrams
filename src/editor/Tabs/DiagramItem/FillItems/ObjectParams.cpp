//
// Created by shchepa on 4.5.22.
//

#include "ObjectParams.h"
#include "../DiagramItem.h"


objectParams::objectParams(qreal x, qreal y, QString name,
                           QColor color, qreal width, qreal height,
                           ClassDiagramItem *classDiagramItemParent) {
    _x = x;
    _y = y;
    _color = color;
    _width = width;
    _height = height;
    _name = name;
    _classDiagramItemParent = classDiagramItemParent;
};

classParams::classParams(qreal x, qreal y, QString name,
                         QColor color, qreal width, qreal height,
                         QList<QString> attrs, QList<QString> methods)
        : objectParams(x, y, name, color, width, height) {
    for (auto val: attrs) {
        _attrs.push_back(val);
    }
    for (auto val: methods) {
        _methods.push_back(val);
    }
};

actorParams::actorParams(qreal x, qreal y, QString name, QColor color,
                         qreal width, qreal height,
                         ClassDiagramItem *classDiagramItemParent)
        : objectParams(x, y, name, color, width, height, classDiagramItemParent) {

};