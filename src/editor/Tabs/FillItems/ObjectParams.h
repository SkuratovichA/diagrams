/** @file ObjectParams.h
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */


#ifndef DIAGRAMS_OBJECTPARAMS_H
#define DIAGRAMS_OBJECTPARAMS_H

#include <QString>
#include <QList>
#include <QColor>
#include <QWidget>
#include "../../Backend/Parse.h"

class SequenceConnectionItem;

/**
 * @brief Namespace with function for control the correctness of a method
 */
namespace ObjectParams {
    bool checkMethod(SequenceConnectionItem *item);
    QString getMethod(std::string fullText);
}

/**
 * @brief Parameters of class diagram connections class
 */
class relationsParams {
public:
    /**
     * @brief A constructor.
     * @param leftObj left object
     * @param leftNum left ordinality
     * @param rightObj right object
     * @param rightNum right ordinality
     * @param msg message above the connection
     * @param type connection type
     * @param order serial number of a connection
     */
    relationsParams(QString leftObj, QString leftNum,
                    QString rightObj, QString rightNum,
                    QString msg,
                    int type,
                    int order) {
        _leftObj = leftObj;
        _leftNum = leftNum;
        _rightObj = rightObj;
        _rightNum = rightNum;
        _msg = msg;
        _typeConnection = type;
        _order = order;
    };

    /**
     * @brief Getter.
     * @return left object
     */
    QString leftObj() {
        return _leftObj;
    }

    /**
     * @brief Getter.
     * @return ordinality of the left object
     */
    QString leftNum() {
        return _leftNum;
    }

    /**
     * @brief Getter
     * @return right object
     */
    QString rightObj() {
        return _rightObj;
    }

    /**
     * @brief Getter
     * @return ordinality of the right object
     */
    QString rightNum() {
        return _rightNum;
    }

    /**
     * @brief Getter
     * @return serial number of a connection
     */
    int order() {
        return _order;
    }

    /**
     * @brief Getter
     * @return type of a connection
     */
    int typeConnection() {
        return _typeConnection;
    }

    /**
     * @brief Getter.
     * @return QString message above the connection
     */
    QString msg() {
        return _msg;
    }

private:
    QString _leftObj; ///< The object a connection begins at
    QString _leftNum; ///< Ordinality of a left object
    QString _rightObj; ///< The object a connection ends at
    QString _rightNum; ///< Ordinality of a right object
    int _typeConnection; ///< Type of a connection
    QString _msg; ///< Message above a connection
    int _order; ///< Serial number of a connection - its order
};

/**
 * @brief Superclass for parameters of class and sequence diagram items
 */
class objectParams {
public:

    /**
     * @brief A contructor.
     * @param x x axis
     * @param y y axis
     * @param name name of the object
     * @param color color of the object
     */
    objectParams(qreal x, qreal y, QString name, QColor color);

    /**
     * @brief A destructor.
     */
    ~objectParams() = default;

    /**
     * @brief Getter.
     * @return x axis value
     */
    qreal x() const {
        return _x;
    }

    /**
     * @brief Getter.
     * @return y axis value
     */
    qreal y() const {
        return _y;
    }

    /**
     * @brief Getter.
     * @return color of an object
     */
    QColor color() const {
        return _color;
    }

    /**
     * @brief Getter.
     * @return width of an object
     */
    qreal width() const {
        return _width;
    }

    /**
     * @brief Getter.
     * @return height of an object
     */
    qreal height() const {
        return _height;
    }

    /**
     * @brief Getter.
     * @return name of an object
     */
    QString name() const {
        return _name;
    }

    /**
     * @brief Create a color
     * @param c color structure
     */
    void fillColor(Color &c) {
        c.r = color().red();
        c.g = color().green();
        c.b = color().blue();
        c.a = color().alpha();
    }

    /**
     * Create coordinates
     * @param v vector with the coordinates
     */
    void fillCoords(std::vector<double> &v) {
        v.push_back(x());
        v.push_back(y());
    }

protected:
    qreal _x; ///< x axis value of an object
    qreal _y; ///< y axis value of an object
    QColor _color; ///< color of an object
    qreal _width; ///< width of an object
    qreal _height; ///< height of an object
    QString _name; ///< name of an object
};

/**
 * @brief Parameters for class diagram item (ClassDiagramItem class)
 */
class ClassDiagramItemParameters : public objectParams {
public:
    /**
     * @brief A constructor
     * @param x x axis
     * @param y y axis
     * @param name name of a class
     * @param color color of a class
     * @param width width of a class
     * @param height height of a class
     * @param attrs attributes of a class
     * @param methods methods of a class
     */
    ClassDiagramItemParameters(qreal x, qreal y,
                               QString name,
                               QColor color,
                               qreal width,
                               qreal height,
                               QList<QString> attrs,
                               QList<QString> methods,
                               QString type);

    /**
     * @brief A Destructor.
     */
    ~ClassDiagramItemParameters() = default;

    /**
     * @brief Getter.
     * @return attributes of a class
     */
    QList<QString> attrs() const {
        return _attrs;
    }

    /**
     * @brief Getter
     * @return methods of a class
     */
    QList<QString> methods() const {
        return _methods;
    }

    /**
     * @brief Getter.
     * @return QStraing type
     */
    QString typeClass() const {
        return _type;
    }

    /**
     * @brief Split every string in an arrray
     * @param at positions
     * @param arr array with strings
     * @return true if every string has been slitted successfully
     */
    bool splitString(std::vector<attrs_t> &at, QList<QString> arr);

private:
    QList<QString> _attrs; ///< list with class attributes
    QList<QString> _methods; ///< list with class methods
    QString _type; ///< type of a class diagram
};

/**
 * @brief A class with parameters for a sequence diagram (SequenceDiagramItem class)
 */
class SequenceDiagramItemParameters : public objectParams {
public:
    /**
     * @brief A Constructor.
     * @param x x axis
     * @param y y axis
     * @param name name of a sequence diagram item
     * @param color color of a sequence diagram item
     */
    SequenceDiagramItemParameters(qreal x, qreal y,
                                  QString name,
                                  QColor color);
};

/**
 * @brief Parameters for a connection item
 */
class messageParams {
public:
    /**
     * @brief A Constructor.
     * @param x x axis
     * @param y y axis
     * @param msg  message above te connection (method name)
     * @param nameFrom start node
     * @param nameTo end node
     * @param type type of a connection
     */
    messageParams(qreal x, qreal y,
                  QString msg,
                  QString nameFrom,
                  QString nameTo,
                  int type);

    /**
     * @brief Getter.
     * @return x axis
     */
    qreal x() {
        return _x;
    }

    /**
     * @brief Getter.
     * @return y axis
     */
    qreal y() {
        return _y;
    }

    /**
     * @brief Getter.
     * @return message
     */
    QString msg() {
        return _msg;
    }

    /**
     * @brief Getter.
     * @return start node name
     */
    QString nameFrom() {
        return _nameFrom;
    }

    /**
     * @brief Getter.
     * @return end node name
     */
    QString nameTo() {
        return _nameTo;
    }

    /**
     * @brief Getter
     * @return type of a message
     */
    int type() {
        return _type;
    }

    /**
     * Create a point
     * @param v coordinates
     */
    void fillCoords(std::vector<double> &v) {
        v.push_back(x());
        v.push_back(y());
    }

private:
    qreal _x; ///< x axis of a connection
    qreal _y; ///< y axis of a connection
    QString _msg; ///< Message above the connection (method name)
    QString _nameFrom; ///< Name of the class which the message comes from
    QString _nameTo; ///< Name of the class whitch the message comes to
    int _type; ///< Type of a message
};

#endif //DIAGRAMS_OBJECTPARAMS_H
