#include "Parse.h"

/**
 *
 * @param filename
 */
void Program::parse_file(std::string filename) {
    std::ifstream f(filename, std::ifstream::in);
    json file;

    try {
        f >> file;
    } catch(nlohmann::detail::parse_error) {
        std::cout << "EEEEE chuska, bad format of the file!![]" << std::endl;
        return;
    }

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

/**
 *
 * @param el
 * @param coords
 */
void Program::push_coords(const json el, std::vector<double>& coords) {

    coords.insert( coords.end(),
               { el.at("x").get<double>(), el.at("y").get<double>() } );
}

/**
 *
 * @param filename
 */
void Program::fill_file(std::string filename) {
    json j;

    j["class_dgrm"]["class"] = nullptr;
    j["class_dgrm"]["connections"] = nullptr;

    obj_cl.add_class_to_file(j["class_dgrm"], this->diagram_class.classes);
    obj_cl.add_connect_to_file(j["class_dgrm"], this->diagram_class.concts);

    j["seq_dgrm"] = nullptr;


    int i = 0;
    for (auto& x : this->diagram_sequence) {
        j["seq_dgrm"][i]["actors"] = nullptr;
        j["seq_dgrm"][i]["actions"] = nullptr;
        j["seq_dgrm"][i]["activates"] = nullptr;
        obj_se.add_actor_to_file(j["seq_dgrm"][i], x.actors);
        obj_se.add_action_to_file(j["seq_dgrm"][i], x.actions);
        obj_se.add_activate_to_file(j["seq_dgrm"][i], x.activates);
        i++;
    }

    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}
