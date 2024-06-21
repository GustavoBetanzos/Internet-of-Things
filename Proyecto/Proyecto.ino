//BETANZOS REYES GUSTAVO NOEL
//PROYECTO FINAL
// Este proyecto final es un videojuego para la clase IoT realizado para cumplir los siguientes requerimientos:
// Utilizar al menos dos temas vistos en clase, uno de las prácticas(1-5) (Sensor LDR, Leds, Botones SW) y otro de las prácticas(6-10) (Servidor Web)
// Utilizar algún sensor extra a los que vienen en la placa de la clase (Buzzer)
//Este juego no es jugable sin la placa ESP32 y la correcta asignacion de sensores 
// Tener un objetivo específico (Vidoejuego portatil de navegador)
// Bibliotecas----------------------------------
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Credenciales Red-----------------------------
const char* ssid = // Nombre de la red WiFi
const char* password =// Contraseña de la red WiFi

// Pines de los LEDs, botones y otros pines-----------------------------
int ledsPines[] = {14, 27, 26, 25, 33};  // Pines para los LEDs
const int numLeds = sizeof(ledsPines) / sizeof(ledsPines[0]);  // Número de LEDs
const int botonAtaque = 4;  // Pin para el botón de ataque
const int botonEscudo = 15;  // Pin para el botón de escudo
const int sensorPin = 34;  // Pin para el sensor (LDR)
const int ZumbadorPin = 32;  // Pin para el zumbador

// Notas musicales para el zumbador--------------------------
// Se definen las frecuencias para cada nota musical
int c = 261, d = 294, e = 329, f = 349, g = 391, gS = 415, a = 440, aS = 455, b = 466,
    cH = 523, cSH = 554, dH = 587, dSH = 622, eH = 659, fH = 698, fSH = 740, gH = 783, gSH = 830, aH = 880;

int estadoLEDs = 0;  // Variable para el estado de los LEDs
bool juegoEnProgreso = false;

AsyncWebServer server(80);  // Servidor web en el puerto 80

// Configuración de la conexión WiFi-----------------------------
void setup_wifi() {
  // Configuración de los pines de los LEDs como salidas y apagados
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledsPines[i], OUTPUT);
    digitalWrite(ledsPines[i], LOW);
  }

  pinMode(ZumbadorPin, OUTPUT);  // Configuración del pin del zumbador como salida

  delay(10);
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP:");
  Serial.println(WiFi.localIP());
  delay(2000);
}

// Lectura del valor del sensor-------------------------------
String ReadSensor() {
  int sensorVal = analogRead(sensorPin);
  Serial.println("Valor fotorresistencia:" + (String)sensorVal);
  return String(sensorVal);
}

// Lectura del botón de ataque-------------------------------
String ReadAtaque() {
  int valorAtaq = digitalRead(botonAtaque);
  Serial.println("ATAQUE EN:" + (String)valorAtaq);
  return String(valorAtaq);
}

// Lectura del botón de escudo-------------------------------
String ReadEsquivar() {
  int valorEsq = digitalRead(botonEscudo);
  Serial.println("ESQUIVAR EN:" + (String)valorEsq);
  return String(valorEsq);
}

// Procesamiento de las solicitudes web-----------------------------
String processor(const String& var) {
  if (var == "SENSOR") {
    return ReadSensor();
  }
  return String();
}

String processor0(const String& var) {
  if (var == "ATAQUE") {
    return ReadAtaque();
  }
  return String();
}

String processor1(const String& var) {
  if (var == "ESQUIVAR") {
    return ReadEsquivar();
  }
  return String();
}

// Configuración inicial-----------------------------
void setup() {
  Serial.begin(115200);  // Inicialización de la comunicación serial
  setup_wifi();  // Configuración de la conexión WiFi

  pinMode(sensorPin, INPUT);  // Configuración del pin del sensor como entrada
  pinMode(botonAtaque, INPUT);  // Configuración del pin del botón de ataque como entrada
  pinMode(botonEscudo, INPUT);  // Configuración del pin del botón de escudo como entrada

  server.begin();  // Inicialización del servidor web

  // Verificación y montaje del sistema de archivos SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar el sistema de archivos SPIFFS.");
    return;
  }

  // Manejo de solicitudes para controlar los LEDs
  server.on("/controlar_leds", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("vida")) {
      int vida = request->getParam("vida")->value().toInt();
      // Actualiza el estado de los LEDs según el valor de vida
      actualizarLEDs(vida);
      request->send(200, "text/plain", "LEDs actualizados");
    } else {
      request->send(400, "text/plain", "Parámetro 'vida' no encontrado");
    }
  });

  // Ruta raíz del archivo HTML-----------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/index.html");
  });

  // Archivo CSS--------------------------------
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  // Archivo JS--------------------------------
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/script.js", "text/js");
  });

  // Nuevas rutas para cada imagen
  server.on("/Dia.png", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/Dia.png", "image/png");
  });

  server.on("/Tarde.png", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/Tarde.png", "image/png");
  });

  server.on("/Noche.png", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/Noche.png", "image/png");
  });

  // Enviar dato sensor-------------------------
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", ReadSensor().c_str());
  });

  // Enviar dato ataque-------------------------
  server.on("/ataque", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", ReadAtaque().c_str());
  });

  // Enviar dato esquivar-------------------------
  server.on("/esquivar", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", ReadEsquivar().c_str());
  });

  // Función para error 404
  server.onNotFound([](AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Error 404");
  });

  // Recibir el dato para saber si reproducir la musica
  server.on("/start_game", HTTP_GET, [](AsyncWebServerRequest* request) {
    // Activa la bandera para indicar que el juego está en progreso
    juegoEnProgreso = true;
    request->send(200, "text/plain", "Juego iniciado");
  });
  
}

  

// Bucle principal que tocara la cancion del juego-----------------------------
void loop(){
  if (juegoEnProgreso) {
    tone(ZumbadorPin, a, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 500);
    delay(500+50);     
    tone(ZumbadorPin, a, 500); 
    delay(500+50);
    tone(ZumbadorPin, f, 350);  
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50); 
    
    tone(ZumbadorPin, a, 500);
    delay(500+50);
    tone(ZumbadorPin, f, 350);
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    tone(ZumbadorPin, a, 1000);   
    delay(1000+50);
   
    tone(ZumbadorPin, eH, 500);
    delay(500+50);
    tone(ZumbadorPin, eH, 500);
    delay(500+50);
    tone(ZumbadorPin, eH, 500); 
    delay(500+50);   
    tone(ZumbadorPin, fH, 350);
    delay(350+50); 
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    
    tone(ZumbadorPin, gS, 500);
    delay(500+50);
    tone(ZumbadorPin, f, 350);
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    tone(ZumbadorPin, a, 1000);
    delay(1000+50);
    
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 350);
    delay(350+50); 
    tone(ZumbadorPin, a, 150);
    delay(150+50);
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, gSH, 250); 
    delay(250+50);
    tone(ZumbadorPin, gH, 250);
    delay(250+50);
    
    tone(ZumbadorPin, fSH, 125);
    delay(125+50);
    tone(ZumbadorPin, fH, 125);
    delay(125+50);    
    tone(ZumbadorPin, fSH, 250);
    delay(250+50);
    delay(250);
    tone(ZumbadorPin, aS, 250);
    delay(250+50);    
    tone(ZumbadorPin, dSH, 500); 
    delay(500+50); 
    tone(ZumbadorPin, dH, 250); 
    delay(250+50); 
    tone(ZumbadorPin, cSH, 250);
    delay(250+50);  
    
    // virtuosismo :)))
    
    tone(ZumbadorPin, cH, 125);
    delay(125+50);  
    tone(ZumbadorPin, b, 125); 
    delay(125+50); 
    tone(ZumbadorPin, cH, 250); 
    delay(250+50);     
    delay(250);
    tone(ZumbadorPin, f, 125); 
    delay(125+50); 
    tone(ZumbadorPin, gS, 500); 
    delay(500+50); 
    tone(ZumbadorPin, f, 375); 
    delay(375+50); 
    tone(ZumbadorPin, a, 125);
    delay(125+50); 
    
    tone(ZumbadorPin, cH, 500); 
    delay(500+50);
    tone(ZumbadorPin, a, 375);  
    delay(375+50);
    tone(ZumbadorPin, cH, 125); 
    delay(125+50);
    tone(ZumbadorPin, eH, 1000); 
    delay(1000+50);
 
    
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 350); 
    delay(350+50);
    tone(ZumbadorPin, a, 150);
    delay(150+50);
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, gSH, 250);
    delay(250+50); 
    tone(ZumbadorPin, gH, 250);
    delay(250+50);
    
    tone(ZumbadorPin, fSH, 125);
    delay(125+50);
    tone(ZumbadorPin, fH, 125);  
    delay(125+50);  
    tone(ZumbadorPin, fSH, 250);
    delay(250+50);
    delay(250);
    tone(ZumbadorPin, aS, 250);  
    delay(250+50);  
    tone(ZumbadorPin, dSH, 500);  
    delay(500+50);
    tone(ZumbadorPin, dH, 250);  
    delay(250+50);
    tone(ZumbadorPin, cSH, 250);
    delay(250+50);  
 
    
    tone(ZumbadorPin, cH, 125);  
    delay(125+50);
    tone(ZumbadorPin, b, 125);  
    delay(125+50);
    tone(ZumbadorPin, cH, 250);
    delay(250+50);      
    delay(250);
    tone(ZumbadorPin, f, 250); 
    delay(250+50); 
    tone(ZumbadorPin, gS, 500); 
    delay(500+50); 
    tone(ZumbadorPin, f, 375);  
    delay(375+50);
    tone(ZumbadorPin, cH, 125); 
    delay(125+50);
           
    tone(ZumbadorPin, a, 500); 
    delay(500+50);           
    tone(ZumbadorPin, f, 375);  
    delay(375+50);          
    tone(ZumbadorPin, c, 125); 
    delay(125+50);  
    tone(ZumbadorPin, a, 1000);
    delay(1000+50);       

    delay(2000);  // dos segundos para volver a empezar.
  }
 }

 void actualizarLEDs(int vida) {
  // Apaga todos los LEDs
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledsPines[i], LOW);
  }

  // Enciende LEDs según el valor de vida
  for (int i = 0; i < vida; i++) {
    digitalWrite(ledsPines[i], HIGH);
  }
}
