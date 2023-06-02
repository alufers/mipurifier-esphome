#ifndef ESPHOME_MIPURIFIER_H
#define ESPHOME_MIPURIFIER_H
#include <string>
#include "esphome.h"

#include "esphome/components/uart/uart.h"
#include "switch/MiPurifierSwitch.h"

// logging macros won't work if we don't do this
using namespace esphome;

class MiPurifierSwitch;

class MiPurifier : public Component, public uart::UARTDevice, public api::CustomAPIDevice
{
public:
  static const int max_line_length = 80;
  char recv_buffer[max_line_length];
  char send_buffer[max_line_length];
  bool is_preset;
  int last_heartbeat, last_query;

  // MiPurifier(UARTComponent *uart) : UARTDevice(uart) {}

  int readline(int readch, char *buffer, int len);

  // only run setup() after a Wi-Fi connection has been established successfully
  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

  void turn_on()
  {
    strcpy(send_buffer, "down set_properties 2 2 true");
  }

  void turn_off()
  {
    strcpy(send_buffer, "down set_properties 2 2 false");
  }



  void set_beeper(bool beeper);
  void set_lock(bool lock);

  void set_mode(std::string mode);
  void set_brightness(std::string brightness);

  void set_manualspeed(int speed)
  {
    snprintf(send_buffer, max_line_length, "down set_properties 10 10 %i", speed);
  }

  void send_command(std::string s)
  {
    strcpy(send_buffer, s.c_str());
  }

  void update_property(char *id, char *val);

  void setup() override;
  void loop() override;

  void set_airquality_sensor(sensor::Sensor *airquality_sensor) { this->airquality_sensor = airquality_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { this->humidity_sensor = humidity_sensor; }
  void set_switch(MiPurifierSwitch *sw);

protected:
  sensor::Sensor *airquality_sensor = NULL;  // new sensor::Sensor();
  sensor::Sensor *humidity_sensor = NULL;    // new sensor::Sensor();
  sensor::Sensor *temperature_sensor = NULL; // new sensor::Sensor();
  sensor::Sensor *filterlife_sensor = NULL;  // new sensor::Sensor();
  MiPurifierSwitch *lock_switch = NULL;
  MiPurifierSwitch *beeper_switch = NULL;

  // esphome::switch_::Switch *beeper_switch = NULL;
};
#endif
