//Parte 1 - Sensor de Temperatura
/*Se implementó un promedio mediante el uso de un array para almacenar múltiples lecturas de ADC calculando así
el promedio, lo que permite reducir el ruido y así las lecturas de temperatura son más uniformes*/
#include <Arduino.h>
#include <esp_adc_cal.h>

#define LM35_GPIO_PIN 35
#define BUTTON_PIN 13
int LM35_Input = 0;
float TempC = 0.0;
//float TempF = 0.0;
float Voltage = 0.0;
bool buttonPressed = false;

// Variables para promediar las lecturas de temperaturas obtenidas
const int numReadings = 10; // Número de lecturas para promediar
int readings[numReadings];  // Array para almacenar las lecturas de temperaturas
int readIndex = 0;          // índice para la leer las lecturas de temperaturas en el array
long total = 0;             // Total acumulado de lecturas

// Calibración ADC para ESP32
esp_adc_cal_characteristics_t adc_chars;

// Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw);

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Inicialización de las lecturas del array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Inicialización de la calibración ADC para ESP32
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    if (!buttonPressed)
    {
      buttonPressed = true;
      
      // Lecturas promedio del ADC
      total = total - readings[readIndex];
      readings[readIndex] = analogRead(LM35_GPIO_PIN);
      total = total + readings[readIndex];
      readIndex = (readIndex + 1) % numReadings;
      LM35_Input = total / numReadings;

      Voltage = readADC_Cal(LM35_Input);
      TempC = ((Voltage/4095)*3.3)/0.01;
      //TempC = Voltage/10;

      Serial.print("Temperatura en °C = ");
      Serial.print(TempC, 1);
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
  //Conversión directa del valor ADC en voltaje utilizando la calibración ADC del ESP32 
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}