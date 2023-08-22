//Parte 1 - Sensor de Temperatura
/*Se implementó un promedio mediante el uso de un array para almacenar múltiples lecturas de ADC calculando así
el promedio, lo que permite reducir el ruido y así las lecturas de temperatura son más uniformes*/
#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 13
int LM35_Input = 0;
float TempC = 0.0;
float TempF = 0.0;
float Voltage = 0.0;
bool buttonPressed = false;

// Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw);

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    if (!buttonPressed)
    {
      buttonPressed = true;
      LM35_Input = analogRead(LM35_GPIO_PIN);
      Voltage = readADC_Cal(LM35_Input);
      TempC = Voltage / 10;
      TempF = (TempC * 1.8) + 32;
      Serial.print("Temperature in °C = ");
      Serial.print(TempC);
      Serial.print(",  Temperature in °F = ");
      Serial.println(TempF);
    }
  }
  else
  {
    buttonPressed = false;
  }
}

// Definición de la función readADC_Cal
uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}