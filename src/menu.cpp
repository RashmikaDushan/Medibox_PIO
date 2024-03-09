#include <functions.h>
#include <globals.h>
#include <definitions.h>

// Main menu option number
int current_mode = 0;
// Number of main menu options
int max_modes = 7;
// Main menu options
String options[] = {"     1                Set Time",
                    "     2               Set Alarm     1",
                    "     3               Set Alarm     2",
                    "     4               Set Alarm     3",
                    "     5                Remove      Alarm",
                    "     6                 Set     Timezone",
                    "     7              Fetch Time from Wifi"};

// to toggle between items in the menu
void run_mode(int mode)
{
    if (mode == 0)
    {
        set_time();
    }
    else if (mode == 1 || mode == 2 || mode == 3)
    {
        set_alarm(mode - 1);
    }
    else if (mode == 4)
    {
        disable_all_alarms();
    }
    else if (mode == 5)
    {
        set_time_zone(0);
    }
    else if (mode == 6)
    {
        update_time_wifi();
    }
}

// function to navigate through the menu
void go_to_menu(void)
{
    while (true)
    {
        display.clearDisplay();
        print_line(options[current_mode], 2, 0, 0, true);
        int pressed = button_press();
        if (pressed == UP)
        {
            current_mode = (current_mode + 1) % max_modes;
        }
        else if (pressed == DOWN)
        {
            current_mode = (current_mode + max_modes - 1) % max_modes;
        }
        else if (pressed == OK)
        {
            Serial.println(current_mode);
            run_mode(current_mode);
        }
        else if (pressed == CANCEL)
        {
            return;
        }
    }
}
