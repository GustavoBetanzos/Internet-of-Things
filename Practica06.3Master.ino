//Gustavo Noel Betanzos Reyes 
//CodigoMaster

//Con el uso de otra ESP32 una en modo esclavo y otra en modo maestro, se logra que cuando se apriete un botón en el ESP32 en modo maestro se envíe la temperatura y encienda un LED al mismo tiempo en el ESP32 en modo esclavo y cuando se apriete el otro se envíe la humedad y se apague el mismo LED.

//Incluir librería de bluetooth
#include "BluetoothSerial.h"

//Incluir librería del sensor
#include "DHT.h"

#define DHTPIN 32     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Definir pines
#define SW1 15 //Pull Down
#define SW2 4 //Pull Up

float h;
// Read temperature as Celsius (the default)
float t;
// Read temperature as Fahrenheit (isFahrenheit = true)

BluetoothSerial BT; // Objeto Bluetooth

String clientName = "ESP32_Vazzvel_Slave"; //Variable con el nombre del cliente (dispositivo a conectarse)
bool connected; //Variable de estado

//Variables para mejorar le funcionamiento del botón
long timeCounter1 = 0;
long timeCounter2 = 0;
int timeThreshold = 250;

void setup() {
  Serial.begin(115200); //Inicializar la comunicación serial
  pinMode(SW1, INPUT); //Pin de entrada
  pinMode(SW2, INPUT); //Pin de entrada
  dht.begin(); //Iniciamos el sensor de temperatura

  BT.begin("ESP32_Vazzvel_Master", true); //Nombre de su dispositivo Bluetooth y en modo maestro
  Serial.println("El dispositivo Bluetooth está en modo maestro.\n Conectando con el anfitrión ...");

  connected = BT.connect(clientName); //Inicializar la conexión vía bluetooth
  if (connected) {
    Serial.println("¡Conectado exitosamente!");
  } else {
    while (!BT.connected(10000)) {
      Serial.println("No se pudo conectar. \n Asegúrese de que el dispositivo remoto esté disponible y dentro del alcance, \n luego reinicie la aplicación.");
    }
  }
}

void loop() {

   //delay para no saturar el sensor
 delay(150);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  if (digitalRead(SW1) == 1) {
    ledOn();
  }
  if (digitalRead(SW2) == 0) {
    ledOff();
  }
}


//Función para prender el Led y enviar la temperatura
void ledOn() {
  if (millis() > timeCounter1 + timeThreshold) {
    BT.write(t); // Envía la temperatura
    timeCounter1 = millis();
    
  }
}

//Función para apagar el Led y enviar la humedad
void ledOff() {
  if (millis() > timeCounter2 + timeThreshold) {
    BT.write(h); // Envía la humedad
    timeCounter2 = millis();
  }

}
