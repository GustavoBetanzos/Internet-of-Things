//Gustavo Noel Betanzos Reyes

//Se encienden y apagan los leds con el fin de familiarizarse con la estructura de programación para Arduino.

void setup() {
  // Declaramos el pin 14 como salida
  pinMode(14, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(14, HIGH);   // Prendemos el LED 1
  delay(9);                       // Esperamos 9 milisegundos
  digitalWrite(14, LOW);    // Apagamos el LED 1
  delay(9);                       // Esperamos 9 milisegundos
  //Nuestro periodo entonces es 9, y para tener la frecuencia solo es necesario escribirlo como 1/9
}
