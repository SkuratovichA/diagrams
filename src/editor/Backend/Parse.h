// File: Parse.h
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <streambuf>
#include <fstream>
#include "json.hpp"

#include "SeqDiagram.h"

using json = nlohmann::json;

class Program {
public:
    dgrmClass_t diagramClass;
    std::vector<dgrmSeq_t> diagramSequence;
    DiagramClass obj_cl;
    DiagramSequence obj_se;

    void parseFile(std::string filename);

    static void pushCoords(const json el, std::vector<double>& coords);

    void fillFile(std::string filename);
};
