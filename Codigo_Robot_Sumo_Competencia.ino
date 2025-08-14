//Codigo Final de minisumo
//Talent Land 2018 //Revisado 2025

#include <AFMotor.h> //libreria para controlador de motores
#define Pulsador 16 // Habilitar puerto 16 para modulo de arranque
#define Pulsador2 17 // Habilitar puerto 16 para sensor de arranque


long distancia; // variable sensor ultrasonico
long tiempo;    //variable sensor ultrasonico

int estadoAC = 0;

AF_DCMotor motor3(3);//habilitar salida motor 3 
AF_DCMotor motor4(4);//habilitar salida motor 4 

void setup()
{
  Serial.begin(9600);
  //configurar pines para sensor ultrasonico
   pinMode(10, OUTPUT); //poner pin 10 de arduino como salida
   pinMode(9, INPUT);   //poner pin 9 de arduino como entrada
  //configurar pines para sensores de linea
   pinMode( 14, INPUT); //poner pin A0 de arduino como entrada digital
   pinMode( 15, INPUT);//poner pin A1 de arduino como entrada digital
   pinMode(Pulsador, INPUT);
      pinMode(Pulsador2, OUTPUT);

   
}
     
void loop() {
     estadoAC = digitalRead (Pulsador);

    if (estadoAC == 1){      
     digitalWrite(Pulsador2,HIGH); 
     digitalWrite(10,LOW); // Por cuestión de estabilización del sensor
     delayMicroseconds(2);//esperar 5 microsegundos
     digitalWrite(10, HIGH); // envío del pulso para disparo del ultrasónico
     delayMicroseconds(4);//esperar 10 microsegundos
     tiempo=pulseIn(9, HIGH);//recibir el pulso del sensor y guardarlo en variable tiempo
     //distancia= int(0.017*tiempo);//convertir tiempo del pulso en distancia y convertirlo en cm
     distancia= int((tiempo/2)/29.154);
    
      
     
    if(distancia <= 14)//si el sensor ultrasonico detecta obstaculo de 15 cm o menos sigue hacia adelante 
  {       
       motor3.setSpeed(255); //motor 3 adelante
       motor3.run(FORWARD);  //motor 3 adelante
       motor4.setSpeed(255); //motor 4 adelanteo
       motor4.run(FORWARD);  //motor 4 adelante
     delay (100);//espera 3 segundos
        Serial.print(distancia);
        Serial.println("cm");
  }
       motor3.setSpeed(100); //motor 3 adelante
       motor3.run(FORWARD);  //motor 3 adelante
       motor4.setSpeed(100); //motor 3 adelante
       motor4.run(BACKWARD);  //motor 3 adelante
    // delay (3000);//espera 3 segundos
   
         Serial.print(distancia);
         Serial.println("cm");
   
    if((!digitalRead(14)))//si el sensor inferior derecha detecta 
  {
    //instrucciones para que avance para atras
       motor3.setSpeed(255); //motor 3 atras
       motor3.run(BACKWARD); //motor 3 atras
       motor4.setSpeed(255); //motor 4 atras
       motor4.run(BACKWARD); //motor 4 atras
     delay (400);//espera 1 segundo
      //instrucciones para que gire a la izquierda
      motor3.setSpeed(255); //motor 3 adelante
      motor3.run(FORWARD);  //motor 3 adelante
      motor4.setSpeed(255); //motor 4 atras
      motor4.run(BACKWARD); //motor 4 atras
     delay (600);//espera 1 segundo
          Serial.print(distancia);
             Serial.println("cm");
  }   
  
  if((!digitalRead(15)))//si el sensor inferior izquierda detecta
  {
     //instrucciones para que avance para atras
       motor3.setSpeed(255); //motor 3 atras
       motor3.run(BACKWARD); //motor 3 atras
       motor4.setSpeed(255); //motor 4 atras
       motor4.run(BACKWARD); //motor 4 atras
     delay (400);//espera 1 segundo
      //instrucciones para que gire a la derecha
      motor4.setSpeed(255); //motor 4 adelante
      motor4.run(FORWARD);  //motor 4 adelante
      motor3.setSpeed(123); //motor 3 atras
      motor3.run(BACKWARD); //motor 3 atras
    delay (600);//espera 1 segundo
          Serial.print(distancia);
             Serial.println("cm");
  }
    
  }else{
  if (estadoAC == 0){      
     digitalWrite(Pulsador2,LOW); 
      motor4.setSpeed(0); //STOP
      motor4.run(RELEASE); //STOP
      motor3.setSpeed(0); //STOP
      motor3.run(RELEASE); //STOP
     }
  } 
}
 
