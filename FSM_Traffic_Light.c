#include <Wire.h>
#include <LiquidCrystal_I2C.h>

typedef enum 
{
  RED,
  YELLOW,
  GREEN,
} State;

int const red_PIN = 27;
int const yellow_PIN = 26;
int const green_PIN = 33;
int const buttonPIN = 14;

State currentState = RED;
State previousState = RED;

int buttonState = LOW;

unsigned long startTime = 0; 
// Initialize I2C 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() 
{
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn LCD backlight ON
  lcd.print("Press button for green");
  // Activates comunication between Laptop & ESP32
  Serial.begin(115200);
  // Initialize LED pins to OUTPUT
  pinMode(red_PIN, OUTPUT);
  pinMode(yellow_PIN, OUTPUT);
  pinMode(green_PIN, OUTPUT);
  // Initialize BUTTON pin to INPUT
  pinMode(buttonPIN, INPUT);
}

void loop() 
{
  // read the state of the button value
  buttonState = digitalRead(buttonPIN);
  delay(50);

  switch (currentState)
  {
  case RED:
    digitalWrite(red_PIN, HIGH); // TURN RED LED ON
    if (buttonState == HIGH)
    {
      if (millis() - startTime >= 3000)
      {
        digitalWrite(red_PIN, LOW); // Turn RED LED OFF
        currentState = YELLOW;
        startTime = millis();
      }
    }
    // Wait 60 seconds to turn Yellow
    else if (millis() - startTime >= 60000)
    {
      digitalWrite(red_PIN, LOW); // Turn RED LED OFF
      currentState = YELLOW;
      startTime = millis();
    }
    break;

  case YELLOW:
    digitalWrite(yellow_PIN, HIGH); // TURN YELLOW LED ON
    if (millis() - startTime >= 3000 && previousState == RED)
    {
      digitalWrite(yellow_PIN, LOW); // TURN YELLOW LED 0FF
      currentState = GREEN;
      previousState = YELLOW;
      startTime = millis(); 
    }
    else if (millis() - startTime >= 3000 && previousState == GREEN)
    {
      digitalWrite(yellow_PIN, LOW); // TURN YELLOW LED 0FF
      currentState = RED;
      previousState = YELLOW;
      startTime = millis(); 
    }
    break;

  case GREEN: 
    digitalWrite(green_PIN, HIGH); // TURN GREEN PIN ON
    if (millis() - startTime >= 45000 && previousState == YELLOW)
    {
      digitalWrite(green_PIN, LOW); // TURN GREEN PIN OFF
      currentState = YELLOW;
      previousState = GREEN;
      startTime = millis();
    }
    break;
  } 
}
