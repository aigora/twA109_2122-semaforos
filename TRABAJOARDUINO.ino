#include <Servo.h>
Servo servo_10; //SERVO EN EL PIN NÚMERO 10


void setup() // ESTE CÓDIGO SE EJECUTA UNA VEZ AL PRINCIPIO DEL PROGRAMA:
{
 
  //SETUP DE LA COMUNICACIÓN CON EL VISUAL
  Serial.begin(9600); //CONFIGURA LA VELOCIDAD DEL PUERTO SERIE A 9600 BITS/SG
  while(!Serial){;} //MIENTRAS NO TENGA CONEXIÓN SE QUEDA EN BUCLE

  //SETUP DE SENSORES Y ACTUADORES
  pinMode(2, OUTPUT); //ESTE PIN EMITE INFORMACIÓN DESDE LA PLACA, LED ROJO
  pinMode(4, OUTPUT); //LED AMARILLO
  pinMode(7, OUTPUT); //LED VERDE
  pinMode(8, INPUT); //ESTE PIN RECIBE INFORMACIÓN A LA PLACA, PULSADOR
  pinMode(12, INPUT);//PULSADOR 2

  servo_10.attach(10); //SETUP DEL SERVO
}

void loop() // ESTE CÓDIGO SE EJECUTA CONSTANTEMENTE:
{
//PRIMER EJEMPLO DE COMUNICACIÓN CON EL VISUAL:

if (Serial.available()>0) //SI HAY DATOS DISPONIBLES EN EL PUERTO SERIE...
{
 String str = Serial.readStringUntil('\n'); //LEE UN STRING HASTA \n

 if (str.compareTo("GET_PRESION_1")==0) //SI EL STRING RECIBIDO ES "GET_PRESION_1"
  str="MENSAJE RECIBIDO";
 Serial.println(str); //ENVÍA LA RESPUESTA A TRAVÉS DEL PUERTO DE SERIE
 }


 //PRIMER EJEMPLO DE FUNCIONAMIENTO DE LOS SEMÁFOROS Y LA BARRERA:
  
digitalWrite(7, HIGH); //LOS LED TIENEN DOS POSICIONES, HIGH Y LOW. HIGH ES ENCENDIDO, ESTA LÍNEA ENCIENDE EL LED VERDE.
servo_10.write(90); //EL SERVO SE ENCUENTRA A 90 GRADOS, LOS COCHES PUEDEN PASAR

if (digitalRead(8)==1) //SI EL PIN 8 RECIBE SEÑAL DEL PULSADOR:
{
  digitalWrite(7,LOW); //SE APAGA EL LED VERDE
  digitalWrite(4,HIGH); //SE ENCIENDE EL LED AMARILLO
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(4,HIGH);
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(4,HIGH);
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(2,HIGH);//SE ENCIENDE EL LED ROJO
  servo_10.write(0); //LA BARRERA BAJA
  delay(5000);
  digitalWrite(2,LOW);
}

if (digitalRead(12)==1) //SI EL PIN 8 RECIBE SEÑAL DEL PULSADOR:
{
  digitalWrite(7,LOW); //SE APAGA EL LED VERDE
  digitalWrite(4,HIGH); //SE ENCIENDE EL LED AMARILLO
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(4,HIGH);
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(4,HIGH);
  delay(1000); //SE ESPERAN 1000 MILISEGUNDOS
  digitalWrite(4,LOW); //SE APAGA EL LED AMARILLO
  delay(500);
  digitalWrite(2,HIGH);//SE ENCIENDE EL LED ROJO
  servo_10.write(0); //LA BARRERA BAJA
  delay(5000);
  digitalWrite(2,LOW);
}

}