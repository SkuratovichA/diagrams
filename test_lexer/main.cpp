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
    CLASS_T, STARTUML_T, ENDUML_T, GENERALIZATION_T,
    ASSOCIATION_T, COMPOSITION_T, AGREGATION_T,
    ATTRIBUTE_T, METHOD_T, INTERFACE_T, NOTE_T,
    OF_T, END_T, PACKAGE_T, ABSTRACT_T, ENUM_T,
    HORIZONTAL_SEP_T, ACTOR_T, PARTICIPANT_T,
    REQUEST_T, RESPONSE_T, ALT_T, ELSE_T,
    ACTIVATE_T, DIACTIVATE_T, OVER_T, BOX_T,
    COLOR_T, RELATION_T, MSG_T, NAME_T,
    TAB_T, NL_T, DIR_T, CURV_LEFT_T,
    CURV_RIGHT_T, COLON_T
};

std::vector<std::pair<std::string, terminals>> const _map_ {
    { "^@class$",       CLASS_T },          { "^@startuml$",       STARTUML_T },     
    { "^@enduml$",      ENDUML_T },         { "^@<|--$",           GENERALIZATION_T },
    { "^@--$",          ASSOCIATION_T },    { "^@o--$",            COMPOSITION_T },
    { "^@*--$",         AGREGATION_T },     { "^attribute$",       ATTRIBUTE_T },
    { "^method",        METHOD_T },         { "^@interface$",      INTERFACE_T },
    { "^note$",         NOTE_T },           { "^of$",              OF_T },
    { "^end$",          END_T },            { "^package$",         PACKAGE_T },
    { "^abstract$",     ABSTRACT_T },       { "^@enum$",           ENUM_T },
    { "^..$",           HORIZONTAL_SEP_T }, { "^@actor$",          ACTOR_T },
    { "^@participant$", PARTICIPANT_T },    { "^->$",              REQUEST_T },
    { "^<--$",          RESPONSE_T },       { "^alt$",             ALT_T },
    { "^else$",         ELSE_T },           { "^activate$",        ACTIVATE_T },
    { "^diactivate$",   DIACTIVATE_T },     { "^over$",            OVER_T },
    { "^box$",          BOX_T },            { "^>$",               DIR_T },
    { "^:$",            COLON_T },          { "^\\{$",             CURV_LEFT_T },
    { "^}$",            CURV_RIGHT_T },     { "^#[0-9a-fA-f]{6}$", COLOR_T }, 
    { "^\".+\"$",       RELATION_T },       { "^[.+]$",            MSG_T }, 
    { "\\w",            NAME_T }, 
};

void print_word(std::string str, int idx) {
    std::cout << str << " : " << idx << "\n";
}

std::string get_str(int &i, std::string str) {
    std::string word;
    char arr[4] = {'\t', '\n', ' ', '\0'};
    char *tmp;

    while(1) {
        tmp = std::find(std::begin(arr), std::end(arr), str[i]);
        if (tmp != std::end(arr))
            return word;

        word.push_back(str[i++]);
    }
}

void check_regexp(std::string word) {
    for(auto const& [key, val] : _map_) {
        std::regex str_expr(key);

        if (regex_search(word, str_expr)) {
            print_word(word, val);
            return;
        }
    }
            
    std::cout << "\n\n\nI DONT KNOW THIS: " << word << "\n";
    exit(-1);
}

int main(int argc, char **argv) {
    std::ifstream t(argv[1]);
    std::string text((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    for (int i = 0; text[i] != '\0'; i++) {
        switch(text[i]) {
            case ' ':
                break;
            case '\n':
                check_regexp(get_str(i, text));
                std::cout << "NEW LINE : " << NL_T << "\n";
                break;
            case '\t':
                std::cout << "TAB : " << TAB_T << "\n";
                break;
            default:
                check_regexp(get_str(i, text));
        }
    }

    std::cout << "\n\n\nALL_GOOD\n";
    return EXIT_SUCCESS;
}
