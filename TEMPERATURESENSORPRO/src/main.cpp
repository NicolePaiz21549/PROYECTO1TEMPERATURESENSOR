#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 33
int LM35_Input = 0;
float TempC = 0.0;
float Voltage = 0.0;
bool buttonPressed = false;

// Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LM35_GPIO_PIN, INPUT); // Inicialización del pin LM35 como input
}

void loop() {
  unsigned long currentTime = millis();
  int buttonState = digitalRead(BUTTON_PIN);

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!buttonPressed) 
    {
      buttonPressed = true;

      // Lectura del valor LM35_ADC
      TempC=((analogRead(LM35_GPIO_PIN+70))*(5000.0/4096.0)); //Sumatoria de un valor de OFFSET para el valor análogo original multiplicado por el voltaje de 5V por pin Vin para la fórmula dividido la resolución ADC de 12bits
      //+ grados celsius de OFFSET para obtener un valor más precisos
      TempC=TempC/10.0; //División dentro de 10 representando los 10mV del LM35 ya que cada cambio de 10mV representa un cambio de 1℃

      Serial.print("Temperatura en °C = ");
      Serial.print(TempC);
      Serial.print(",  Lectura analógica pura = ");
      Serial.println(analogRead(LM35_GPIO_PIN));
      Serial.println();
    }
  } else {
    buttonPressed = false;
  }
}