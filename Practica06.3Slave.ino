//Gustavo Noel Betanzos Reyes 
//CodigoSlave

//Con el uso de otra ESP32 una en modo esclavo y otra en modo maestro, se logra que cuando se apriete un botón en el ESP32 en modo maestro se envíe la temperatura y encienda un LED al mismo tiempo en el ESP32 en modo esclavo y cuando se apriete el otro se envíe la humedad y se apague el mismo LED.

//Incluir librería de bluetooth
#include "BluetoothSerial.h"

//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Definir un pin para el LED
#define LED 2


BluetoothSerial BT; //Objeto bluetooth

//Función de llamada haciendo referencia al objeto BT
void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  switch (event) {
    case ESP_SPP_START_EVT:
      Serial.println("Inicializado SPP");
      break;
    case ESP_SPP_SRV_OPEN_EVT:
      Serial.println("Cliente conectado");
      break;
    case ESP_SPP_CLOSE_EVT:
      Serial.println("Cliente desconectado");
      break;
    case ESP_SPP_DATA_IND_EVT:
      while (BT.available()) { // Mientras haya datos por recibir
        
        int incoming = BT.read();// Lee un byte de los datos recibidos
        float cast = float(incoming);          // Convierte los datos a tipo flotante
        Serial.println(cast); 

            if(incoming<40){ // Caso para saber la temperatura 
              digitalWrite(LED, HIGH);
            }else if(incoming>40){// Caso para saber la humedad
              digitalWrite(LED, LOW);
             }
      }
      break;
  }
}
void setup() {
  Serial.begin(115200);
  BT.begin("ESP32_Vazzvel_Slave"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function); //Devolución de la función callback
  pinMode (LED, OUTPUT); // Pin LED como salida
}

void loop() {


}
