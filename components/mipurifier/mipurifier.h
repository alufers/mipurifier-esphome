#ifndef ESPHOME_MIPURIFIER_H
#define ESPHOME_MIPURIFIER_H
#include "esphome.h"


// logging macros won't work if we don't do this
using namespace esphome;


class MiPurifier : public Component, public UARTDevice, public CustomAPIDevice
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

  void enable_beeper()
  {
    strcpy(send_buffer, "down set_properties 5 1 true");
  }

  void disable_beeper()
  {
    strcpy(send_buffer, "down set_properties 5 1 false");
  }

  void lock()
  {
    strcpy(send_buffer, "down set_properties 7 1 true");
  }

  void unlock()
  {
    strcpy(send_buffer, "down set_properties 7 1 false");
  }

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

protected:
  Sensor *airquality_sensor = new Sensor();
  Sensor *humidity_sensor = new Sensor();
  Sensor *temperature_sensor = new Sensor();
  Sensor *filterlife_sensor = new Sensor();
};
#endif
