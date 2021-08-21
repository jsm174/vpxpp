#include "PinTable.h"
#include <stdio.h>

int main(int argc, char ** argv) {
    PinTable* pPinTable = new PinTable();
    
    //std::string filename = "/Users/jmillard/git/vpxpp/tables/exampleTable.vpx";
    //std::string filename = "/Users/jmillard/git/vpxpp/tables/blankTable.vpx";
    std::string filename = "/Users/jmillard/Desktop/Terminator 2 (Williams 1991).vpx";

    if (argc > 1) { 
        filename = argv[1]; 
    }

    pPinTable->LoadGameFromFilename(filename.c_str());

    printf("done");
}