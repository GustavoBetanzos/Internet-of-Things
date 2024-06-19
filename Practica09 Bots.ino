//Gustavo Noel Betanzos Reyes 
//Se creo un bot de Telegram en donde el usuario pueda realizar lo siguiente:
//Interactuar con los 5 LEDs de la placa, apagándolos y prendiéndolos
//Monitorear el voltaje del potenciómetro
//Usando la fotorresistencia, se implementó una alarma en donde el umbral de activación es dado por el usuario, junto la capacidad de activarla o desactivarla en cualquier momento.

#include "ArduinoJson.h"
#include "CTBot.h" // Incluye la biblioteca CTBot para interactuar con la API de Telegram.

#define pinPhoto 34 // Define el número de pin al que está conectada la fotorresistencia.
const int portPin = 35; // Define el número de pin al que está conectada el potenciometro.
int potValor=0;
float voltaje=0.0;

const int leds[5] = {25, 26, 27, 14, 12}; //Definimos los pins de los leds
CTBot myBot; // Crea un objeto para interactuar con el bot de Telegram.

// Detalles de la red Wi-Fi y el token de autenticación del bot de Telegram.
String ssid = //Nombre de la RED
String pass = //Contraseña de la RED
String token = "6740514801:AAG6JB8uNel5tPzI45GKMoAUQ_JSgIpUMnw"; // Reemplazar con el token de tu bot de Telegram. 

boolean state = false; // Variable para rastrear si la alarma está activada o desactivada.
int umbral; // Define el umbral sobre el cual se activará la alarma.
int id = 0; // Variable para almacenar el ID del chat de Telegram desde el que se activó la alarma.
int estadoConversacion = 0;

void setup() {
  Serial.begin(115200); // Inicia la comunicación serial.
  Serial.println("Comenzando TelegramBot..."); 
  pinMode(pinPhoto, INPUT); // Configura el pin del sensor como entrada.

  //Definimos los leds como salida
  for (int i = 4; i >= 0 ; i--) {
    pinMode(leds[i], OUTPUT);
  }
  
  myBot.wifiConnect(ssid, pass); // Conecta a la red Wi-Fi especificada.
  myBot.setTelegramToken(token); // Establece el token de autenticación para el bot de Telegram.
  delay(2000); // Espera un momento para que la conexión se establezca.

  // Verifica si la conexión con el bot de Telegram se ha establecido correctamente.
  if (myBot.testConnection())
    Serial.println("Conectado con Bot");
  else
    Serial.println("Error en la conexión");
}

void loop(){
  TBMessage msg; // Crea un objeto para almacenar los mensajes entrantes de Telegram.
  int sensor = analogRead(pinPhoto); // Lee el valor actual del sensor.
  Serial.println(sensor); // Imprime el valor del sensor en el monitor serial.

  potValor=analogRead(portPin);
  delay(1000);
  
  // Verifica si hay un nuevo mensaje para el bot de Telegram.
  if (myBot.getNewMessage(msg)) {   
    if (estadoConversacion == 0) {
    if (msg.text.equalsIgnoreCase("/start")){
      // Si el comando es "/start", responde con un mensaje de bienvenida e instrucciones.
      Serial.print("Nueva interacción de: ");
      Serial.println(msg.sender.username);
      myBot.sendMessage(msg.sender.id, "Hola, por favor envía /Dato para monitorear la variable, /DefinirUmbral para modificar el umbral de la fotoresistencia /Alarma si deseas activar una alarma, /Alto si deseas desactivar esta última, /VoltajePot si deseas saber el voltaje del potenciometro, /PrenderLed1, /ApagarLed1, /PrenderLed2, /ApagarLed2, /PrenderLed3, /ApagarLed3, /PrenderLed4, /ApagarLed4, /PrenderLed5, /ApagarLed5, para interactuar con los leds");
    }
    else if (msg.text.equalsIgnoreCase("/Dato")){
      // Si el comando es "/Dato", envía el valor actual del sensor al usuario.
      Serial.print("Monitoreo: ");
      Serial.println(sensor);
      String mensaje = (String)"El valor actual es: " + (String)sensor;
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Dato enviado");
    }
    else if (msg.text.equalsIgnoreCase("/DefinirUmbral")) {
      // Si el comando es "/DefinirUmbral", permite al usuario modificar el umbral 
        String mensaje = "Dame el valor del umbral (entero):";
        myBot.sendMessage(msg.sender.id, mensaje);
        estadoConversacion = 1; //entero para ir internamente a la condicion de definir el umbral
      }
    else if (msg.text.equalsIgnoreCase("/Alarma")){
      // Si el comando es "/Alarma", activa la alarma y guarda el ID del chat.
      state = true;
      id = msg.sender.id;
      String mensaje = "Alarma activada";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println(mensaje);
    }
    else if (msg.text.equalsIgnoreCase("/Alto")){
      // Si el comando es "/Alto", desactiva la alarma.
      state = false;
      id = msg.sender.id;
      String mensaje = "Alarma desactivada";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println(mensaje);
      id = 0; // Restablece el ID del chat ya que la alarma está desactivada.
    }
    else if (msg.text.equalsIgnoreCase("/VoltajePot")){
      // Si el comando es "/VoltajePot", envía calcula el voltaje en base al potenciometro y lo envia al usuario.
      voltaje = (potValor * 3.3)/4096;
      Serial.print("Voltaje potenciometro: ");
      Serial.print(voltaje);
      String mensaje = (String)voltaje;
      myBot.sendMessage(msg.sender.id, mensaje);
    }
    else if (msg.text.equalsIgnoreCase("/PrenderLed1")){
      // Si el comando es "/PrenderLed1", prende el Led1 
      digitalWrite(leds[0],1);
      String mensaje = (String)"Led1 prendido";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led1 prendido");
    }

    else if (msg.text.equalsIgnoreCase("/ApagarLed1")){
      // Si el comando es "/ApagarLed1", apaga el Led1 
      digitalWrite(leds[0],0);
      String mensaje = (String)"Led1 apagado";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led1 apagado");
    }
    
    else if (msg.text.equalsIgnoreCase("/PrenderLed2")){
      // Si el comando es "/PrenderLed2", prende el Led2 
      digitalWrite(leds[1],1);
      String mensaje = (String)"Led2 prendido";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led2 prendido");
    }    else if (msg.text.equalsIgnoreCase("/ApagarLed2")){
      // Si el comando es "/ApagarLed2", apaga el Led2 
      digitalWrite(leds[1],0);
      String mensaje = (String)"Led2 apagado";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led2 apagado");
    }
    
    else if (msg.text.equalsIgnoreCase("/PrenderLed3")){
      // Si el comando es "/PrenderLed3", prende el Led3 
      digitalWrite(leds[2],1);
      String mensaje = (String)"Led3 prendido";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led3 prendido");
    }    else if (msg.text.equalsIgnoreCase("/ApagarLed3")){
      // Si el comando es "/ApagarLed3", apaga el Led3 
      digitalWrite(leds[2],0);
      String mensaje = (String)"Led3 apagado";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led3 apagado");
    }
    
    else if (msg.text.equalsIgnoreCase("/PrenderLed4")){
      // Si el comando es "/PrenderLed4", prende el Led4 
      digitalWrite(leds[3],1);
      String mensaje = (String)"Led4 prendido";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led4 prendido");
    }    else if (msg.text.equalsIgnoreCase("/ApagarLed4")){
      // Si el comando es "/ApagarLed4", apaga el Led4 
      digitalWrite(leds[3],0);
      String mensaje = (String)"Led4 apagado";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led4 apagado");
    }
    
    else if (msg.text.equalsIgnoreCase("/PrenderLed5")){
      // Si el comando es "/PrenderLed5", prende el Led5 
      digitalWrite(leds[4],1);
      String mensaje = (String)"Led5 prendido";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led5 prendido");
    }    else if (msg.text.equalsIgnoreCase("/ApagarLed5")){
      // Si el comando es "/ApagarLed5", apaga el Led5 
      digitalWrite(leds[4],0);
      String mensaje = (String)"Led5 apagado";
      myBot.sendMessage(msg.sender.id, mensaje);
      Serial.println("Led5 apagado");
    } else{
      // Si el mensaje recibido no coincide con ninguno de los comandos anteriores, envía un mensaje de error.
      String mensaje1 = "Mensaje no válido, intenta de nuevo con: /start";
      myBot.sendMessage(msg.sender.id, mensaje1);
      Serial.println(mensaje1);
    }

    if ((state == true) && (sensor > umbral)){ 
      // Si la alarma está activada y el valor del sensor supera el umbral, envía una alerta al usuario.
        Serial.println(sensor);
        String mensaje = "¡¡ALERTA!! El nivel aumentó a: "+(String)sensor;
        Serial.println("¡¡ALERTA!! El nivel aumentó a: "+ sensor);
        myBot.sendMessage(msg.sender.id, mensaje);
    }
    

    }
    else if (estadoConversacion == 1) {//if auxiliar para leer el umbral del usuario
      // Convierte el mensaje en un entero
      umbral = atoi(msg.text.c_str());

      // Ahora, 'umbral' contiene el valor como un entero.
      Serial.print("Umbral definido como: ");
      Serial.println(umbral);

      String mensaje1 = "Umbral modificado a: " + String(umbral);
      myBot.sendMessage(msg.sender.id, mensaje1);

      // Restablece el estado de la conversación
      estadoConversacion = 0;
    }
  }
  
  
  
  delay(50);
}
