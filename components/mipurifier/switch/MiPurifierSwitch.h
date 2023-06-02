#ifndef MIPURIFIERSWITCH_H_
#define MIPURIFIERSWITCH_H_

#include "esphome.h"

#include "esphome/components/switch/switch.h"
#include "../mipurifier.h"

using namespace esphome;

enum class MiPurifierSwitchRole
{
    BEEPER,
    LOCK,
    POWER,
};

class MiPurifier;

class MiPurifierSwitch : public Component, public switch_::Switch
{
public:
    MiPurifierSwitchRole role;
    MiPurifierSwitch(MiPurifier *mipurifier, MiPurifierSwitchRole role) : Component(), switch_::Switch(), mipurifier(mipurifier) {
        this->role = role;
    }
    void write_state(bool state);

private:
    MiPurifier *mipurifier;
};

#endif // MIPURIFIERSWITCH_H_
