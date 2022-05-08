//
// Created by shchepa on 4.5.22.
//

#include "ObjectParams.h"

objectParams::objectParams(qreal x, qreal y, QString name, QColor color)
{
    _x = x;
    _y = y;
    _color = color;
    _name = name;
};

classParams::classParams(qreal x, qreal y, QString name,
                         QColor color, qreal width, qreal height,
                         QList<QString> attrs, QList<QString> methods)
        : objectParams(x, y, name, color) {
    _width = width;
    _height = height;

    for (auto val: attrs) {
        _attrs.push_back(val);
    }
    for (auto val: methods) {
        _methods.push_back(val);
    }
};

bool classParams::splitString(std::vector<attrs_t> &at, QList<QString> arr) {
    std::string perm;
    std::string type;
    std::string name;
    std::string textStd;
    char *token;

    for (auto x : arr) {
        textStd = x.toStdString();
        if (textStd.length() < 2) {
            return false;
        }

        token = strtok(const_cast<char*>(textStd.c_str()), " ");
        perm = std::string(token);
        if (token == NULL || ( perm != "#" && perm != "+"
                               && perm != "~" && perm != "-") )
        {
            return false;
        }

        token = strtok(nullptr, " ");
        type = std::string(token);
        if (token == NULL) {
            return false;
        }

        long long pos = type.length() + perm.length() + 2;
        long long len = textStd.length() - pos;
        name = textStd.substr(pos, len);

        at.push_back({perm, type, name});
    }

    return true;
}

actorParams::actorParams(qreal x, qreal y, QString name, QColor color)
    : objectParams(x, y, name, color) {
};