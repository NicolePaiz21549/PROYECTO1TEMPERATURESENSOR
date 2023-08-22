#include <Arduino.h>
#include "driver/ledc.h"
#include "esp_adc_cal.h"

#define SNLM35 35
#define BTN_TEMP 13 // Change this to the actual button pin number

float TempC_LM35 = 0.0;
int temp = 0;
int placeValuesofTemp[4];

unsigned long time_now = 0;
bool buttonPressed = false;
bool temperatureTaken = false;
unsigned long lastButtonPressTime = 0;
unsigned long debounceDelay = 50;
bool displaysOn = false;
int lastButtonState = HIGH;

uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void setup() {
  analogReadResolution(12);

  pinMode(BTN_TEMP, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  int reading = digitalRead(BTN_TEMP);
  unsigned long currentTime = millis();

  if (!displaysOn && reading == LOW) {
    displaysOn = true;
  }

  if (displaysOn) {
    if (reading != lastButtonState) {
      lastButtonState = reading;

      if (reading == LOW && (currentTime - lastButtonPressTime) > debounceDelay) {
        lastButtonPressTime = currentTime;
        
        // Update temperature when the button is pressed again
        temperatureTaken = false;
      }
    }
  }

  // Read and display the temperature if the button has been pressed
  if (displaysOn && !temperatureTaken && reading == LOW) {
    int SNLM35_Raw = analogRead(SNLM35);
    float Voltage = readADC_Cal(SNLM35_Raw);
    TempC_LM35 = Voltage / 10;

    temp = TempC_LM35 * 100;

    placeValuesofTemp[3] = ((temp) / 1) % 10;
    placeValuesofTemp[2] = ((temp) / 10) % 10;
    placeValuesofTemp[1] = ((temp) / 100) % 10;
    placeValuesofTemp[0] = ((temp) / 1000) % 10;

    Serial.print("Temperature: ");
    Serial.println(TempC_LM35);
    temperatureTaken = true;
  }
}

