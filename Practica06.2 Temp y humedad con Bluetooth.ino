//Gustavo Noel Betanzos Reyes

//Usando la aplicación Serial Bluetooth Terminal se logro que cada vez que se apriete un botón, se recibe la temperatura actual en el smartphone y cuando se apriete el otro, la humedad.

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


//Definimos los pines para los botones
#define PinBotton_PullUp 4 
#define PinBotton_PullDown 15

BluetoothSerial BT; //Objeto Bluetooth

//Variable con el nombre del cliente (dispositivo a conectarse - esclavo)
String clientName = "SensorTemperatura"; 
bool connected;

void setup() {
  Serial.begin(115200); //Inicializar la comunicación serial
  
  dht.begin(); //Iniciamos el sensor de temperatura

  BT.begin(clientName); //Nombre de su dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejarse");
  
  pinMode(PinBotton_PullUp, INPUT);
  pinMode(PinBotton_PullDown, INPUT);
}

void loop() {
  //delay para no saturar el sensor
  delay(150);

   int x = digitalRead(PinBotton_PullUp); //izquierda que manda siempre la señal en 1
   int y = digitalRead(PinBotton_PullDown); //derecha que manda siempre la señal en 0


   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  //definimos las variables para no estar leyendo constantemente
  float humedad = h;
  float tempC = t;
  float tempF = f;
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
      //si presionamos un boton nos manda la humedad
      if(x==0){
        BT.println("");
        BT.print(F("Humidity: "));
        BT.print(humedad);
       }

      //si presionamos un boton nos manda la temperatura
      if(y==1){
      BT.println("");
      BT.print(F("Temperature: "));
      BT.print(tempC);
      BT.print(F("°C "));
      BT.print(tempF);
      }
}
