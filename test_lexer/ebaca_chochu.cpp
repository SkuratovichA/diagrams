#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include "json.hpp"
#include <typeinfo>
#include <streambuf>
#include <fstream>
#include <iomanip>
using json = nlohmann::json;

json check_null(std::string val) {
    json tmp = nullptr;

    if (val != "") {
        tmp = val;
    }

    return tmp;
}

typedef struct attrs {
    std::string perm;
    std::string type;
    std::string name;
} attrs_t;

typedef struct methods {
    std::string perm;
    std::string type;
    std::string name;
} methods_t;

class Class {
public:
    std::vector<double> coords;
    std::string name;
    std::string color;
    std::vector<attrs_t> attrs;
    std::vector<methods_t> methods;

    void push_color(const json el) {
        this->color = el.is_null() ? "" : el.get<std::string>();
    }

    void push_name(const json el) {
        this->name = el.get<std::string>();
    }

    void push_coords(const json el) {
        this->coords.insert( this->coords.end(),
                             { el.at("x").get<double>(),
                               el.at("y").get<double>() } );
    }

    void push_attrs(const json el) {
        std::string tmp;

        for(auto x : el) {
            tmp = x.at("perm").is_null() ? "" : x.at("perm").get<std::string>();
            this->attrs.push_back( { .perm = tmp,
                                     .type = x.at("type").get<std::string>(),
                                     .name = x.at("name").get<std::string>() } );
        }
    }

    void push_methods(const json el) {
        std::string tmp;

        for(auto x : el) {
            tmp = x.at("perm").is_null() ? "" : x.at("perm").get<std::string>();
            this->methods.push_back( { .perm = tmp,
                                       .type = x.at("type").get<std::string>(),
                                       .name = x.at("name").get<std::string>() } );
        }
    }
};

class Conct {
public:
    std::string left_obj;
    std::string left_num;
    std::string arrow;
    std::string right_obj;
    std::string right_num;
    std::string msg;

    void fill_connection(const json el) {
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
};

class Program {
public:
    std::vector<Class> classes;
    std::vector<Conct> concts;
    std::vector<std::string> notes;


    void fill_structure_conct(const json el) {
        for (auto x : el) {
            Conct tmp;
            tmp.fill_connection(x);
            this->concts.push_back(tmp);
        }
    }

    void fill_structure_class(const json el) {
        json tmp_ch;

        for (auto x : el) {
            Class tmp;
            tmp.push_coords(x.at("coords"));
            tmp.push_name(x.at("name"));
            tmp.push_color(x.at("color"));
            tmp.push_attrs(x.at("attrs"));
            tmp.push_methods(x.at("methods"));
            this->classes.push_back(tmp);
        }
    }

    void parse_file(const json file) {
        try {
            this->fill_structure_class(file.at("class_dgrm").at("class"));
            this->fill_structure_conct(file.at("class_dgrm").at("connections"));
        } catch(nlohmann::detail::out_of_range) {
            std::cout << "The user is dolbaeb, menya ne ebet!" << std::endl;
        } catch(nlohmann::detail::type_error) {
            std::cout << "Omg, Maks, eto ty? Shto za hyinu in the file ty napisal..." << std::endl;
        }
    }

    json fill_attrs(std::vector<attrs_t> x) {
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

    json fill_methods(std::vector<methods_t> x) {
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

    void fill_file() {
        json j;
        j["class_dgrm"]["class"] = nullptr;
        j["class_dgrm"]["connections"] = nullptr;
        std::ofstream o("pretty.json");

        int i = 0;
        for (auto& x : this->classes) {
            j["class_dgrm"]["class"][i] =
            {
                {"color" , check_null(x.color) },
                {"name" , x.name },
                {"coords",
                    {
                        {"x" , x.coords[0]},
                        {"y" , x.coords[1]}
                    }
                },
                {"attrs" , fill_attrs(x.attrs) },
                {"methods", fill_methods(x.methods) }
            };

            i++;
        }

        i = 0;
        j["class_dgrm"]["connections"] = nullptr;
        for (auto& x : this->concts) {
            j["class_dgrm"]["connections"][i] =
            {
                {"left_obj" ,  x.left_obj },
                {"left_num" , check_null(x.left_num) },
                {"arrow" , x.arrow },
                {"right_num", check_null(x.right_num) },
                {"right_obj", x.right_obj },
                {"msg", check_null(x.msg) }
            };

            i++;
        }

        o << std::setw(4) << j << std::endl;
    }
};

int main(int argc, char **argv) {
    Program prg;

    std::ifstream f(argv[1], std::ifstream::in);
    json j;
    f >> j;

    prg.parse_file(j);
    prg.fill_file();

    return 0;
}
