#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include "single_include/nlohmann/json.hpp"
#include <typeinfo>

#include <streambuf>
#include <fstream>
#include <iomanip>
using json = nlohmann::json;

bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
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
    std::vector<attrs_t> attrs;
    std::vector<methods_t> methods;

    void push_name(const json el) {
        this->name = el.get<std::string>();
    }

    void push_coords(const json el) {
        this->coords.insert( this->coords.end(),
                             { el.at("x").get<double>(),
                               el.at("y").get<double>() } );
    }

    void push_attrs(const json el) {
        for(auto x : el) {
            this->attrs.push_back( { .perm = x.at("perm").get<std::string>(),
                                     .type = x.at("type").get<std::string>(),
                                     .name = x.at("name").get<std::string>() } );
        }
    }

    void push_methods(const json el) {
        for(auto x : el) {
            this->methods.push_back( { .perm = x.at("perm").get<std::string>(),
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
        this->left_obj  = el.at("left_obj" ).get<std::string>();
        this->left_num  = el.at("left_num" ).get<std::string>();
        this->arrow     = el.at("arrow"    ).get<std::string>();
        this->right_obj = el.at("right_obj").get<std::string>();
        this->right_num = el.at("right_num").get<std::string>();
        this->msg       = el.at("msg"      ).get<std::string>();
    }
};


// add color to classes
// add objects
// add interface
// add sequence action


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
        for (auto x : el) {
            Class tmp;
            tmp.push_coords(x.at("coords"));
            tmp.push_name(x.at("name"));
            std::cout << x << std::endl;
            if (x["attrs"] == NULL) {
                printf("aaaaaaaaaaaaaa\n");
            }
            std::cout << x << std::endl;

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
            printf("PIZDA\n");
        } catch (nlohmann::detail::type_error) {
            printf("Idi nahui\n");
        }
    }
};

int main(int argc, char **argv) {
    Program prg;

    std::ifstream f(argv[1], std::ifstream::in);
    json j;
    f >> j;

    prg.parse_file(j);

    return 0;
}
