#include "parse.h"

void Action::fill_connection(const json el) {
    this->from  = el.at("from" ).get<std::string>();
    this->arrow = el.at("arrow").get<std::string>();
    this->to    = el.at("to"   ).get<std::string>();
    this->msg   = el.at("msg"  ).get<std::string>();
}

void Activate::fill_activate(const json el) {
    this->who = el.at("who").get<std::string>();
}

void DiagramSequence::fill_structure_actor(const json el, dgrm_seq_t& o) {
    for (auto x : el) {
        o.actors.push_back(x.at("actor").get<std::string>());
    }
}

void DiagramSequence::fill_structure_action(const json el, dgrm_seq_t& o) {
    for (auto x : el) {
        Action tmp;
        tmp.fill_connection(x);
        o.actions.push_back(tmp);
    }
}

void DiagramSequence::fill_structure_activate(const json el, dgrm_seq_t& o) {
    for (auto x : el) {
        Activate tmp;
        Program::push_coords(x.at("coords"), tmp.coords);
        tmp.fill_activate(x);
        o.activates.push_back(tmp);
    }
}

void DiagramSequence::add_actor_to_file(json& j, std::vector<std::string> ac) {
    int i = 0;

    for (auto& x : ac) {
        j["actors"][i++] =
        {
            {"actor" ,  x }
        };
    }
}

void DiagramSequence::add_action_to_file(json& j, std::vector<Action> ac) {
    int i = 0;

    for (auto& x : ac) {
        j["actions"][i++] =
        {
            {"from" ,  x.from },
            {"to" , x.to },
            {"arrow" , x.arrow },
            {"msg", x.msg }
        };
    }
}

void DiagramSequence::add_activate_to_file(json& j, std::vector<Activate> ac) {
    int i = 0;

    for (auto& x : ac) {
        j["activates"][i++] =
        {
            {"coords",
                {
                    {"x" , x.coords[0]},
                    {"y" , x.coords[1]}
                }
            },
            {"who" , x.who }
        };
    }
}
