//Gustavo Noel Betanzos Reyes 

//Usando la aplicación Serial Bluetooth Terminal se encienden los LEDs dependiendo el valor enviado, es decir, al enviar un “1” se enciende el LED 1, al enviar “2” se enciende el LED 2 y así sucesivamente, todos se apagan cuando se envíe un “0”

//Incluir librería de bluetooth
#include "BluetoothSerial.h"

//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Defino los leds de uso en un arreglo
const int leds[5] = {14, 27, 26, 25, 33};

BluetoothSerial BT; //Objeto Bluetooth

void setup() {
  Serial.begin(115200); //Inicializar la comunicación serial
  BT.begin("SENSOR"); //Nombre de su dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejarse");
  //Defino mis pines como salida
  for (int i = 4; i >= 0 ; i--) {
    pinMode(leds[i], OUTPUT);
  }
  
  digitalWrite(14, 0); //Se pone el pin 14 en 0 porque por default inicia prendido

}

void loop() {
  if (BT.available()) // Mientras haya datos por recibir vía bluetooth
  {
    int val = BT.read(); // Lee un byte de los datos recibidos
    if (val == 49) { // 1 en ASCII
      digitalWrite(leds[0], HIGH);//prende el Led 1
    }
    if (val == 50) { // 2 en ASCII
      digitalWrite(leds[1], HIGH);//prende el Led 2
    }
    if (val == 51) { // 3 en ASCII
      digitalWrite(leds[2], HIGH);//prende el Led 3
    }
    if (val == 52) { // 4 en ASCII
      digitalWrite(leds[3], HIGH);//prende el Led 4
    }
    if (val == 53) { // 5 en ASCII
      digitalWrite(leds[4], HIGH); //prende el Led 5
    }
    if (val == 48) { // 0 en ASCII
      for(int i=0; i<5; i++){
      digitalWrite(leds[i], LOW); //apaga todos los leds
    }
    }
    
  }
}
