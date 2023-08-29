/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  " " //Sin datos para no actualizar la llave por lo público del GITHUB
#define IO_KEY       " "

/******************************* WIFI **************************************/
#define WIFI_SSID "HUAWEI P smart"
#define WIFI_PASS "922a17ca40a1"
// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
