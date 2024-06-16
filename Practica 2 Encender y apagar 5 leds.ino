//Gustavo Noel Betanzos Reyes

//Defino los leds de uso en un arreglo
const int leds[5] = {14, 27, 26, 25, 33};

void setup() {
  //Defino mis pines como salida
  for (int i = 4; i >= 0 ; i--) {
    pinMode(leds[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  //Ciclo for para prender y apagar los 5 leds en orden
  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], 1);
    delay(200);
    digitalWrite(leds[i], 0);
  }
  

  //Ciclo for para prender y apagar los 5 leds de reversa
  for (int i = 4; i >= 0; i--) {
    digitalWrite(leds[i], 1);
    delay(200);
    digitalWrite(leds[i], 0);
  }
  
}
