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

    qreal width() const {
        return _width;
    }

    qreal height() const {
        return _height;
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


private:
    qreal _height;
    qreal _width;
    qreal _scale;
    QList<QString> _attrs;
    QList<QString> _methods;
    QString _name;
};

class fillItems {

};


#endif //DIAGRAMS_FILLITEMS_H
