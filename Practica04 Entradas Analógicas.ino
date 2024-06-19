//Gustavo Noel Betanzos Reyes

//Se encienden los 5 leds de la placa en función de la luz inducida a la fotorresistencia, es decir, sin nada de luz que ninguno se enciende y conforme la luz llega al sensor se van iluminando los demás leds hasta que los 5 se iluminen por completo.

#define PinADC 34 //Defino pin
//Defino un arreglo para los leds
const int leds[5] = {14, 27, 26, 25, 33};

void setup() {
    //Defino mis pines como salida
  for (int i = 4; i >= 0 ; i--) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(PinADC, INPUT);   //Declaro al pin como entrada
}

void loop() {
  int sensor = analogRead(PinADC);  //Guardo en una variable la lectura del pin
  float lectura = (6.0/4096)*sensor;     //Aplico una regla de 3, para definir todos los estados de los leds (apagadp, 1,2,3,4 y 5 prendidos)

  if(lectura<1){ //cuando la lectura sea muy baja apaga todos los leds
    for(int j=0; j<5; j++){
      digitalWrite(leds[j], 0);
    }
   }
    
  for(int i=1; i<=lectura; i++){//ciclo for para prender los leds que cumplan con la condicion de lectura
    digitalWrite(leds[i-1], 1);
    for(int j=5; j>=i; j--){//ciclo for que mantiene apagados los leds que no cumplian con la condicon de lectura
      digitalWrite(leds[j], 0);
    }
  } 
}
