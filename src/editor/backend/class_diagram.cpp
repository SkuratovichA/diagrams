#include "parse.h"

void Class::push_color(const json el) {
    this->color = el.is_null() ? "" : el.get<std::string>();
}

void Class::push_name(const json el) {

    this->name = el.get<std::string>();
}

void Class::push_width(const json el) {
    this->width = el.get<double>();
}

void Class::push_height(const json el) {
    this->height = el.get<double>();
}

void Class::push_attrs(const json el, std::vector<attrs_t>& obj) {
    std::string tmp;

    for(auto x : el) {
        tmp = x.at("perm").is_null() ? "" : x.at("perm").get<std::string>();
        obj.push_back( { .perm = tmp,
                         .type = x.at("type").get<std::string>(),
                         .name = x.at("name").get<std::string>() } );
    }
}

void Conct::fill_connection(const json el) {
    std::string tmp1 = el.at("left_num" ).is_null() ? "" : el.at("left_num").get<std::string>();
    std::string tmp2 = el.at("right_num").is_null() ? "" : el.at("right_num").get<std::string>();
    std::string tmp3 = el.at("msg").is_null() ? "" : el.at("msg").get<std::string>();

    this->left_obj  = el.at("left_obj" ).get<std::string>();
    this->left_num  = tmp1;
    this->arrow     = el.at("arrow"    ).get<std::string>();
    this->right_obj = el.at("right_obj").get<std::string>();
    this->right_num = tmp2;
    this->msg       = tmp3;
}

void DiagramClass::fill_structure_conct(const json el, dgrm_class_t& o) {
    for (auto x : el) {
        Conct tmp;
        tmp.fill_connection(x);
        o.concts.push_back(tmp);
    }
}

void DiagramClass::fill_structure_class(const json el, dgrm_class_t& o) {
    json tmp_ch;

    for (auto x : el) {
        Class tmp;
        Program::push_coords(x.at("coords"), tmp.coords);
        tmp.push_name(x.at("name"));
        tmp.push_color(x.at("color"));
        tmp.push_attrs(x.at("attrs"), tmp.attrs);
        tmp.push_attrs(x.at("methods"), tmp.methods);
        tmp.push_width(x.at("width"));
        tmp.push_height(x.at("height"));
        o.classes.push_back(tmp);
    }
}

void DiagramClass::add_connect_to_file(json& j, std::vector<Conct> cn) {
    int i = 0;

    for (auto& x : cn) {
        j["connections"][i++] =
        {
            {"left_obj" ,  x.left_obj },
            {"left_num" , check_null(x.left_num) },
            {"arrow" , x.arrow },
            {"right_num", check_null(x.right_num) },
            {"right_obj", x.right_obj },
            {"msg", check_null(x.msg) }
        };
    }
}

void DiagramClass::add_class_to_file(json& j, std::vector<Class> cl) {
    int i = 0;

    for (auto& x : cl) {
        j["class"][i++] =
        {
            {"color" , check_null(x.color) },
            {"name" , x.name },
            {"coords",
                {
                    {"x" , x.coords[0]},
                    {"y" , x.coords[1]}
                }
            },
            {"attrs" , add_attrs(x.attrs) },
            {"methods", add_attrs(x.methods) },
            {"width", x.width},
            {"height", x.height}
        };
    }
}

json DiagramClass::add_attrs(std::vector<attrs_t> x) {
    json tmp = nullptr;
    int j = 0;

    if (x.size() != 0) {
        for (auto & xx : x) {
            tmp[j++] = {
                {"perm" , check_null(xx.perm)},
                {"type" , xx.type},
                {"name" , xx.name}
            };
        }
    }

    return tmp;
}
