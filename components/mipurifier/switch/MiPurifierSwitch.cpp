#include "MiPurifierSwitch.h"

void MiPurifierSwitch::write_state(bool state)
{
    if (this->role == MiPurifierSwitchRole::BEEPER)
    {
        this->mipurifier->set_beeper(state);
    }
    else if (this->role == MiPurifierSwitchRole::LOCK)
    {
        this->mipurifier->set_lock(state);
    }
}
