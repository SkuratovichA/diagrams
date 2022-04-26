#include "parse.h"

void Note::fill_note(const json el) {
    std::string tmp = el.at("msg").is_null() ? "" : el.at("msg").get<std::string>();
    this->msg = tmp;
}

json check_null(std::string val) {
    //json tmp = val != "" ? val : nullptr;
    json tmp = nullptr;
    if (val != "") {
        tmp = val;
    }

    return tmp;
}

void Program::parse_file(const json file) {
    try {
        obj_cl.fill_structure_class(file.at("class_dgrm").at("class"), this->diagram_class);
        obj_cl.fill_structure_conct(file.at("class_dgrm").at("connections"), this->diagram_class);
        this->fill_structure_note(file.at("class_dgrm").at("notes"), this->diagram_class.notes);

        for (auto& seq : file.at("seq_dgrm")) {
            dgrm_seq_t tmp;
            obj_se.fill_structure_actor(seq.at("actors"), tmp);
            obj_se.fill_structure_action(seq.at("actions"), tmp);
            this->fill_structure_note(seq.at("notes"), tmp.notes);
            obj_se.fill_structure_activate(seq.at("activates"), tmp);
            obj_se.fill_structure_alt(seq.at("alts"), tmp);
            this->diagram_sequence.push_back(tmp);
        }
    } catch(nlohmann::detail::out_of_range) {
        std::cout << "The user is dolbaeb, menya ne ebet!" << std::endl;
    } catch(nlohmann::detail::type_error) {
        std::cout << "Omg, Maks, eto ty? Shto za hyinu in the file ty napisal..." << std::endl;
    }
}

void Program::push_coords(const json el, std::vector<double>& coords) {

    coords.insert( coords.end(),
                   { el.at("x").get<double>(),
                     el.at("y").get<double>() } );
}

void Program::fill_structure_note(const json el, std::vector<Note>& note) {
    for (auto x : el) {
        Note tmp;
        push_coords(x.at("coords"), tmp.coords);
        tmp.fill_note(x);
        note.push_back(tmp);
    }
}

void Program::add_note_to_file(json& j, std::vector<Note> no) {
    int i = 0;

    for (auto& x : no) {
        j["notes"][i++] =
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

void Program::fill_file() {
    json j;

    j["class_dgrm"]["class"] = nullptr;
    j["class_dgrm"]["connections"] = nullptr;
    j["class_dgrm"]["notes"] = nullptr;

    obj_cl.add_class_to_file(j["class_dgrm"], this->diagram_class.classes);
    this->add_note_to_file(j["class_dgrm"], this->diagram_class.notes);
    obj_cl.add_connect_to_file(j["class_dgrm"], this->diagram_class.concts);

    j["seq_dgrm"] = nullptr;

    int i = 0;
    for (auto& x : this->diagram_sequence) {
        obj_se.add_actor_to_file(j["seq_dgrm"][i], x.actors);
        obj_se.add_action_to_file(j["seq_dgrm"][i], x.actions);
        this->add_note_to_file(j["seq_dgrm"][i], x.notes);
        obj_se.add_activate_to_file(j["seq_dgrm"][i], x.activates);
        obj_se.add_alt_to_file(j["seq_dgrm"][i], x.alts);
        i++;
    }

    std::ofstream o("pretty.json");
    o << std::setw(4) << j << std::endl;
}
