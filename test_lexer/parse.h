#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <streambuf>
#include <fstream>
#include "json.hpp"

#include "seq_diagram.h"
#include "class_diagram.h"

using json = nlohmann::json;

class Note {
public:
    std::vector<double> coords;
    std::string msg;

    void fill_note(const json el);
};

json check_null(std::string val);

class Program {
public:
    dgrm_class_t diagram_class;
    dgrm_seq_t diagram_sequence;
    DiagramClass obj_cl;
    DiagramSequence obj_se;

    void parse_file(const json file);

    static void push_coords(const json el, std::vector<double>& coords);

    void fill_structure_note(const json el, std::vector<Note>& note);

    void add_note_to_file(json& j, std::vector<Note> no);

    void fill_file();
};
