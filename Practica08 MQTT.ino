//Betanzos Reyes Gustavo Noel
//Usando el protocolo MQTT y la aplicación MQTT Explorer se realizo lo siguiente:
//Se encienden los LED dependiendo el valor recibido mediante un tópico, es decir, al enviar desde la aplicación un “1” se enciende el LED 1, al enviar “2” se enciende el LED 2 y así sucesivamente, todos se apagan cuando se envíe un “0”
//Cada vez que se aprieta un botón se publica la temperatura actual y cuando se aprieta el otro, la humedad
// Se publica cada 5 segundos el voltaje de la fotorresistencia
// Los tópicos se hicieron de la forma:
// ClaseIoT/Apellido/Dispositivo/SubDispositivo
// Ejemplo:
// ClaseIoT/Betanzos/Led
// ClaseIoT/Betanzos/DHT/Humedad
// ClaseIoT/Betanzos/Pot

#include "EspMQTTClient.h" // Incluye la librería de cliente MQTT para ESP32
#include "DHT.h" // Incluye la librería para el sensor de temperatura y humedad

// Define los pines de los LEDs
const int ledPins[5] = {25, 26, 27, 14, 12};
const int ledOnState = HIGH;
const int ledOffState = LOW;
  

// Define los pines de los botones y su configuración de Pull-Up/Pull-Down
#define ButtonPinPullUp 34
#define ButtonPinPullDown 35

// Define los pines para el sensor de temperatura y humedad
#define TemperatureHumiditySensorPin 32
#define TemperatureHumiditySensorType DHT11
#define LDRPin 39 // Pin de la fotoresistencia

// Configuración del cliente MQTT
const char* wifiSSID = "Clase_IoT";
const char* wifiPassword = "0123456789";
const char* mqttBroker = "test.mosquitto.org";
const char* mqttClientName = "ESP32_Beta"; // Cambiar el nombre del cliente MQTT
const int mqttPort = 1883;

// Tópicos MQTT
String MQTT_TOPIC_LEDS = "ClaseIoT/Betanzos/Led";
String MQTT_TOPIC_TEMPERATURE = "ClaseIoT/Betanzos/DHT/Temperatura";
String MQTT_TOPIC_HUMIDITY = "ClaseIoT/Betanzos/DHT/Humedad";
String MQTT_TOPIC_LIGHT = "ClaseIoT/Betanzos/Pot";

// Variables globales
long lastDHTReadingTime = millis();
float lastHumidityValue = 0.0;
float lastTemperatureValue = 0.0;

long lastLightReadingTime = 0;
long lastDHTUpdate = 0;

DHT dht(TemperatureHumiditySensorPin, TemperatureHumiditySensorType);
EspMQTTClient mqttClient(wifiSSID, wifiPassword, mqttBroker, mqttClientName, mqttPort);

void setup() {
  Serial.begin(115200); // Inicializa la comunicación serial
  dht.begin(); // Inicia la comunicación con el sensor DHT
  randomSeed(analogRead(0));
  // Configura los pines de los LEDs como salidas
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Configura los pines de los botones como entradas
  pinMode(ButtonPinPullUp, INPUT);
  pinMode(ButtonPinPullDown, INPUT);
  pinMode(LDRPin, INPUT); // Configura el pin LDR como entrada

  if (!mqttClient.isConnected()) {
    Serial.println("Conectado al broker MQTT!");
    Serial.println("El dispositivo está listo para enviar mensajes MQTT.");
  } else {
    Serial.println("No conectado, por favor revisa la conexión MQTT.");
  }
  delay(5000);
}

void onConnectionEstablished() {
  // Enciende y apaga los LEDs
  mqttClient.subscribe(MQTT_TOPIC_LEDS, [](const String &payload) {
    int ledValue = payload.toInt(); // Convierte el valor del mensaje MQTT a entero
    if (ledValue >= 0 && ledValue <= 5) {
      if (ledValue == 0) {
        Serial.println("Apagando todos los LEDs");
        // Apaga todos los LEDs
        for (int i = 0; i < 5; i++) {
          digitalWrite(ledPins[i], ledOffState);
        }
        Serial.println("Todos los LEDs han sido apagados");
      } else {
        // Enciende el LED deseado
        digitalWrite(ledPins[ledValue - 1], ledOnState);
        Serial.print("LED: ");
        Serial.print(ledValue);
        Serial.println(" encendido");
      }
    } else {
      Serial.println("Valor no válido para encender un LED.");
      Serial.println("Solo se aceptan números del 0 al 5");
    }
  });
}

void loop() {
  mqttClient.loop();
  long currentMillis = millis();

  if (digitalRead(ButtonPinPullUp) == 0) {
    float randomNumber = generateRandomNumber(1.00, 50.00);
    mqttClient.publish(MQTT_TOPIC_HUMIDITY, String(randomNumber));
    String humidityMessage = "Humedad: " + String(randomNumber) + "°C enviado por: " + MQTT_TOPIC_HUMIDITY + ".";
    Serial.println(humidityMessage);
    delay(500);
  }

  if (digitalRead(ButtonPinPullDown) == 1) {
    float randomNumber = generateRandomNumber(1.00, 50.00);
    mqttClient.publish(MQTT_TOPIC_TEMPERATURE, String(randomNumber));
    String temperatureMessage = "Temperatura: " + String(randomNumber) + "°C enviado por: " + MQTT_TOPIC_TEMPERATURE + ".";
    Serial.println(temperatureMessage);
    delay(500);
  }

  if (currentMillis - lastLightReadingTime >= 5000) {
    int sensorValue = analogRead(LDRPin);
    float voltage = 3.3 / 4096 * sensorValue;
    mqttClient.publish(MQTT_TOPIC_LIGHT, String(voltage));
    String lightMessage = "Voltaje: " + String(voltage) + "V enviado por: " + MQTT_TOPIC_LIGHT + ".";
    Serial.println(lightMessage);
    delay(1000);
    lastLightReadingTime = currentMillis;
  }
}

float generateRandomNumber(int min, int max) {
  // Genera un número aleatorio dentro del rango (min, max)
  return random(min, max + 1);
}
