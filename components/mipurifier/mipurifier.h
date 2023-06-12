#ifndef ESPHOME_MIPURIFIER_H
#define ESPHOME_MIPURIFIER_H
#include <string>
#include "esphome.h"

#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "switch/MiPurifierSwitch.h"
#include "select/MiPurifierSelect.h"


// logging macros won't work if we don't do this
using namespace esphome;

class MiPurifierSwitch;
class MiPurifierSelect;

class MiPurifier : public Component, public uart::UARTDevice
{
public:
  static const int max_line_length = 80;
  char recv_buffer[max_line_length];
  char send_buffer[max_line_length];
  bool is_preset;
  int last_heartbeat, last_query;


  int readline(int readch, char *buffer, int len);

  // only run setup() after a Wi-Fi connection has been established successfully
  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }



  void set_power(bool power);
  void set_beeper(bool beeper);
  void set_lock(bool lock);

  void set_mode(std::string mode);
  void set_brightness(std::string brightness);

  void set_manualspeed(int speed)
  {
    snprintf(send_buffer, max_line_length, "down set_properties 10 10 %i", speed);
  }

  void send_command(std::string s);

  void update_property(char *id, char *val);

  void setup() override;
  void loop() override;

  void set_airquality_sensor(sensor::Sensor *airquality_sensor) { this->airquality_sensor = airquality_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { this->humidity_sensor = humidity_sensor; }
  void set_filter_life_sensor(sensor::Sensor *filter_life_sensor) { this->filter_life_sensor = filter_life_sensor; }
  void set_switch(MiPurifierSwitch *sw);
  void set_select(MiPurifierSelect *sel);

protected:
  sensor::Sensor *airquality_sensor = NULL;
  sensor::Sensor *humidity_sensor = NULL;
  sensor::Sensor *temperature_sensor = NULL; 
  sensor::Sensor *filter_life_sensor = NULL;
  MiPurifierSwitch *power_switch = NULL;
  MiPurifierSwitch *lock_switch = NULL;
  MiPurifierSwitch *beeper_switch = NULL;

  MiPurifierSelect *mode_select = NULL;
  MiPurifierSelect *brightness_select = NULL;

  // esphome::switch_::Switch *beeper_switch = NULL;
};
#endif
