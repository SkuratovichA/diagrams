// File: SeqDiagram.cpp
// Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
// Date: 07.05.2022

#include "Parse.h"

/**
 * Set values to the structure Action from json file.
 *
 * @param el node of a json file with attributes
 * for one action between 2 actors
 */
void Action::pushConnection(const json el) {
    //this->fromId = el.at("fromId").get<int>();
    //this->toId = el.at("toId").get<int>();
    this->from  = el.at("from" ).get<std::string>();
    this->arrow = el.at("arrow").get<std::string>();
    this->to    = el.at("to"   ).get<std::string>();
    this->msg   = el.at("msg"  ).get<std::string>();
    this->type  = el.at("type" ).get<int>();
}

/**
 * Create elements of Actor type, fill necessary items and push to the vector
 * of all actors for one sequence diagram.
 *
 * @param el node of a json file with all actors
 * for a sequence diagram
 * @param o structure of a sequence diagram
 */
void DiagramSequence::fillStructureActor(const json el, dgrmSeq_t& o) {
    for (auto x : el) {
        Actor act;
        Program::pushCoords(x.at("coords"), act.coords);
        act.name = x.at("actor").get<std::string>();
        //act.id = x.at("id").get<int>();
        act.color.r = x.at("color").at("r").get<int>();
        act.color.g = x.at("color").at("g").get<int>();
        act.color.b = x.at("color").at("b").get<int>();
        act.color.a = x.at("color").at("a").get<int>();
        o.actors.push_back(act);
    }
}

/**
 * Create elements of Action type, fill necessary items and push to the vector
 * of all actions for one sequence diagram.
 *
 * @param el node of a json file with all actions
 * for a sequence diagram
 * @param o structure of a sequence diagram
 */
void DiagramSequence::fillStructureAction(const json el, dgrmSeq_t& o) {
    for (auto x : el) {
        Action tmp;
        Program::pushCoords(x.at("coords"), tmp.coords);
        tmp.pushConnection(x);
        o.actions.push_back(tmp);
    }
}

/**
 * Write the data about all actors from Actor structure
 * to json file.
 *
 * @param j json file
 * @param ac vector of the actors for sequence diagram
 */
void DiagramSequence::addActorToFile(json& j, std::vector<Actor> ac) {
    int i = 0;

    for (auto& x : ac) {
        j["actors"][i++] =
        {
            {"actor" ,  x.name },
            //{"id", x.if},
            {"color",
                {
                    {"r" , x.color.r},
                    {"g" , x.color.g},
                    {"b" , x.color.b},
                    {"a" , x.color.a}
                }
            },
            {"coords",
                    {
                            {"x" , x.coords[0]},
                            {"y" , x.coords[1]}
                    }
            }
        };
    }
}

/**
 * Write the data about all actions from Action structure
 * to json file.
 *
 * @param j json file
 * @param ac vector of the actions for sequence diagram
 */
void DiagramSequence::addActionToFile(json& j, std::vector<Action> ac) {
    int i = 0;

    for (auto& x : ac) {
        j["actions"][i++] =
        {
            {"from" ,  x.from },
            //{"fromId", x.fromId},
            //{"toId", x.toId},
            {"to" , x.to },
            {"arrow" , x.arrow },
            {"msg", x.msg },
            {"type", x.type},
            {"coords",
                    {
                            {"x" , x.coords[0]},
                            {"y" , x.coords[1]}
                    }
            }
        };
    }
}
