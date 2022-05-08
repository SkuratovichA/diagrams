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
    std::string name;
    Color color;
    std::vector<attrs_t> attrs;
    std::vector<attrs_t> methods;
    double width;
    double height;

    void push_color(const json el);

    void push_name(const json el);

    void push_attrs(const json el, std::vector<attrs_t>& obj);

    void push_width(const json el);

    void push_height(const json el);
};

class Conct {
public:
    std::string left_obj;
    std::string left_num;
    int arrow;
    std::string right_obj;
    std::string right_num;
    std::string msg;

    void fill_connection(const json el);
};

typedef struct dgrm_class {
    std::vector<Class> classes;
    std::vector<Conct> concts;
} dgrm_class_t;

class DiagramClass {
public:
    void fill_structure_conct(const json el, dgrm_class_t& o);

    void fill_structure_class(const json el, dgrm_class_t& o);

    void add_connect_to_file(json& j, std::vector<Conct> cn);

    void add_class_to_file(json& j, std::vector<Class> cl);

    json add_attrs(std::vector<attrs_t> x);
};
