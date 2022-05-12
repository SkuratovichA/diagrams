// File: ClassDiagram.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Parse.h"

/**
 * Set values to the structure Color from json file using RGB colors.
 *
 * @param el node of a json file with color
 */
void Class::pushColor(const json el) {
    this->color.r = el.at("r").get<int>();
    this->color.g = el.at("g").get<int>();
    this->color.b = el.at("b").get<int>();
    this->color.a = el.at("a").get<int>();
}

/**
 * Set value to the name variable from json file.
 *
 * @param el node of a json file with name of class
 */
void Class::pushName(const json el) {
    this->name = el.get<std::string>();
}

/**
 * Set value to the width variable from json file.
 *
 * @param el node of a json file with width
 */
void Class::pushWidth(const json el) {
    this->width = el.get<double>();
}

/**
 * Set values to the height from json file.
 *
 * @param el node of a json file with height
 */
void Class::pushHeight(const json el) {
    this->height = el.get<double>();
}

/**
 * Set values to the structure attrs_t from json file.
 *
 * @param el node of a json file with attributes
 * perm, type and name
 * @param obj vector with attributes
 */
void Class::pushAttrs(const json el, std::vector<attrs_t>& obj) {
    for(auto x : el) {
        obj.push_back( { .perm = x.at("perm").get<std::string>(),
                         .type = x.at("type").get<std::string>(),
                         .name = x.at("name").get<std::string>() } );
    }
}

/**
 * Set values to the structure Conct from json file.
 *
 * @param el node of a json file with all attributes
 * for one connection between 2 classes
 */
void Conct::pushConnection(const json el) {
    //this->leftObjId = el.at("leftObjId").get<int>();
    //this->rightObjId() = el.at("rightObjId").get<int>();
    this->leftObj  = el.at("left_obj" ).get<std::string>();
    this->leftNum  = el.at("left_num" ).get<std::string>();
    this->arrow     = el.at("arrow"    ).get<int>();
    this->rightObj = el.at("right_obj").get<std::string>();
    this->rightNum = el.at("right_num" ).get<std::string>();
    this->msg       = el.at("msg" ).get<std::string>();
    this->order     = el.at("order" ).get<int>();
}

/**
 * Create elements of Conct type, fill necessary items and push to the vector
 * of all connections for one class diagram.
 *
 * @param el node of a json file with all connections
 * for a class diagram
 * @param o structure of a class diagram
 */
void DiagramClass::fillStructureConct(const json el, dgrmClass_t& o) {
    for (auto x : el) {
        Conct tmp;
        tmp.pushConnection(x);
        o.concts.push_back(tmp);
    }
}

/**
 * Create elements of Class type, fill necessary items and push to the vector
 * of all classes for one class diagram.
 *
 * @param el node of a json file with all classes
 * for a class diagram
 * @param o structure of a class diagram
 */
void DiagramClass::fillStructureClass(const json el, dgrmClass_t& o) {
    json tmp_ch;

    for (auto x : el) {
        Class tmp;
        Program::pushCoords(x.at("coords"), tmp.coords);
        tmp.pushName(x.at("name"));
        tmp.pushColor(x.at("color"));
        tmp.pushAttrs(x.at("attrs"), tmp.attrs);
        tmp.pushAttrs(x.at("methods"), tmp.methods);
        tmp.pushWidth(x.at("width"));
        tmp.pushHeight(x.at("height"));
        //tmp.id = el.at("id").get<int>();
        o.classes.push_back(tmp);
    }
}

/**
 * Write the data about all connections from Connect structure
 * to json file.
 *
 * @param j json file
 * @param cn vector of the connections for class diagram
 */
void DiagramClass::addConnectToFile(json& j, std::vector<Conct> cn) {
    int i = 0;

    for (auto& x : cn) {
        j["connections"][i++] =
        {
            {"left_obj" , x.leftObj },
            //{"leftObjId", x.leftObjId},
            //{"rightObjId", x.rightObjId},
            {"left_num" , x.leftNum },
            {"arrow" ,    x.arrow },
            {"right_num", x.rightNum },
            {"right_obj", x.rightObj },
            {"msg",       x.msg },
            {"order",     x.order}
        };
    }
}

/**
 * Write the data about all classes from Class structure
 * to json file.
 *
 * @param j json file
 * @param cl vector of the classes for class diagram
 */
void DiagramClass::addClassToFile(json& j, std::vector<Class> cl) {
    int i = 0;

    for (auto& x : cl) {
        j["class"][i++] =
        {
            {"color" ,
                        {
                    {"r", x.color.r},
                    {"g", x.color.g},
                    {"b", x.color.b},
                    {"a", x.color.a},
                }
            },
            //{"id", x.id},
            {"name" ,   x.name },
            {"coords",
                        {
                    {"x" , x.coords[0]},
                    {"y" , x.coords[1]}
                }
            },
            {"attrs" , addAttrs(x.attrs) },
            {"methods", addAttrs(x.methods) },
            {"width",   x.width},
            {"height",  x.height}
        };
    }
}

/**
 * Write the data about all connections from Connect structure
 * to json file.
 *
 * @param x vector of attributes
 *
 * @return a json node for attributes
 */
json DiagramClass::addAttrs(std::vector<attrs_t> x) {
    json tmp = nullptr;
    int j = 0;

    if (x.size() != 0) {
        for (auto & xx : x) {
            tmp[j++] = {
                {"perm" , xx.perm},
                {"type" , xx.type},
                {"name" , xx.name}
            };
        }
    }

    return tmp;
}
