#include "parse.h"

int main(int argc, char **argv) {
    Program prg;

    std::ifstream f(argv[1], std::ifstream::in);
    json j;
    f >> j;

    prg.parse_file(j);
    prg.fill_file();

    return 0;
}
