#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <streambuf>
#include <fstream>

enum terminals {
    END_PROGRAM = -1, CLASS_T, STARTUML_T, ENDUML_T,
    GENERALIZATION_T, UNEXPECTED_T,
    ASSOCIATION_T, COMPOSITION_T, AGREGATION_T,
    ATTRIBUTE_T, METHOD_T, INTERFACE_T, NOTE_T,
    OF_T, END_T, PACKAGE_T, ABSTRACT_T, ENUM_T,
    HORIZONTAL_SEP_T, ACTOR_T, PARTICIPANT_T,
    REQUEST_T, RESPONSE_T, ALT_T, ELSE_T,
    ACTIVATE_T, DIACTIVATE_T, OVER_T, BOX_T,
    COLOR_T, RELATION_T, MSG_T, NAME_T,
    NL_T, DIR_T, CURV_LEFT_T, CURV_RIGHT_T,
    COLON_T, OBJECT_T, STATIC_T, PUBLIC_T,
    PROTECTED_T, PRIVATE_T, PACKAGE_MOD_T
};

typedef struct Text {
    std::string text;
    int cur_idx;
    terminals cur_token;
} text_t;

std::vector<std::pair<std::string, terminals>> const _map_ {
    { "^\\+$",             PUBLIC_T },         { "^\\#$",             PROTECTED_T },
    { "^\\-$",             PRIVATE_T },        { "^\\~$",             PACKAGE_MOD_T },
    { "^@class$",          CLASS_T },          { "^@startuml_diagram$", STARTUML_T },
    { "^@enduml_diagram$", ENDUML_T },         { "^@<|--$",           GENERALIZATION_T },
    { "^@--$",             ASSOCIATION_T },    { "^@o--$",            COMPOSITION_T },
    { "^@*--$",            AGREGATION_T },     { "^attribute$",       ATTRIBUTE_T },
    { "^method",           METHOD_T },         { "^@interface$",      INTERFACE_T },
    { "^note$",            NOTE_T },           { "^of$",              OF_T },
    { "^end$",             END_T },            { "^package$",         PACKAGE_T },
    { "^abstract$",        ABSTRACT_T },       { "^@enum$",           ENUM_T },
    { "^\\.\\.$",          HORIZONTAL_SEP_T }, { "^@actor$",          ACTOR_T },
    { "^@participant$",    PARTICIPANT_T },    { "^->$",              REQUEST_T },
    { "^<--$",             RESPONSE_T },       { "^alt$",             ALT_T },
    { "^else$",            ELSE_T },           { "^activate$",        ACTIVATE_T },
    { "^diactivate$",      DIACTIVATE_T },     { "^over$",            OVER_T },
    { "^@object$",         OBJECT_T } ,        { "^box$",             BOX_T },
    { "^>$",               DIR_T },            { "^:$",               COLON_T },
    { "^\\{static\\}$",    STATIC_T },         { "^\\{$",             CURV_LEFT_T },
    { "^\\}$",             CURV_RIGHT_T },     { "^#[0-9a-fA-f]{6}$", COLOR_T },
    { "^\".+\"$",          RELATION_T },       { "^\\[.+\\]$",            MSG_T },
    { "\\w",               NAME_T },
};

void static_(text_t &Text);
void mod(text_t &Text);
void attrs(text_t &Text);
void action(text_t &Text);
void arrow(text_t &Text);
void relation(text_t &Text);
void stat_diagram(text_t &Text);
void program(text_t &Text);
std::string get_str(int &i, text_t &Text);
terminals check_regexp(std::string word);
void get_token(text_t &Text);

#define CHECK_TOKEN(cur_token, exp_token, Text, msg_err) \
        if ((cur_token) != (exp_token)) {                \
            printf((msg_err));                           \
            exit(-1);                                    \
        }                                                \
        else {                                           \
            get_token((Text));                           \
        }

void program(text_t &Text) {
    get_token(Text);
    CHECK_TOKEN(Text.cur_token, STARTUML_T, Text,
                "err program start uml\n");
    CHECK_TOKEN(Text.cur_token, NL_T, Text,
                "err program new line\n");

    stat_diagram(Text);
}

void stat_diagram(text_t &Text) {
    if (Text.cur_token == CLASS_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, NAME_T, Text,
                    "err stat_diagram name\n");
        CHECK_TOKEN(Text.cur_token, CURV_LEFT_T, Text,
                    "err stat_diagram left brace\n");
        CHECK_TOKEN(Text.cur_token, NL_T, Text,
                    "err stat_diagram new line\n");

        attrs(Text);
        stat_diagram(Text);
    }
    else if (Text.cur_token == NAME_T) {
        get_token(Text);
        relation(Text);
        arrow(Text);
        relation(Text);

        CHECK_TOKEN(Text.cur_token, NAME_T, Text,
                    "err stat_diagram name\n");

        action(Text);

        CHECK_TOKEN(Text.cur_token, NL_T, Text,
                    "err stat_diagram new line\n");

        stat_diagram(Text);
    }

}

void relation(text_t &Text) {
    if (Text.cur_token == RELATION_T) {
        get_token(Text);
    }
}

void arrow(text_t &Text) {
    if (Text.cur_token == AGREGATION_T) {
        get_token(Text);
    }
    else if (Text.cur_token == COMPOSITION_T) {
        get_token(Text);
    }
    else if (Text.cur_token == GENERALIZATION_T) {
        get_token(Text);
    }
    else if (Text.cur_token == ASSOCIATION_T) {
        get_token(Text);
    }
    else {
        printf("err with arrow in arrow\n");
        exit(-1);
    }
}

void action(text_t &Text) {
    if (Text.cur_token == MSG_T) {
        get_token(Text);
    }
}

void attrs(text_t &Text) {
    if (Text.cur_token == CURV_RIGHT_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, NL_T, Text,
                    "err stat_diagram new line\n");

        stat_diagram(Text);
    }
    else {
        if (Text.cur_token == HORIZONTAL_SEP_T) {
            get_token(Text);
            CHECK_TOKEN(Text.cur_token, NL_T, Text,
                        "err stat_diagram new line\n");

            attrs(Text);
        }
        else {
            static_(Text);
            if (Text.cur_token == ATTRIBUTE_T) {
                get_token(Text);
                CHECK_TOKEN(Text.cur_token, COLON_T, Text,
                            "err attr colon name\n");

                mod(Text);

                CHECK_TOKEN(Text.cur_token, NAME_T, Text,
                            "err stat_diagram name\n");
                CHECK_TOKEN(Text.cur_token, NAME_T, Text,
                            "err stat_diagram name\n");
                CHECK_TOKEN(Text.cur_token, NL_T, Text,
                            "err stat_diagram new line\n");

                attrs(Text);
            }
            else if (Text.cur_token == METHOD_T) {
                get_token(Text);

                CHECK_TOKEN(Text.cur_token, COLON_T, Text,
                            "err attr colon name\n");
                CHECK_TOKEN(Text.cur_token, NAME_T, Text,
                            "err stat_diagram name\n");
                CHECK_TOKEN(Text.cur_token, MSG_T, Text,
                            "err attr message name\n");
                CHECK_TOKEN(Text.cur_token, NL_T, Text,
                            "err stat_diagram new line\n");

                attrs(Text);
            }
            else {
                printf("err attrs method/attr\n");
                exit(-1);
            }
        }
    }
}

void mod(text_t &Text) {
    if (Text.cur_token == PROTECTED_T) {
        get_token(Text);
    }
    else if (Text.cur_token == PACKAGE_MOD_T) {
        get_token(Text);
    }
    else if (Text.cur_token == PRIVATE_T) {
        get_token(Text);
    }
    else if (Text.cur_token == PUBLIC_T) {
        get_token(Text);
    }
}

void static_(text_t &Text) {
    if (Text.cur_token == STATIC_T) {
        get_token(Text);
    }
}

void print_word(std::string str, int idx) {
    std::cout << str << " : " << idx << "\n";
}

std::string get_str(int &i, text_t &Text) {
    std::string word;
    char arr[4] = {'\t', '\n', ' ', '\0'};
    char *tmp;

    while(1) {
        tmp = std::find(std::begin(arr), std::end(arr), Text.text[i]);
        if (tmp != std::end(arr)) {
            Text.cur_idx = i;
            return word;
        }

        word.push_back(Text.text[i++]);
    }
}

terminals check_regexp(std::string word) {
    for(auto const& [key, val] : _map_) {
        std::regex str_expr(key);

        if (regex_search(word, str_expr)) {
            print_word(word, val);
            return val;
        }
    }

    return UNEXPECTED_T;
    //std::cout << "\n\n\nI DONT KNOW THIS: " << word << "\n";
}

void get_token(text_t &Text) {
    //printf("|%c| %d\n", Text.text[Text.cur_idx], Text.cur_idx);
    for (int i = Text.cur_idx; Text.text[i] != '\0'; i++) {
        switch(Text.text[i]) {
            case ' ':case '\t':
                break;
            case '\n':
                Text.cur_token = NL_T;
                Text.cur_idx = ++i;
                return;
                //std::cout << "NEW LINE : " << NL_T << "\n";
                //break;
            default:
                Text.cur_token = check_regexp(get_str(i, Text));
                return;
        }
    }

    std::cout << "\n\n\nALL_GOOD\n";

    //return END_PROGRAM;
}

int main(int argc, char **argv) {
    std::ifstream t(argv[1]);
    std::string text((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    text_t Text;
    Text.text = text;
    Text.cur_idx = 0;

    program(Text);

    return 0;
}
