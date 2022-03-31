#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>

#include <streambuf>
#include <fstream>
#include <iomanip>

#define TERMS \
X(END_PROGRAM, "END OF PROGRAM") \
X(CLASS_T, "@CLASS") \
X(INTERFACE_T, "@INTERFACE") \
X(OBJECT_T, "@OBJECT") \
X(ENUM_T, "@ENUM") \
X(ABSTRACT_T, "@ABSTRACT") \
X(STARTUML_CLASS_T, "STARTUML_CLASS") \
X(ENDUML_CLASS_T, "ENDUML_CLASS") \
X(STARTUML_SEQ_T, "STARTUML_SEQ") \
X(ENDUML_SEQ_T, "ENDUML_SEQ") \
X(STARTMTH_T, "STARTMTH") \
X(ENDMTH_T, "ENDMTH") \
X(STARTACTMSG_T, "STARTACTMSG") \
X(ENDACTMSG_T, "ENDACTMSG_T") \
X(STARTACTION_T, "STARTACTION") \
X(ENDACTION_T, "ENDACTION") \
X(GENERALIZATION_T, "GENERALIZATION <|--") \
X(ASSOCIATION_T, "ASSOCIATION --") \
X(COMPOSITION_T, "COMPOSITION o--") \
X(AGREGATION_T, "AGREGATION *--") \
X(PACKAGE_MOD_T, "PACKAGE_MOD ~") \
X(PUBLIC_T, "PUBLIC_MOD +") \
X(PROTECTED_T, "PROTECTED_MOD #") \
X(PRIVATE_T, "PRIVATE_MOD -") \
X(ACTIVATE_T, "@ACTIVATE") \
X(DEACTIVATE_T, "@DEACTIVATE") \
X(ATTRIBUTE_T, "@ATTRIBUTE") \
X(METHOD_T, "@METHOD") \
X(REQUEST_T, "REQUEST ->") \
X(RESPONSE_T, "RESPONSE <--") \
X(ACTOR_T, "@ACTOR") \
X(PARTICIPANT_T, "@PARTICIPANT") \
X(CURV_LEFT_T, "BRACE {") \
X(CURV_RIGHT_T, "BRACE }") \
X(COLON_T, "COLON :") \
X(COLOR_T, "COLOR") \
X(ALT_END_T, "@ALTEND") \
X(ALT_START_T, "@ALTSTART") \
X(ELSE_T, "@ELSE") \
X(PACKAGE_T, "@PACKAGE") \
X(PACKAGEEND_T, "@PACKAGEEND") \
X(STARTNOTE_T, "@STARTNOTE") \
X(ENDNOTE_T, "@ENDNOTE") \
X(BOX_START_T, "@BOX_START") \
X(BOX_END_T, "@BOX_END") \
X(SEQ_SEP_T, "==") \
X(NAME_START_T, "@NAME_START") \
X(NAME_END_T, "@NAME_END") \
X(COND_START_T, "@COND_START") \
X(COND_END_T, "@COND_END") \
X(CONTINUE_LINE_T, "... continue") \
X(UNEXPECTED_T, "SOME ERROR") \
X(RELATION_T, "RELATION \"...\"") \
X(STATIC_T, "@STATIC") \
X(WORD_T, "WORD") \
X(START_SEP_T, "start sep") \
X(END_SEP_T, "end sep") \
X(WORD_M_T, "WORD with brackets name()") \
X(QUOTE_T, "QUOTE \"\"\"") \

#define X(term, name) term,
enum terminals : size_t
{
    TERMS
};
#undef X

#define X(term, name) name,
char const *term_name[] =
{
    TERMS
};
#undef X

/*enum terminals {
    END_PROGRAM = -1,
    CLASS_T, INTERFACE_T, OBJECT_T, ENUM_T, ABSTRACT_T,
    STARTUML_CLASS_T, ENDUML_CLASS_T,
    GENERALIZATION_T, ASSOCIATION_T, COMPOSITION_T, AGREGATION_T,
    STARTUML_SEQ_T, ENDUML_SEQ_T,
    PACKAGE_MOD_T, PUBLIC_T, PROTECTED_T, PRIVATE_T,
    ACTIVATE_T, DIACTIVATE_T,
    ATTRIBUTE_T, METHOD_T,
    REQUEST_T, RESPONSE_T,
    ACTOR_T, PARTICIPANT_T,
    CURV_LEFT_T, CURV_RIGHT_T
    COLON_T, NL_T,
    COLOR_T,
    ALT_T, ELSE_T,
    PACKAGE_T, BOX_T,
    UNEXPECTED_T,
    HORIZONTAL_SEP_T, RELATION_T, DIR_T, STATIC_T,
    WORD_T, QUOTE_T
};*/

typedef struct Text {
    std::string text;
    std::string token_to_push;
    std::string cur_word;
    char separator;
    int cur_idx;
    terminals cur_token;
} text_t;

std::vector<std::pair<std::string, terminals>> const _map_ {
    { "^@enduml_seq$", ENDUML_SEQ_T },
    { "^@startuml_seq$", STARTUML_SEQ_T },
    { "^@endmth$", ENDMTH_T },
    { "^@startmth$", STARTMTH_T },
    { "^@endaction$", ENDACTION_T },
    { "^@startaction$", STARTACTION_T },
    { "^@startactmsg$", STARTACTMSG_T },
    { "^@endactmsg$", ENDACTMSG_T },
    { "^\\+$", PUBLIC_T },
    { "^\\#$", PROTECTED_T },
    { "^\\-$", PRIVATE_T },
    { "^\\~$", PACKAGE_MOD_T },
    { "^@class$",          CLASS_T },
    { "^@startuml_class$", STARTUML_CLASS_T },
    { "^@enduml_class$",ENDUML_CLASS_T },
    { "^(\\<\\|\\-\\-|\\-\\-\\|\\>)$", GENERALIZATION_T },
    { "^(\\-\\-|\\-\\-)$", ASSOCIATION_T },
    { "^(o\\-\\-|\\-\\-o)$", COMPOSITION_T },
    { "^(\\*\\-\\-|\\-\\-\\*)$", AGREGATION_T },
    { "^@attribute$", ATTRIBUTE_T },
    { "^@method", METHOD_T },
    { "^@interface$", INTERFACE_T },
    { "^@package$", PACKAGE_T },
    { "^@packageend$", PACKAGEEND_T },
    { "^@abstract$", ABSTRACT_T },
    { "^@enum$", ENUM_T },
    { "^@actor$", ACTOR_T },
    { "^@participant$", PARTICIPANT_T },
    { "^(\\-\\>|\\<\\-)$", REQUEST_T },
    { "^(\\<\\-\\-|\\-\\-\\>)$", RESPONSE_T },
    { "^@altstart$", ALT_START_T },
    { "^@altend$", ALT_END_T },
    { "^@else$", ELSE_T },
    { "^@activate$", ACTIVATE_T },
    { "^@startnote$", STARTNOTE_T },
    { "^@endnote$", ENDNOTE_T },
    { "^@deactivate$", DEACTIVATE_T },
    { "^@object$", OBJECT_T } ,
    { "^@boxstart$", BOX_START_T },
    { "^@boxend$", BOX_END_T },
    { "^@condstart$", COND_START_T },
    { "^@condend$", COND_END_T },
    { "^@namestart$", NAME_START_T },
    { "^@nameend$", NAME_END_T },
    { "^\\=\\=$", SEQ_SEP_T },
    { "^\\:$", COLON_T },
    { "^@static$", STATIC_T },
    { "^\\{$", CURV_LEFT_T },
    { "^\\}$", CURV_RIGHT_T },
    { "^\\.\\.\\.$", CONTINUE_LINE_T },
    { "^@startsep$", START_SEP_T },
    { "^@endsep$", END_SEP_T },
    { "^#[0-9a-fA-f]{6}$", COLOR_T },
    { "^\\\"\\\"\\\"$", QUOTE_T } ,
    { "^\".+\"$", RELATION_T },
    { "\\w+\\(((\\w,)|\\w)*\\)$", WORD_M_T },
    { "\\w", WORD_T },
};

/*
note
msg_act
msg
sep_msg
u_u
cond_msg
questmsg
wordss
*/
void multiple_token(text_t &Text, terminals expected_term);

void type_(text_t &Text);
void color(text_t &Text);
void enum_(text_t &Text);
void startmth(text_t &Text);
void dir(text_t &Text);
void abstract(text_t &Text);
void static_(text_t &Text);
void mod(text_t &Text);
void attrs(text_t &Text);
void stat_seq(text_t &Text);
void action(text_t &Text);
void arrow(text_t &Text);
void relation(text_t &Text);
void stat_class(text_t &Text);
void program(text_t &Text);
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
    if (Text.cur_token == STARTUML_CLASS_T) {
        get_token(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == STARTUML_SEQ_T) {
        get_token(Text);
        stat_seq(Text);
    }
    else {
        printf("err with startuml\n");
        exit(-1);
    }
}

void stat_class(text_t &Text) {
    if (Text.cur_token == CLASS_T) {
        get_token(Text);
        abstract(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err class name\n");
        CHECK_TOKEN(Text.cur_token, CURV_LEFT_T, Text, "err class left brace\n");
        attrs(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == PACKAGE_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err package name\n");
        color(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == PACKAGEEND_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err packageend name\n");
        stat_class(Text);
    }
    else if (Text.cur_token == INTERFACE_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err interface name\n");
        CHECK_TOKEN(Text.cur_token, CURV_LEFT_T, Text, "err interface left brace\n");
        attrs(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == WORD_T) {
        get_token(Text);
        relation(Text);
        arrow(Text);
        relation(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err relation 2nd name\n");
        action(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == OBJECT_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err object name\n");
        stat_class(Text);
    }
    else if (Text.cur_token == STARTNOTE_T) {
        get_token(Text);
        multiple_token(Text, ENDNOTE_T);
        stat_class(Text);
    }
    else if (Text.cur_token == ENUM_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err enum name\n");
        CHECK_TOKEN(Text.cur_token, CURV_LEFT_T, Text, "err enum left brace\n");
        enum_(Text);
        stat_class(Text);
    }
    else if (Text.cur_token == ENDUML_CLASS_T) {
        // end of class diagram
        return;
    }
}

void stat_seq(text_t &Text) {
    if (Text.cur_token == ACTOR_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err actor name\n");
        stat_seq(Text);
    }
    else if (Text.cur_token == PARTICIPANT_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err paticipant name\n");
        stat_seq(Text);
    }
    else if (Text.cur_token == STARTNOTE_T) {
        get_token(Text);
        multiple_token(Text, ENDNOTE_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == CONTINUE_LINE_T) {
        get_token(Text);
        stat_seq(Text);
    }
    else if (Text.cur_token == START_SEP_T) {
        get_token(Text);
        multiple_token(Text, END_SEP_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == ACTIVATE_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err activate name\n");
        stat_seq(Text);
    }
    else if (Text.cur_token == DEACTIVATE_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err deactivate name\n");
        stat_seq(Text);
    }
    else if (Text.cur_token == WORD_T) {
        get_token(Text);
        type_(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err act 2nd name\n");
        CHECK_TOKEN(Text.cur_token, STARTACTMSG_T, Text, "err actmsg\n");
        multiple_token(Text, ENDACTMSG_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == SEQ_SEP_T) {
        get_token(Text);
        multiple_token(Text, SEQ_SEP_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == ALT_START_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, COND_START_T, Text, "err alt cond_start name\n");
        multiple_token(Text, COND_END_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == ELSE_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, COND_START_T, Text, "err else cond_start name\n");
        multiple_token(Text, COND_END_T);
        stat_seq(Text);
    }
    else if (Text.cur_token == ALT_END_T) {
        get_token(Text);
        stat_seq(Text);
    }
    else if (Text.cur_token == BOX_START_T) {
        get_token(Text);
        CHECK_TOKEN(Text.cur_token, NAME_START_T, Text, "err box name\n");
        multiple_token(Text, NAME_END_T);
        color(Text);
        stat_seq(Text);
    }
    else if (Text.cur_token == BOX_END_T) {
        get_token(Text);
        stat_seq(Text);
    }
    else if (Text.cur_token == ENDUML_SEQ_T) {
        // end of sequence diagram
        return;
    }
}

void multiple_token(text_t &Text, terminals expected_term) {
    if (Text.cur_token == expected_term) {
        get_token(Text);
        return;
    }

    get_token(Text);
    multiple_token(Text, expected_term);
}

void type_(text_t &Text) {
    if (Text.cur_token == RESPONSE_T) {
        get_token(Text);
    }
    else if (Text.cur_token == REQUEST_T) {
        get_token(Text);
    }
    else {
        printf("err type msg seq stat\n");
        exit(-1);
    }
}

void enum_(text_t &Text) {
    if (Text.cur_token == CURV_RIGHT_T) {
        get_token(Text);
        return;
    }

    CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err enum arg name\n");
    enum_(Text);
}

void abstract(text_t &Text) {
    if (Text.cur_token == ABSTRACT_T) {
        get_token(Text);
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
    if (Text.cur_token == STARTACTION_T) {
        get_token(Text);
        multiple_token(Text, ENDACTION_T);
    }
}

void color(text_t &Text) {
    if (Text.cur_token == COLOR_T) {
        get_token(Text);
    }
}

void attrs(text_t &Text) {

    if (Text.cur_token == CURV_RIGHT_T) {
        get_token(Text);
        return;
    }

    static_(Text);

    if (Text.cur_token == ATTRIBUTE_T) {
        get_token(Text);
        mod(Text);
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err attr type\n");
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err attr name\n");
        attrs(Text);
    }
    else if (Text.cur_token == METHOD_T) {
        get_token(Text);
        mod(Text);
        CHECK_TOKEN(Text.cur_token, WORD_M_T, Text, "err method name\n");
        CHECK_TOKEN(Text.cur_token, WORD_T, Text, "err method type\n");
        startmth(Text);
        attrs(Text);
    }
    else {
        printf("err attrs method/attr\n");
        exit(-1);
    }
}

void startmth(text_t &Text) {
    if (Text.cur_token == STARTMTH_T) {
        get_token(Text);
        multiple_token(Text, ENDMTH_T);
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
    std::cout << "token = " << term_name[idx] << std::setw(22-strlen(term_name[idx]))
              <<" : " << str << "\n";
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
}

void get_token(text_t &Text) {
    Text.cur_word.clear();
    for (int i = Text.cur_idx; Text.text[i] != '\0'; i++) {
        switch(Text.text[i]) {
            case ' ':case '\t':case '\n':
                Text.separator = Text.text[i];
                Text.cur_token = check_regexp(Text.cur_word);
                Text.cur_idx = i + 1;
                return;
            default:
                Text.cur_word.push_back(Text.text[i]);
        }
    }
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

int main(int argc, char **argv) {
    std::ifstream t(argv[1]);
    std::string text((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    std::string::iterator new_end = std::unique(text.begin(), text.end(), BothAreSpaces);
    text.erase(new_end, text.end());

    text_t Text;
    Text.text = text;
    Text.cur_idx = 0;
    Text.cur_word = "";
    Text.token_to_push = "";

    program(Text);
    std::cout << "\n\n\nALL_GOOD\n";

    return 0;
}
