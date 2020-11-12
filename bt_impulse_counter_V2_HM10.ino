// put your main code here, to run repeatedly:

#include "LowPower.h"

#define sensor A0 //defining sensor pin
#define interruptPin 2
#define statePin 3
#define alarmPin 5

float resistor1 = 10.0; //you can change it if you want, just make sure you also do it on the circuit
float resistor2 = 2.0;

float Vmax = ((resistor1+resistor2))/resistor2;

volatile int licznik = 0;

bool isconnecting = false;

void setup() {
  pinMode(sensor, INPUT); //declaring sensor as an output
  pinMode(interruptPin, INPUT);
  pinMode(statePin, INPUT_PULLUP);
  pinMode(alarmPin, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), counter, RISING);

  analogReference(INTERNAL);
  
  licznik = 0;
  isconnecting=true;
  Serial.begin(9600);
  delay(1000);
  Serial.println("AT+ROLE1"); // st up as Master
  delay(2000);
  Serial.println("AT+INQ"); // look for nearby Slave
  delay(2000);
  Serial.println("AT+CONN1");
  delay(3000);
  isconnecting=false;
  
  attachInterrupt(digitalPinToInterrupt(statePin), checkstate, FALLING);
}

void loop() {
  bool state = digitalRead(statePin);
  bool alarm = digitalRead(alarmPin);

  if (isconnecting) {
    exit; 
  }

  if (!state) {
    //Serial.println("INFO: loop reconnect");
    isconnecting=true;
    Serial.println("AT+INQ"); // look for nearby Slave
    delay(2000);
    Serial.println("AT+CONN1");
    delay(3000);
    isconnecting=false;
  }
  else {    
    int a = analogRead(sensor);
    float V = a* 1.1 * Vmax / 1024;
  
    int sum = 2*licznik+a+V+2020;
  
  //  Serial.print("L;");
    Serial.print(licznik);
    Serial.print(";");
    Serial.print(V);  
    Serial.print(";");  
    Serial.print(a);
    Serial.print(";");  
    Serial.print(sum); //printing voltage
    Serial.print(";");
    Serial.print(alarm);
    Serial.println(";");
  
    delay(50);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    
//test  
//  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
//  licznik = licznik +1;
  }
}

void counter() {
  licznik = licznik +1;
}

void checkstate() {
  //
}
