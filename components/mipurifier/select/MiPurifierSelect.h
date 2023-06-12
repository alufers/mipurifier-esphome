#ifndef MIPURIFIERSELECT_H_
#define MIPURIFIERSELECT_H_

#include <string>

#include "esphome.h"

#include "esphome/components/select/select.h"
#include "../mipurifier.h"

using namespace esphome;

enum class MiPurifierSelectRole
{
   SCREEN_BRIGHTNESS,
   MODE,
};

class MiPurifier;

class MiPurifierSelect : public Component, public select::Select
{
public:
    MiPurifierSelectRole role;
    MiPurifierSelect(MiPurifier *mipurifier, MiPurifierSelectRole role) : Component(), select::Select(), mipurifier(mipurifier) {
        this->role = role;
    }
    void control(const std::string &value);

private:
    MiPurifier *mipurifier;
};

#endif // MIPURIFIERSELECT_H_
