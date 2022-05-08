#include "Parse.h"

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

        for (auto& seq : file.at("seq_dgrm")) {
            dgrm_seq_t tmp;
            obj_se.fill_structure_actor(seq.at("actors"), tmp);
            obj_se.fill_structure_action(seq.at("actions"), tmp);
            obj_se.fill_structure_activate(seq.at("activates"), tmp);
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
               { el.at("x").get<double>(), el.at("y").get<double>() } );
}

void Program::fill_file(std::string filename) {
    json j;

    j["class_dgrm"]["class"] = nullptr;
    j["class_dgrm"]["connections"] = nullptr;

    obj_cl.add_class_to_file(j["class_dgrm"], this->diagram_class.classes);
    obj_cl.add_connect_to_file(j["class_dgrm"], this->diagram_class.concts);

    j["seq_dgrm"] = nullptr;

    int i = 0;
    for (auto& x : this->diagram_sequence) {
        obj_se.add_actor_to_file(j["seq_dgrm"][i], x.actors);
        obj_se.add_action_to_file(j["seq_dgrm"][i], x.actions);
        obj_se.add_activate_to_file(j["seq_dgrm"][i], x.activates);
        i++;
    }

    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}
