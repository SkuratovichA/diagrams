//
// Created by shchepa on 4.5.22.
//

#ifndef DIAGRAMS_FILLITEMS_H
#define DIAGRAMS_FILLITEMS_H

#include <QString>
#include <QList>

class classParams {
public:
    classParams(qreal x, qreal y, qreal scale, QList<QString> attrs,
                QList<QString> methods, QString name);

    qreal x() const {
        return _x;
    }

    qreal y() const {
        return _y;
    }

    qreal scale() const {
        return _scale;
    }

    QList<QString> attrs() const {
        return _attrs;
    }

    QList<QString> methods() const {
        return _methods;
    }

    QString name() const {
        return _name;
    }

//    void pushAttr(QString pushItem) {
//        _attrs.push_back(pushItem);
//    }
//
//    void pushMethod(QString pushItem) {
//        _methods.push_back(pushItem);
//    }
//
//    void setName(QString name) {
//        _name = name;
//    }


private:
    qreal _x;
    qreal _y;
    qreal _scale;
    QList<QString> _attrs;
    QList<QString> _methods;
    QString _name;
};


#endif //DIAGRAMS_FILLITEMS_H
