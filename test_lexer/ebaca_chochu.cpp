#include <iostream>
#include <string>
#include <vector>
#include <iterator>
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
    //json tmp = val != "" ? val : nullptr;
    json tmp = nullptr;
    if (val != "") {
        tmp = val;
    }

    return tmp;
}

class Note {
public:
    std::vector<double> coords;
    std::string msg;

    void fill_note(const json el) {
        std::string tmp = el.at("msg").is_null() ? "" : el.at("msg").get<std::string>();
        this->msg = tmp;
    }
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
    std::string color;
    std::vector<attrs_t> attrs;
    std::vector<attrs_t> methods;

    void push_color(const json el) {
        this->color = el.is_null() ? "" : el.get<std::string>();
    }

    void push_name(const json el) {
        this->name = el.get<std::string>();
    }

    void push_attrs(const json el, std::vector<attrs_t>& obj) {
        std::string tmp;

        for(auto x : el) {
            tmp = x.at("perm").is_null() ? "" : x.at("perm").get<std::string>();
            obj.push_back( { .perm = tmp,
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

class Action {
public:
    std::string from;
    std::string to;
    std::string arrow;
    std::string msg;

    void fill_connection(const json el) {
        this->from  = el.at("from" ).get<std::string>();
        this->arrow = el.at("arrow").get<std::string>();
        this->to    = el.at("to"   ).get<std::string>();
        this->msg   = el.at("msg"  ).get<std::string>();
    }
};

class Activate {
public:
    std::string who;
    std::vector<double> coords;

    void fill_activate(const json el) {
        this->who = el.at("msg").get<std::string>();
    }
};

class Alt {
public:
    std::string if_cond;
    std::string else_cond;
    std::vector<double> coords;

    void fill_alt(const json el) {
        this->if_cond = el.at("if_cond").get<std::string>();
        this->else_cond = el.at("else_cond").get<std::string>();;
    }
};

typedef struct dgrm_class {
    std::vector<Class> classes;
    std::vector<Conct> concts;
    std::vector<Note> notes;
} dgrm_class_t;

typedef struct dgrm_seq {
    std::vector<Alt> alts;
    std::vector<std::string> actors;
    std::vector<Action> actions;
    std::vector<Note> notes;
    std::vector<Activate> activates;
} dgrm_seq_t;

class Program {
public:
    dgrm_class_t diagram_class;
    dgrm_seq_t diagram_sequence;

    void parse_file(const json file) {
        try {
            this->fill_structure_class(file.at("class_dgrm").at("class"));
            this->fill_structure_conct(file.at("class_dgrm").at("connections"));
            this->fill_structure_note(file.at("class_dgrm").at("notes"), diagram_class.notes);

            for (auto& seq : file.at("seq_dgrm")) {
                this->fill_structure_actor(seq.at("actors"));
                this->fill_structure_action(seq.at("actions"));
                this->fill_structure_note(seq.at("notes"), diagram_sequence.notes);
                this->fill_structure_activate(seq.at("activates"));
                this->fill_structure_alt(seq.at("alts"));
            }
        } catch(nlohmann::detail::out_of_range) {
            std::cout << "The user is dolbaeb, menya ne ebet!" << std::endl;
        } catch(nlohmann::detail::type_error) {
            std::cout << "Omg, Maks, eto ty? Shto za hyinu in the file ty napisal..." << std::endl;
        }
    }

    void push_coords(const json el, std::vector<double>& coords) {

        coords.insert( coords.end(),
                       { el.at("x").get<double>(),
                         el.at("y").get<double>() } );
    }

    void fill_structure_actor(const json el) {
        for (auto x : el) {
            this->diagram_sequence.actors.push_back(x.at("actor").get<std::string>());
        }
    }

    void fill_structure_action(const json el) {
        for (auto x : el) {
            Action tmp;
            tmp.fill_connection(x);
            this->diagram_sequence.actions.push_back(tmp);
        }
    }

    void fill_structure_activate(const json el) {
        for (auto x : el) {
            Activate tmp;
            push_coords(x.at("coords"), tmp.coords);
            tmp.fill_activate(x);
            this->diagram_sequence.activates.push_back(tmp);
        }
    }

    void fill_structure_alt(const json el) {
        for (auto x : el) {
            Alt tmp;
            push_coords(x.at("coords"), tmp.coords);
            tmp.fill_alt(x);
            this->diagram_sequence.alts.push_back(tmp);
        }
    }

    void fill_structure_note(const json el, std::vector<Note>& note) {
        for (auto x : el) {
            Note tmp;
            push_coords(x.at("coords"), tmp.coords);
            tmp.fill_note(x);
            note.push_back(tmp);
        }
    }

    void fill_structure_conct(const json el) {
        for (auto x : el) {
            Conct tmp;
            tmp.fill_connection(x);
            this->diagram_class.concts.push_back(tmp);
        }
    }

    void fill_structure_class(const json el) {
        json tmp_ch;

        for (auto x : el) {
            Class tmp;
            push_coords(x.at("coords"), tmp.coords);
            tmp.push_name(x.at("name"));
            tmp.push_color(x.at("color"));
            tmp.push_attrs(x.at("attrs"), tmp.attrs);
            tmp.push_attrs(x.at("methods"), tmp.methods);
            this->diagram_class.classes.push_back(tmp);
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

    void add_connect_to_file(json& j, std::vector<Conct> cn) {
        int i = 0;

        for (auto& x : cn) {
            j["class_dgrm"]["connections"][i++] =
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

    void add_note_to_file(json& j, std::vector<Note> no) {
        int i = 0;

        for (auto& x : no) {
            j["class_dgrm"]["notes"][i++] =
            {
                {"coords",
                    {
                        {"x" , x.coords[0]},
                        {"y" , x.coords[1]}
                    }
                },
                {"msg" , x.msg }
            };
        }
    }

    void add_class_to_file(json& j, std::vector<Class> cl) {
        int i = 0;

        for (auto& x : cl) {
            j["class_dgrm"]["class"][i++] =
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
                {"methods", fill_attrs(x.methods) }
            };
        }
    }

    void fill_file() {
        json j;
        j["class_dgrm"]["class"] = nullptr;
        j["class_dgrm"]["connections"] = nullptr;
        j["class_dgrm"]["notes"] = nullptr;

        add_class_to_file(j, this->diagram_class.classes);
        add_connect_to_file(j, this->diagram_class.concts);
        add_note_to_file(j, this->diagram_class.notes);

        j["seq_dgrm"]["actors"] = nullptr;
        j["seq_dgrm"]["actions"] = nullptr;
        j["seq_dgrm"]["notes"] = nullptr;
        j["seq_dgrm"]["activates"] = nullptr;
        j["seq_dgrm"]["alts"] = nullptr;



        std::ofstream o("pretty.json");
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
