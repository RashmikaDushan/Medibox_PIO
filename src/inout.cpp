#include <functions.h>
#include <definitions.h>
#include <globals.h>

// object Declarations
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display

// function to print a line of text in a given text size and the given position
void print_line(String text, int text_size, int row, int column, bool printImmediatly)
{
  display.setTextSize(text_size);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(column, row);      // Start at (row, column)
  display.println(text);
  if (printImmediatly)
  {
    display.display();
  }
}

// function to wait for button press in the menu
int button_press()
{
  if (digitalRead(UP) == LOW)
  {
    Serial.println("UP");
    delay(500);
    return UP;
  }

  else if (digitalRead(DOWN) == LOW)
  {
    Serial.println("DOWN");
    delay(500);
    return DOWN;
  }
  else if (digitalRead(CANCEL) == LOW)
  {
    Serial.println("CANCEL");
    delay(500);
    return CANCEL;
  }
  else if (digitalRead(OK) == LOW)
  {
    Serial.println("OK");
    delay(500);
    return OK;
  }
  return 0;
}
