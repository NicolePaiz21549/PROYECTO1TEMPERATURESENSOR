//******************************************************************************************************************
//Dulce Nicole Monney Paiz, 21549
//Proyecto 1 – Sensor de temperatura
//BE3029 - Electrónica Digital 2
//Librerías 
//******************************************************************************************************************
#include <Arduino.h>
#include <esp_adc_cal.h>
#include "display7.h"
#include <driver/ledc.h>

//Definiciones
//******************************************************************************************************************
//Definiciones del sensor LM35
#define LM35_GPIO_PIN 35
#define BUTTON_PIN 33 //Botón para grabar la lectura actual de temperatura
int LM35_Input = 0;
float TempC = 0.0;
float Voltage = 0.0;
bool buttonPressed = false;

//Definiciones para los displays de 7 segmentos
#define pA 17
#define pB 5
#define pC 21
#define pD 22
#define pE 23
#define pF 16
#define pG 4
#define pdP 19

#define display1 27
#define display2 14
#define display3 12

//Definiciones para el semáforo de temperatura 
//Variables para los LEDs
#define RED 15 //Case 2
#define GREEN 18 //Case 0
#define YELLOW 13 //Case 1

//Canales para los LEDs con su color correspondiente
#define redChannel 0 //Case 2
#define greenChannel 1 //Case 0
#define yellowChannel 2 //Case 1

//Variables para el control del servo motor
#define servoChannel 7 //Canal para el servo motor
#define servoPin 2 //Pin del servo motor
//#define angleStep 5 //Tamaño del paso para el cambio del ángulo

//Variables generales
int angle = 0; //Ángulo/posición inicial del servo (0: Verde, 1: Amarillo, 2: Rojo)
int selectedColor = 0; //Variable para guardar el color seleccionado

//Prototipos de funciones
//******************************************************************************************************************
//Prototipo de función para readADC_Cal
uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Botón con resistencia de pull-up interna

  pinMode(LM35_GPIO_PIN, INPUT); //Inicialización del pin LM35

  configurarDisplay7(pA, pB, pC, pD, pE, pF, pG, pdP);
  pinMode(display1, OUTPUT);
  pinMode(display2, OUTPUT);
  pinMode(display3, OUTPUT);

  digitalWrite(display1, HIGH);
  digitalWrite(display2, HIGH);
  digitalWrite(display3, HIGH);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  ledcSetup(redChannel, 5000, 8); //Canal 0, frecuencia de 5kHz, resolución de 8-bits (0-255)
  ledcSetup(greenChannel, 5000, 8); //Canal 1, frecuencia de 5kHz, resolución de 8-bits (0-255)
  ledcSetup(yellowChannel, 5000, 8); //Canal 2, frecuencia de 5kHz, resolución de 8-bits (0-255)
  ledcSetup(servoChannel, 50, 16); //Canal 7, frecuencia de 50kHz, resolución de 16-bits (0-65535)

  ledcAttachPin(RED, redChannel);
  ledcAttachPin(GREEN, greenChannel);
  ledcAttachPin(YELLOW, yellowChannel);
  ledcAttachPin(servoPin, servoChannel); 
  
//Inicialización de los LEDs
  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 0);
  ledcWrite(yellowChannel, 0);

//Inicialización de la posición inicial 
  ledcWrite(servoChannel, map(angle, 0, 180, 0, 65535));
  delay(1000); //Tiempo de espera de 1 segundo para la inicialización
}

void loop() {
  unsigned long currentTime = millis();
  int buttonState = digitalRead(BUTTON_PIN);

      // Display de temperatura en displays de 7-segmentos (Todo el tiempo están encendidos los displays)
      int tempInt = int(TempC);
      int tempDecimal = int((TempC - tempInt) * 10); //Cálculo del decimal del valor de temperatura, lo multiplica por 10 para convertirlo en una fracción de 10 grados y almacena el resultado como un número entero en la variable tempDecimal.
      int tempDecimalTens = tempInt / 10;
      int tempDecimalUnits = tempInt % 10;

      digitalWrite(display1, HIGH);
      digitalWrite(display2, LOW);
      digitalWrite(display3, LOW);
      desplegarValor(tempDecimalTens);
      delay(5);

      digitalWrite(display1, LOW);
      digitalWrite(display2, HIGH);
      digitalWrite(display3, LOW);
      desplegarValor(tempDecimalUnits);
      desplegarPunto(true);
      delay(5);

      digitalWrite(display1, LOW);
      digitalWrite(display2, LOW);
      digitalWrite(display3, HIGH);
      desplegarValor(tempDecimal);
      desplegarPunto(false);
      delay(5);


  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;

      // Lectura del valor LM35_ADC
      TempC = ((analogRead(LM35_GPIO_PIN) + 70) * (5000.0/ 4096.0)); // Sumatoria de un valor OFFSET (factor de correción) para el valor análogo original multiplicado por el voltaje de 5V por pin Vin para la fórmula dividido la resolución ADC de 12bits
      TempC = TempC / 10.0; // División dentro de 10 representando los 10mV del LM35 ya que cada cambio de 10mV representa un cambio de 1℃

      Serial.print(TempC); //Impresión de backup
      Serial.println();

      // Condicionales para el comportamiento de los LEDs y el servo
      if (TempC < 37.0) {
        angle = 45;
        ledcWrite(redChannel, 0);
        ledcWrite(yellowChannel, 0);
        ledcWrite(greenChannel, 1);
      } else if (TempC >= 37.0 && TempC <= 37.5) {
        angle = 90;
        ledcWrite(redChannel, 0);
        ledcWrite(greenChannel, 0);
        ledcWrite(yellowChannel, 1);
      } else {
        angle = 130;
        ledcWrite(greenChannel, 0);
        ledcWrite(yellowChannel, 0);
        ledcWrite(redChannel, 1);      
}

      ledcWrite(servoChannel, 1638+map(angle, 0, 180, 0, 6226)); //Mapeado con OFFSET en la diferencia de operación del rango del servo del 2% al 12% del Duty Cycle
    }
  } else {
    buttonPressed = false;
  }
}


