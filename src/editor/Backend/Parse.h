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

/**
 * Program representation in json format.
 */
class Program {
public:
    dgrmClass_t diagramClass;
    std::vector<dgrmSeq_t> diagramSequence;
    DiagramClass obj_cl;
    DiagramSequence obj_se;

    /**
     * @brief Read a json file and fill in all structures related to the node of json file.
     * @param filename name of the file for reading
     */
    void parseFile(std::string filename);

    /**
     * @brief Set values to the structure coords from json file.
     * @param el node of a json file with coordinates [x, y] for object
     * @param coords vector with coordinates
     */
    static void pushCoords(const json el, std::vector<double> &coords);

    /**
     * @brief Fill an array of class_dgrm and seq_dgrm nodes in json file.
     * @param filename name of the file for writing
     */
    void fillFile(std::string filename);
};
