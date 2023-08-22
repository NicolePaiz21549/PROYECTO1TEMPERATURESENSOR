#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 13
int LM35_Input = 0;
float TempC = 0.0;
float Voltage = 0.0;
bool buttonPressed = false;

// Variables for averaging temperature readings
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
long total = 0;

// Calibration ADC for ESP32
esp_adc_cal_characteristics_t adc_chars;

// Prototyping readADC_Cal function
uint32_t readADC_Cal(int ADC_Raw);

// Refresh interval and minimum button press delay
const unsigned long minButtonPressDelay = 1000; // Minimum 1 second between presses
unsigned long lastButtonPressTime = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

void loop()
{
  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_PIN) == LOW)
  {
    if (!buttonPressed && currentTime - lastButtonPressTime >= minButtonPressDelay)
    {
      buttonPressed = true;
      lastButtonPressTime = currentTime;

      total = total - readings[readIndex];
      readings[readIndex] = analogRead(LM35_GPIO_PIN);
      total = total + readings[readIndex];
      readIndex = (readIndex + 1) % numReadings;
      LM35_Input = total / numReadings;

      Voltage = readADC_Cal(LM35_Input);
      TempC = ((Voltage/4095)*3.3)/0.01;

      Serial.print("Temperatura en °C = ");
      Serial.print(TempC, 1);
    }
  }
  else
  {
    buttonPressed = false;
  }
}

uint32_t readADC_Cal(int ADC_Raw)
{
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}
