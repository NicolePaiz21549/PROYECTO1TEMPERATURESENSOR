#ifndef __DISPLAY7_H__
#define __DISPLAY7_H__

#include <Arduino.h>

extern uint8_t pinA, pinB, pinC, pinD, pinE, pinF, pinG, pindP; //Variables globales

//Función para configurar el display de 7 segmentos
void configurarDisplay7(uint8_t pA, uint8_t pB, uint8_t pC, uint8_t pD, uint8_t pE, uint8_t pF, uint8_t pG, uint8_t pdP);

//Función para desplegar el valor al display de 7 segmentos
void desplegarValor(uint8_t valor);

//Función para desplegar el punto decimal en el display de 7 segmentos
void desplegarPunto(boolean punto);

#endif // __DISPLAY7_H__