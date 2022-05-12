// File: SeqDiagram.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Parse.h"

void Action::pushConnection(const json el) {
    this->from = el.at("from").get<std::string>();
    this->arrow = el.at("arrow").get<std::string>();
    this->to = el.at("to").get<std::string>();
    this->msg = el.at("msg").get<std::string>();
    this->type = el.at("type").get<int>();
}

void DiagramSequence::fillStructureActor(const json el, dgrmSeq_t &o) {
    for (auto x: el) {
        Actor act;
        Program::pushCoords(x.at("coords"), act.coords);
        act.name = x.at("actor").get<std::string>();
        act.color.r = x.at("color").at("r").get<int>();
        act.color.g = x.at("color").at("g").get<int>();
        act.color.b = x.at("color").at("b").get<int>();
        act.color.a = x.at("color").at("a").get<int>();
        o.actors.push_back(act);
    }
}

void DiagramSequence::fillStructureAction(const json el, dgrmSeq_t &o) {
    for (auto x: el) {
        Action tmp;
        Program::pushCoords(x.at("coords"), tmp.coords);
        tmp.pushConnection(x);
        o.actions.push_back(tmp);
    }
}

void DiagramSequence::addActorToFile(json &j, std::vector<Actor> ac) {
    int i = 0;
    for (auto &x: ac) {
        j["actors"][i++] =
                {
                        {"actor", x.name},
                        {"color",
                                  {
                                          {"r", x.color.r},
                                          {"g", x.color.g},
                                          {"b", x.color.b},
                                          {"a", x.color.a}
                                  }
                        },
                        {"coords",
                                  {
                                          {"x", x.coords[0]},
                                          {"y", x.coords[1]}
                                  }
                        }
                };
    }
}

void DiagramSequence::addActionToFile(json &j, std::vector<Action> ac) {
    int i = 0;

    for (auto &x: ac) {
        j["actions"][i++] =
                {
                        {"from",  x.from},
                        {"to",    x.to},
                        {"arrow", x.arrow},
                        {"msg",   x.msg},
                        {"type",  x.type},
                        {"coords",
                                  {
                                          {"x", x.coords[0]},
                                          {"y", x.coords[1]}
                                  }
                        }
                };
    }
}
