// File: ClassDiagram.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Parse.h"

void Table::pushColor(const json el) {
    this->color.r = el.at("r").get<int>();
    this->color.g = el.at("g").get<int>();
    this->color.b = el.at("b").get<int>();
    this->color.a = el.at("a").get<int>();
}

void Table::pushName(const json el) {
    this->name = el.get<std::string>();
}

void Table::pushWidth(const json el) {
    this->width = el.get<double>();
}

void Table::pushHeight(const json el) {
    this->height = el.get<double>();
}

void Table::pushAttrs(const json el, std::vector<attrs_t> &obj) {
    for (auto x: el) {
        obj.push_back({.perm = x.at("perm").get<std::string>(),
                              .type = x.at("type").get<std::string>(),
                              .name = x.at("name").get<std::string>()});
    }
}

void ClassDiagramConnectionRepresentation::pushConnection(const json el) {
    this->leftObj = el.at("left_obj").get<std::string>();
    this->leftNum = el.at("left_num").get<std::string>();
    this->arrow = el.at("arrow").get<int>();
    this->rightObj = el.at("right_obj").get<std::string>();
    this->rightNum = el.at("right_num").get<std::string>();
    this->msg = el.at("msg").get<std::string>();
    this->order = el.at("order").get<int>();
}

void DiagramClass::fillStructureConct(const json el, dgrmClass_t &o) {
    for (auto x: el) {
        ClassDiagramConnectionRepresentation tmp;
        tmp.pushConnection(x);
        o.concts.push_back(tmp);
    }
}

void DiagramClass::fillStructureClass(const json el, dgrmClass_t &o) {
    json tmp_ch;

    for (auto x: el) {
        Class tmp;
        Program::pushCoords(x.at("coords"), tmp.coords);
        tmp.pushName(x.at("name"));
        tmp.pushColor(x.at("color"));
        tmp.pushAttrs(x.at("attrs"), tmp.attrs);
        tmp.pushAttrs(x.at("methods"), tmp.methods);
        tmp.pushWidth(x.at("width"));
        tmp.pushHeight(x.at("height"));
        o.classes.push_back(tmp);
    }
}

void DiagramClass::addConnectToFile(json &j, std::vector<ClassDiagramConnectionRepresentation> cn) {
    int i = 0;
    for (auto &x: cn) {
        j["connections"][i++] =
                {
                        {"left_obj",  x.leftObj},
                        {"left_num",  x.leftNum},
                        {"arrow",     x.arrow},
                        {"right_num", x.rightNum},
                        {"right_obj", x.rightObj},
                        {"msg",       x.msg},
                        {"order",     x.order}
                };
    }
}

void DiagramClass::addClassToFile(json &j, std::vector<Class> cl) {
    int i = 0;

    for (auto &x: cl) {
        j["class"][i++] =
                {
                        {"color",
                                    {
                                            {"r", x.color.r},
                                            {"g", x.color.g},
                                            {"b", x.color.b},
                                            {"a", x.color.a},
                                    }
                        },
                        {"name",    x.name},
                        {"coords",
                                    {
                                            {"x", x.coords[0]},
                                            {"y", x.coords[1]}
                                    }
                        },
                        {"attrs",   addAttrs(x.attrs)},
                        {"methods", addAttrs(x.methods)},
                        {"width",   x.width},
                        {"height",  x.height}
                };
    }
}

void DiagramClass::addInterfaceToFile(json& j, std::vector<Interface> cl) {
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
            {"name" ,   x.name },
            {"coords",
                {
                    {"x" , x.coords[0]},
                    {"y" , x.coords[1]}
                }
            },
            {"methods", addAttrs(x.methods) },
            {"width",   x.width},
            {"height",  x.height}
        };
    }
}

json DiagramClass::addAttrs(std::vector<attrs_t> x) {
    json tmp = nullptr;
    int j = 0;

    if (x.size() != 0) {
        for (auto &xx: x) {
            tmp[j++] = {
                    {"perm", xx.perm},
                    {"type", xx.type},
                    {"name", xx.name}
            };
        }
    }

    return tmp;
}
