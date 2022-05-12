// File: Parse.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Parse.h"

void Program::parseFile(std::string filename) {
    std::ifstream f(filename, std::ifstream::in);
    json file;

    try {
        f >> file;
    } catch (nlohmann::detail::parse_error) {
        std::cout << "EEEEE chuska, bad format of the file!![]" << std::endl;
        return;
    }

    try {
        obj_cl.fillStructureClass(file.at("class_dgrm").at("class"), this->diagramClass);
        obj_cl.fillStructureConct(file.at("class_dgrm").at("connections"), this->diagramClass);

        for (auto &seq: file.at("seq_dgrm")) {
            dgrmSeq_t tmp;
            obj_se.fillStructureActor(seq.at("actors"), tmp);
            obj_se.fillStructureAction(seq.at("actions"), tmp);
            this->diagramSequence.push_back(tmp);
        }
    } catch (nlohmann::detail::out_of_range) {
        std::cout << "The user is dolbaeb, menya ne ebet!" << std::endl;
    } catch (nlohmann::detail::type_error) {
        std::cout << "Omg, Maks, eto ty? Shto za hyinu in the file ty napisal..." << std::endl;
    }
}

void Program::pushCoords(const json el, std::vector<double> &coords) {

    coords.insert(coords.end(),
                  {el.at("x").get<double>(), el.at("y").get<double>()});
}

void Program::fillFile(std::string filename) {
    json j;

    j["class_dgrm"]["class"] = nullptr;
    j["class_dgrm"]["connections"] = nullptr;

    obj_cl.addClassToFile(j["class_dgrm"], this->diagramClass.classes);
    obj_cl.addConnectToFile(j["class_dgrm"], this->diagramClass.concts);

    j["seq_dgrm"] = nullptr;

    int i = 0;
    for (auto &x: this->diagramSequence) {
        j["seq_dgrm"][i]["actors"] = nullptr;
        j["seq_dgrm"][i]["actions"] = nullptr;
        obj_se.addActorToFile(j["seq_dgrm"][i], x.actors);
        obj_se.addActionToFile(j["seq_dgrm"][i], x.actions);
        i++;
    }

    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}
