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

class Class {
public:
    std::vector<double> coords;
    //int id;
    std::string name;
    Color color;
    std::vector<attrs_t> attrs;
    std::vector<attrs_t> methods;
    double width;
    double height;

    void pushColor(const json el);

    void pushName(const json el);

    void pushAttrs(const json el, std::vector<attrs_t>& obj);

    void pushWidth(const json el);

    void pushHeight(const json el);
};

class Conct {
public:
    //int leftObjId;
    //int rightObjId;
    std::string leftObj;
    std::string leftNum;
    int arrow;
    std::string rightObj;
    std::string rightNum;
    std::string msg;
    int order;

    void pushConnection(const json el);
};

typedef struct dgrm_class {
    std::vector<Class> classes;
    std::vector<Conct> concts;
} dgrmClass_t;

class DiagramClass {
public:
    void fillStructureConct(const json el, dgrmClass_t& o);

    void fillStructureClass(const json el, dgrmClass_t& o);

    void addConnectToFile(json& j, std::vector<Conct> cn);

    void addClassToFile(json& j, std::vector<Class> cl);

    json addAttrs(std::vector<attrs_t> x);
};
