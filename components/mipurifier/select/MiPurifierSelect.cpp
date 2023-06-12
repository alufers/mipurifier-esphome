#include "MiPurifierSelect.h"


void MiPurifierSelect::control(const std::string &value) {
    if(this->role == MiPurifierSelectRole::MODE) {
        this->mipurifier->set_mode(value);
    } else if(this->role == MiPurifierSelectRole::SCREEN_BRIGHTNESS) {
        this->mipurifier->set_brightness(value);
    }
}
