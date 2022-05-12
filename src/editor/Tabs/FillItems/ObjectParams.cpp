// File: ObjectParams.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "ObjectParams.h"
#include <QDebug>
#include <QMessageBox>
#include "../TabCanvas/TabCanvas.h"

QString ObjectParams::getMethod(std::string fullText) {
    std::string perm;
    std::string type;
    char *token;

    if (fullText.length() < 2) {
        return QString::fromStdString("");
    }

    token = strtok(const_cast<char*>(fullText.c_str()), " ");
    perm = std::string(token);
    if (token == NULL) {
        return QString::fromStdString("");
    }

    token = strtok(nullptr, " ");
    type = std::string(token);
    if (token == NULL) {
        return QString::fromStdString("");
    }

    long long pos = type.length() + perm.length() + 2;
    long long idx = fullText.find('(');
    return QString::fromStdString(fullText.substr(pos, idx - pos));
}

bool ObjectParams::checkMethod(SequenceConnectionItem *item) {
    QString msgMethod;
    QString classMethod;
    bool flag = false;
    long long idx;

    idx = item->getText()->toPlainText().toStdString().find('(');
    msgMethod = item->getText()->toPlainText().mid(0, idx);
    for (auto method : item->nodeTo()->parentClassDiagramItem()->methods()) {
        if (method->toPlainText() == "METHODS") {
            continue;
        }

        classMethod = getMethod(method->toPlainText().toStdString());

        if (classMethod == "") {
            qDebug() << "HUINYA METHOD";
        }
        else if (classMethod == msgMethod) {
            flag = true;
            qDebug() << classMethod;
            qDebug() << msgMethod;
        }
    }

    if (!flag) {
        item->getText()->setDefaultTextColor(QColor(Qt::red));
        return false;
    }
    else {
        item->getText()->setDefaultTextColor(QColor(Qt::black));
        return true;
    }
}

/**
 * A constructor.
 *
 * @param x
 * @param y
 * @param name
 * @param color
 */
objectParams::objectParams(qreal x, qreal y, QString name, QColor color/*, int id*/)
{
    _x = x;
    _y = y;
    _color = color;
    _name = name;
//    _id = id;
};

/**
 *
 * @param x
 * @param y
 * @param name
 * @param color
 * @param width
 * @param height
 * @param attrs
 * @param methods
 */
classParams::classParams(qreal x, qreal y, QString name,
                         QColor color, qreal width, qreal height,
                         QList<QString> attrs, QList<QString> methods/*, int id*/)
        : objectParams(x, y, name, color/*, id*/) {
    _width = width;
    _height = height;

    for (auto val: attrs) {
        _attrs.push_back(val);
    }
    for (auto val: methods) {
        _methods.push_back(val);
    }
};

/**
 *
 * @param at
 * @param arr
 * @return
 */
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
        if (token == NULL) {
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

/**
 * A constructor.
 *
 * @param x
 * @param y
 * @param name
 * @param color
 */
actorParams::actorParams(qreal x, qreal y, QString name, QColor color/*, int id*/)
    : objectParams(x, y, name, color/*, id*/) {
};

messageParams::messageParams(qreal x, qreal y, QString msg, QString nameFrom, QString nameTo, int type/*, int fromId, int toId*/) {
    _x = x;
    _y = y;
    _msg = msg;
    _nameFrom = nameFrom;
    _nameTo = nameTo;
    _type = type;
    //_fromId = fromId;
    //_toId = toId;
}