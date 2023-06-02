#include "mipurifier.h"

int MiPurifier::readline(int readch, char *buffer, int len)
{
    static int pos = 0;
    int rpos;

    if (readch > 0)
    {
        switch (readch)
        {
        case '\r': // Return on CR
            rpos = pos;
            pos = 0; // Reset position index ready for next time
            return rpos;
        default:
            if (pos < len - 1)
            {
                buffer[pos++] = readch;
                buffer[pos] = 0;
            }
        }
    }
    // No end of line has been found, so return -1.
    return -1;
}

void MiPurifier::set_mode(std::string mode)
{
    // 0: auto, 1: sleep, 2: manual, 3: low, 4: med, 5: high
    if (mode == "auto")
    {
        strcpy(send_buffer, "down set_properties 2 5 0");
    }
    else if (mode == "night")
    {
        strcpy(send_buffer, "down set_properties 2 5 1");
    }
    else if (mode == "manual")
    {
        strcpy(send_buffer, "down set_properties 2 5 2");
    }
    else if (mode == "low")
    {
        strcpy(send_buffer, "down set_properties 2 4 1");
    }
    else if (mode == "medium")
    {
        strcpy(send_buffer, "down set_properties 2 4 2");
    }
    else if (mode == "high")
    {
        strcpy(send_buffer, "down set_properties 2 4 3");
    }
}

void MiPurifier::set_brightness(std::string brightness)
{
    if (brightness == "off")
    {
        strcpy(send_buffer, "down set_properties 6 1 2");
    }
    else if (brightness == "low")
    {
        strcpy(send_buffer, "down set_properties 6 1 1");
    }
    else if (brightness == "high")
    {
        strcpy(send_buffer, "down set_properties 6 1 0");
    }
}


void MiPurifier::set_lock(bool lock)
{
    if (lock)
    {
        strcpy(send_buffer, "down set_properties 7 1 true");
    }
    else
    {
        strcpy(send_buffer, "down set_properties 7 1 false");
    }
}

void MiPurifier::set_beeper(bool beeper)
{
    if (beeper)
    {
        strcpy(send_buffer, "down set_properties 5 1 true");
    }
    else
    {
        strcpy(send_buffer, "down set_properties 5 1 false");
    }
}

void MiPurifier::update_property(char *id, char *val)
{
    if (strcmp(id, "36") == 0)
    {
        airquality_sensor->publish_state(atof(val));
    }
    else if (strcmp(id, "37") == 0)
    {
        humidity_sensor->publish_state(atof(val));
    }
    else if (strcmp(id, "38") == 0)
    {
        temperature_sensor->publish_state(atof(val));
    }
    else if (strcmp(id, "43") == 0)
    {
        filterlife_sensor->publish_state(atof(val));
    }
    else if (strcmp(id, "22") == 0)
    {
        // power (on, off)
        // power_switch->publish_state(strcmp(val, "true") == 0);
    }
    else if (strcmp(id, "25") == 0)
    {
        // mode (auto, night, manual, preset)
        is_preset = false;
        switch (atoi(val))
        {
        case 0:
            // mode_select->publish_state("auto");
            break;
        case 1:
            // mode_select->publish_state("night");
            break;
        case 2:
            // mode_select->publish_state("manual");
            break;
        case 3:
            is_preset = true;
            break;
        }
    }
    else if (strcmp(id, "24") == 0)
    {
        // preset (low, medium, high)
        if (is_preset)
        {
            switch (atoi(val))
            {
            case 1:
                // mode_select->publish_state("low");
                break;
            case 2:
                // mode_select->publish_state("medium");
                break;
            case 3:
                // mode_select->publish_state("high");
                break;
            }
        }
    }
    else if (strcmp(id, "51") == 0)
    {
        // beeper (on, off)
        beeper_switch->publish_state(strcmp(val, "true") == 0);
    }
    else if (strcmp(id, "71") == 0)
    {
        // lock (on, off)
        lock_switch->publish_state(strcmp(val, "true") == 0);
    }
    else if (strcmp(id, "61") == 0)
    {
        // display brightness (off, low, high)
        switch (atoi(val))
        {
        case 0:
            // brightness_select->publish_state("high");
            break;
        case 1:
            // brightness_select->publish_state("low");
            break;
        case 2:
            // brightness_select->publish_state("off");
            break;
        }
    }
    else if (strcmp(id, "1010") == 0)
    {
        // manual speed
        // manualspeed->publish_state(atof(val) + 1);
    }
}

void MiPurifier::setup()
{
    register_service(&MiPurifier::send_command, "send_command", {"command"});
    // get initial state & settings
    strcpy(send_buffer, "down get_properties 2 2 2 5 5 1 6 1 7 1 10 10");
}

void MiPurifier::loop()
{
    while (available())
    {
        if (readline(read(), recv_buffer, max_line_length) > 0)
        {
            char *cmd = strtok(recv_buffer, " ");
            if (strcmp(cmd, "net") == 0)
            {
                this->write_str("local");
            }
            else if (strcmp(cmd, "time") == 0)
            {
                write_str("0");
            }
            else if (strcmp(cmd, "get_down") == 0)
            {
                // send command from send_buffer
                if (strlen(send_buffer) > 0)
                {
                    write_str(send_buffer);
                    send_buffer[0] = '\0';
                    ESP_LOGD("mipurifier", "sent send_buffer");
                }
                else if (millis() - last_heartbeat > 60000)
                {
                    // send mysterious heartbeat message
                    write_str("down set_properties 13 9 60");
                    last_heartbeat = millis();
                    ESP_LOGD("purifier", "sent heartbeat");
                }
                else if (millis() - last_query > 60000)
                {
                    // force sensor update
                    write_str("down get_properties 3 6 3 7 3 8 4 3");
                    last_query = millis();
                    ESP_LOGD("purifier", "sent query string");
                }
                else
                {
                    write_str("down none");
                }
            }
            else if (strcmp(cmd, "properties_changed") == 0)
            {
                ESP_LOGD("mipurifier", "parsing properties_changed message");
                char *id1 = strtok(NULL, " ");
                char *id2 = strtok(NULL, " ");
                char *id = strcat(id1, id2);
                char *val = strtok(NULL, " ");
                update_property(id, val);
                write_str("ok");
            }
            else if (strcmp(cmd, "result") == 0)
            {
                // loop over all properties and update
                ESP_LOGD("mipurifier", "parsing result message");
                char *id1, *id2, *id, *val;
                while (true)
                {
                    if (!(id1 = strtok(NULL, " ")))
                        break;
                    if (!(id2 = strtok(NULL, " ")))
                        break;
                    id = strcat(id1, id2);
                    strtok(NULL, " "); // skip 0
                    if (!(val = strtok(NULL, " ")))
                        break;
                    update_property(id, val);
                }
                write_str("ok");
            }
            else
            {
                // just acknowledge any other message
                write_str("ok");
            }
        }
    }
}

void MiPurifier::set_switch(MiPurifierSwitch *sw)
{

    if (sw->role == MiPurifierSwitchRole::BEEPER)
    {
        this->beeper_switch = sw;
    }
    else if (sw->role == MiPurifierSwitchRole::LOCK)
    {
        this->lock_switch = sw;
    }
}
