#pragma once

using json = nlohmann::json;

class Action {
public:
    std::string from;
    std::string to;
    std::string arrow;
    std::string msg;

    void fill_connection(const json el);
};

class Activate {
public:
    std::string who;
    std::vector<double> coords;

    void fill_activate(const json el);
};

typedef struct dgrm_seq {
    std::vector<std::string> actors;
    std::vector<Action> actions;
    std::vector<Activate> activates;
} dgrm_seq_t;

class DiagramSequence {
public:
    void fill_structure_actor(const json el, dgrm_seq_t& o);

    void fill_structure_action(const json el, dgrm_seq_t& o);

    void fill_structure_activate(const json el, dgrm_seq_t& o);

    void add_actor_to_file(json& j, std::vector<std::string> ac);

    void add_action_to_file(json& j, std::vector<Action> ac);

    void add_activate_to_file(json& j, std::vector<Activate> ac);
};
