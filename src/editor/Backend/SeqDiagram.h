/** @file SeqDiagram.h
 * @author Shchapaniak Andrei <xshcha00\@vutbr.cz>
 * @date 07.05.2022
 */


#pragma once

#include "ClassDiagram.h"

using json = nlohmann::json;

/**
 * @brief A class reprensenting a Sequence Diagram Item
 */
class Actor {
public:
    std::string name; ///< name of an actor (classname)
    Color color; ///< color of a class
    std::vector<double> coords; ///< coordinates where the box with the actor is placed
};

/**
 * @brief A class representing the sequence diagram connection
 */
class Action {
public:
    std::string from; ///< classname from
    std::string to; ///< classname to
    std::string arrow; ///< arrow type
    std::string msg; ///< message above the arrow
    std::vector<double> coords; ///< coordinates of a line
    int type;

    /**
     * @brief Set values to the structure Action from json file.
     * @param el node of a json file with attributes
     * for one action between 2 actors
     */
    void pushConnection(const json el);
};

/**
 * @brief Sequence diagram representation.
 */
typedef struct dgrm_seq {
    std::vector<Actor> actors; ///< all actors
    std::vector<Action> actions; ///< all actions
} dgrmSeq_t;


/**
 * @brief A class representing a sequence diagram item and performs actions such adding/filling SequenceDiagramItem class.
 */
class DiagramSequence {
public:
    /**
     * @brief Create elements of Actor type, fill necessary items and push to the vector
     * of all actors for one sequence diagram.
     * @param el node of a json file with all actors
     * for a sequence diagram
     * @param o structure of a sequence diagram
     */
    void fillStructureActor(const json el, dgrmSeq_t &o);

    /**
     * @brief Create elements of Action type, fill necessary items and push to the vector
     * of all actions for one sequence diagram.
     * @param el node of a json file with all actions
     * for a sequence diagram
     * @param o structure of a sequence diagram
     */
    void fillStructureAction(const json el, dgrmSeq_t &o);
    /**
     * @brief Write the data about all actors from Actor structure
     * to json file.
     * @param j json file
     * @param ac vector of the actors for sequence diagram
     */
    void addActorToFile(json &j, std::vector<Actor> ac);

    /**
     * @brief Write the data about all actions from Action structure
     * to json file.
     * @param j json file
     * @param ac vector of the actions for sequence diagram
     */
    void addActionToFile(json &j, std::vector<Action> ac);
};
