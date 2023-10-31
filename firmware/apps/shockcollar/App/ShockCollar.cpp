#include "ShockCollar.hpp"
#include "ShockCollarRF.hpp"

#include <stringdb.h>


void ShockCollar::begin()
{    
    ShockCollarSetup();

    Menu::begin();
}

void ShockCollar::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SHOCK_COLLAR));

    {
        STRING_CACHE();

        menu.add("VIBRATE", 
        [&menu, this](){ 
            ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_VIBRATE, 99 );
            TransmitShockCollarMsg( msg );
            STRING_CACHE();
            // string shit
            STRING_CLEAR();
        });

        menu.add("SHOCK", 
        [&menu, this](){ 
            ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_SHOCK, 10 );
            TransmitShockCollarMsg( msg );
            STRING_CACHE();
            // string shit
            STRING_CLEAR();
        });

        menu.add("BEEP", 
        [&menu, this](){ 
            ShockCollarMsg msg = BuildShockCollarMsg( RemoteSecret, 0, TYPE_BEEP, 0 );
            TransmitShockCollarMsg( msg );
            STRING_CACHE();
            // string shit
            STRING_CLEAR();
        });

        STRING_CLEAR();
    }
}
