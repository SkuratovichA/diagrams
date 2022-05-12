// File: ClassDiagram.h
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#pragma once

using json = nlohmann::json;

class Color {
public:
    int r;
    int g;
    int b;
    int a;
};

typedef struct attrs {
    std::string perm;
    std::string type;
    std::string name;
} attrs_t;

/**
 * Text (json representation of a class)
 */
class Class {
public:
    std::vector<double> coords; ///< coordinates
    std::string name; ///< name of a class
    Color color; ///< color of class's borders
    std::vector<attrs_t> attrs; ///< class attributes
    std::vector<attrs_t> methods; ///< class methods
    double width; ///< class width
    double height; ///< class height

    /**
     * @brief Set values to the structure Color from json file using RGB colors.
     * @param el node of a json file with color
     */
    void pushColor(const json el);

    /**
     * @brief Set value to the name variable from json file.
     * @param el node of a json file with name of class
     */
    void pushName(const json el);

    /**
     * @brief Set values to the structure attrs_t from json file.
     * @param el node of a json file with attributes
     * perm, type and name
     * @param obj vector with attributes
     */
    void pushAttrs(const json el, std::vector<attrs_t> &obj);

    /**
     * @bief Set value to the width variable from json file.
     * @param el node of a json file with width
     */
    void pushWidth(const json el);

    /**
     * @brief Set values to the height from json file.
     *
     * @param el node of a json file with height
     */
    void pushHeight(const json el);
};

/**
 * Connection representation.
 */
class ClassDiagramConnectionRepresentation {
public:
    std::string leftObj; ///< name of a left (from) object
    std::string leftNum; ///< coordinality on the top of the left part of the arrow
    int arrow; ///< type of an arrow
    std::string rightObj; ///< name of a right (to) object
    std::string rightNum; ///< coordinality on the top of thhe right part of the arrow
    std::string msg; ///< text above the connection line
    int order; ///< order of a connection

    /**
     * @brief Set values to the structure ClassDiagramConnectionRepresentation from json file.
     * @param el node of a json file with all attributes
     * for one connection between 2 classes
     */
    void pushConnection(const json el);
};

typedef struct dgrm_class {
    std::vector<Class> classes;
    std::vector<ClassDiagramConnectionRepresentation> concts;
} dgrmClass_t;

/**
 * Class for a textual representation of a program.
 */
class DiagramClass {
public:

    /**
     * @brief Create elements of ClassDiagramConnectionRepresentation type, fill necessary items and push to the vector
     * of all connections for one class diagram.
     * @param el node of a json file with all connections
     * for a class diagram
     * @param o structure of a class diagram
     */
    void fillStructureConct(const json el, dgrmClass_t &o);

    /**
     * @brief Create elements of Class type, fill necessary items and push to the vector
     * of all classes for one class diagram.
     * @param el node of a json file with all classes
     * for a class diagram
     * @param o structure of a class diagram
     */
    void fillStructureClass(const json el, dgrmClass_t &o);

    /**
     * @brief Write the data about all connections from Connect structure
     * to json file.
     * @param j json file
     * @param cn vector of the connections for class diagram
     */
    void addConnectToFile(json &j, std::vector<ClassDiagramConnectionRepresentation> cn);

    /**
     * @brief Write the data about all classes from Class structure
     * to json file.
     * @param j json file
     * @param cl vector of the classes for class diagram
     */
    void addClassToFile(json &j, std::vector<Class> cl);

    /**
     * @brief Write the data about all connections from Connect structure
     * to json file.
     * @param x vector of attributes
     * @return a json node for attributes
     */
    json addAttrs(std::vector<attrs_t> x);
};
