#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 13
int LM35_Input = 0;
float TempC = 0.0;
float Voltage = 0.0;
//unsigned long time_now = 0;
bool buttonPressed = false;
//unsigned long lastButtonPressTime = 0;
//unsigned long debounceDelay = 50;
//int lastButtonState = HIGH;

// Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LM35_GPIO_PIN, INPUT); // Initialize LM35 pin as input
}

void loop() {
  unsigned long currentTime = millis();
  int buttonState = digitalRead(BUTTON_PIN);

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!buttonPressed) 
    {
      buttonPressed = true;

      // Read LM35_ADC value
      TempC=((analogRead(LM35_GPIO_PIN)*4.88)/10);
      
      //LM35_Input = analogRead(LM35_GPIO_PIN);
      //Voltage = readADC_Cal(LM35_Input);
      //TempC = ((Voltage / 4095) * 3.3) / 0.01;

      Serial.print("Temperatura en °C = ");
      Serial.print(TempC, 1);
      Serial.println();
    }
  } else {
    buttonPressed = false;
  }
}
