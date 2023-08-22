#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 13

// Variables for tracking temperature changes
float lastTempC = 0.0;

// Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw);

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  uint32_t LM35_Input = analogRead(LM35_GPIO_PIN);
  float Voltage = readADC_Cal(LM35_Input);
  float TempC = ((Voltage / 4095) * 3.3) / 0.01; // Conversion for temperature in °C

  // Continuous monitoring of temperature fluctuations
  if (abs(TempC - lastTempC) >= 0.2) { // Adjust the threshold as needed
    lastTempC = TempC;
    Serial.print("Temperature change detected: ");
    Serial.print("Temperature in °C = ");
    Serial.println(TempC);
  }

  // Check for button press and capture temperature snapshot
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) { // Ensure stable button press
      Serial.print("Button pressed - Temperature snapshot: ");
      Serial.print("Temperature in °C = ");
      Serial.println(TempC);
      delay(1000); // Delay to avoid repeated captures
    }
  }
}
// Definición de la función readADC_Cal
uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
