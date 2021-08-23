#include <stdio.h>
#include <map>
#include "EditableRegistry.h"

#include "PinTable.h"
#include "Bumper.h"
#include "Decal.h"
#include "DispReel.h"
#include "Flasher.h"
#include "Flipper.h"
#include "Gate.h"
#include "Kicker.h"
#include "Light.h"
#include "LightSeq.h"
#include "Plunger.h"
#include "Primitive.h"
#include "Ramp.h"
#include "Rubber.h"
#include "Spinner.h"
#include "Surface.h"
#include "Textbox.h"
#include "Timer.h"
#include "Trigger.h"
#include "HitTarget.h"

std::map<ItemTypeEnum, EditableInfo> EditableRegistry::m_map;

int main(int argc, char ** argv) {
    
    EditableRegistry::RegisterEditable<Bumper>();
    EditableRegistry::RegisterEditable<Decal>();
    EditableRegistry::RegisterEditable<DispReel>();
    EditableRegistry::RegisterEditable<Flasher>();
    EditableRegistry::RegisterEditable<Flipper>();
    EditableRegistry::RegisterEditable<Gate>();
    EditableRegistry::RegisterEditable<Kicker>();
    EditableRegistry::RegisterEditable<Light>();
    EditableRegistry::RegisterEditable<LightSeq>();
    EditableRegistry::RegisterEditable<Plunger>();
    EditableRegistry::RegisterEditable<Primitive>();
    EditableRegistry::RegisterEditable<Ramp>();
    EditableRegistry::RegisterEditable<Rubber>();
    EditableRegistry::RegisterEditable<Spinner>();
    EditableRegistry::RegisterEditable<Surface>();
    EditableRegistry::RegisterEditable<Textbox>();
    EditableRegistry::RegisterEditable<Timer>();
    EditableRegistry::RegisterEditable<Trigger>();
    EditableRegistry::RegisterEditable<HitTarget>();

    PinTable* pPinTable = new PinTable();
    
    std::string filename = "/Users/jmillard/git/vpxpp/tables/exampleTable.vpx";
    //std::string filename = "/Users/jmillard/git/vpxpp/tables/blankTable.vpx";
    //std::string filename = "/Users/jmillard/Desktop/Terminator 2 (Williams 1991).vpx";

    if (argc > 1) { 
        filename = argv[1]; 
    }

    pPinTable->LoadGameFromFilename(filename.c_str());

    printf("done");
}