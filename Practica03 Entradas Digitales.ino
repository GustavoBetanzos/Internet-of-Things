//Gustavo Noel Betanzos Reyes

//Definimos los pines para los botones
#define PinBotton_PullUp 4 
#define PinBotton_PullDown 15

//Defino los leds de uso en un arreglo
const int leds[5] = {14, 27, 26, 25, 33};
int contador=0; //definimos el contador para saber cuantas veces hemos presionado el boton izquierdo
long timeCounter = 0;
const int timeThreshold = 170;


void setup() {
  //Defino mis pines como salida
  for (int i = 4; i >= 0 ; i--) {
    pinMode(leds[i], OUTPUT);
  }
  
  pinMode(PinBotton_PullUp, INPUT);
  pinMode(PinBotton_PullDown, INPUT);
  digitalWrite(14, 0); //Se pone el pin 14 en 0 porque por default inicia prendido
}

void loop() {
    int x = digitalRead(PinBotton_PullUp); //izquierda que manda siempre la señal en 1
    int y = digitalRead(PinBotton_PullDown); //derecha que manda siempre la señal en 0

    if(x==0){//caso cuando presionamos el boton izquierdo
        if (millis() > timeCounter + timeThreshold) {
          Serial.println(contador);
          digitalWrite(leds[contador], 1);
          timeCounter = millis();
          contador++;
        }

        if(contador==6){//si presionamos el boton izquierdo 6 veces se apagan los leds
            for(int i=0; i<5; i++){
              digitalWrite(leds[i], 0);
              contador=0;
            }
          }
    }

    if(y==1){//caso cuando presionamos el boton derecho se apaguen todos leds
      for(int i=0; i<5; i++){
        digitalWrite(leds[i], 0);
        contador=0;
      }
     }
    
}