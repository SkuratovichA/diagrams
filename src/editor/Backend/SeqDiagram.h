// File: SeqDiagram.h
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#pragma once

#include "ClassDiagram.h"

using json = nlohmann::json;

class Actor {
public:
    std::string name;
    Color color;
    std::vector<double> coords;
};

class Action {
public:
    std::string from;
    std::string to;
    std::string arrow;
    std::string msg;

    void pushConnection(const json el);
};

class Activate {
public:
    std::string who;
    std::vector<double> coords;

    void pushActivate(const json el);
};

typedef struct dgrm_seq {
    std::vector<Actor> actors;
    std::vector<Action> actions;
    std::vector<Activate> activates;
} dgrmSeq_t;

class DiagramSequence {
public:
    void fillStructureActor(const json el, dgrmSeq_t& o);

    void fillStructureAction(const json el, dgrmSeq_t& o);

    void fillStructureActivate(const json el, dgrmSeq_t& o);

    void addActorToFile(json& j, std::vector<Actor> ac);

    void addActionToFile(json& j, std::vector<Action> ac);

    void addActivateToFile(json& j, std::vector<Activate> ac);
};
