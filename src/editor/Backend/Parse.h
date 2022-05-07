#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <streambuf>
#include <fstream>
#include "json.hpp"

#include "SeqDiagram.h"
#include "ClassDiagram.h"

using json = nlohmann::json;

json check_null(std::string val);

class Program {
public:
    dgrm_class_t diagram_class;
    std::vector<dgrm_seq_t> diagram_sequence;
    DiagramClass obj_cl;
    DiagramSequence obj_se;


    void parse_file(const json file);

    static void push_coords(const json el, std::vector<double>& coords);

    void fill_file();
};
