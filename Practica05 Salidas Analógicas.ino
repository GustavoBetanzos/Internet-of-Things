//Gustavo Noel Betanzos Reyes

//Definimos los pines de los LED, del PWM y DAC
#define PinPWM 26
#define PinDAC 25
#define PinADCPWM 13
#define PinADCDAC 12

//Variables para definir el PWM
const int freq = 1000; //Frecuencia en Hz
const int ChanelPWM = 0; //Canal 0 - 15
const int resolution = 8; //Bits de resolución, hasta 8
const int timedelay = 10000; //delay microseconds
bool state = true; //Variable de estado

void setup() {
  Serial.begin(115200);
  pinMode(PinPWM, OUTPUT);
  pinMode(PinDAC, OUTPUT);
  pinMode(PinADCPWM, INPUT);
  pinMode(PinADCDAC, INPUT);
  ledcSetup(ChanelPWM, freq, resolution);
  ledcAttachPin(PinPWM, ChanelPWM);
}

void loop() {
  int val = state ? 255 : 0;  // Valor para alternar entre encendido y apagado
  for (int i = 0; i < 256; i++) {  // Loop de 0 a 255
    for (int i = 0; i < 360; i++) { 
      float seno = 126 * sin(i * M_PI / 180) + 128;  // Calculamos el valor del seno
      Serial.print("PWM:");  
      ledcWrite(ChanelPWM, seno);  // Escribimos el valor del seno al PWM
      Serial.print(analogRead(PinADCPWM));  
      delay(10);  

      Serial.print(",");
      Serial.print("DAC:");  
      dacWrite(PinDAC, seno);  // Escribimos el valor del seno al DAC
      Serial.println(analogRead(PinADCDAC));  // Leemos y mostramos el valor analógico de PinADCDAC
      delay(10);  
    }
    
    delayMicroseconds(timedelay);  
  }
  state = !state;  // Alternamos el estado (encendido o apagado) del LED
}
