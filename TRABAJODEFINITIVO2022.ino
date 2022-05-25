#include <Servo.h>

String mensaje_entrada;
String mensaje_salida;
int estado=0;

Servo servo_10; //SERVO EN EL PIN NÚMERO 10

//=======================================================================================================================================//
//FUNCIONES PROTOTIPO
void comprobacion (void);
void comprobacion_sensores(void);
void barrera(void);

//=======================================================================================================================================//
//FUNCIÓN SETUP
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

//=======================================================================================================================================//
//FUNCIÓN LOOP
void loop()  // ESTE CÓDIGO SE EJECUTA CONSTANTEMENTE
{
  if (Serial.available()>0) //SI HAY DATOS DISPONIBLES EN EL PUERTO SERIE...
{
  mensaje_entrada=Serial.readStringUntil('\n');//LEE UN STRING RECIBIDO HASTA '\n'

if (mensaje_entrada.compareTo("COMPROBACION")==0)
{
 comprobacion ();
  }
else if (mensaje_entrada.compareTo("COMPROBACION_SENSORES")==0)
 {
  comprobacion_sensores();
  }
else  if (mensaje_entrada.compareTo("BARRERA")==0)
 {
  while (mensaje_entrada.compareTo("SALIR")!=0)
  {
  barrera();
  }
 }
 }
}


//=======================================================================================================================================//
//FUNCIONES
void comprobacion()
{
  mensaje_salida="CONECTADO";
  Serial.println(mensaje_salida);
  }

void comprobacion_sensores()
{
  int valor1, valor2;

  valor1=digitalRead(8);
  valor2=digitalRead(12);

   if (valor1==HIGH && valor2==HIGH) //ETCÉTERA
  {
    mensaje_salida="3";
    Serial.println(mensaje_salida);
  }
  
  else if (valor1==HIGH && valor2==LOW) //SI EL PRIMERO ESTÁ PULSADO Y EL SEGUNDO NO
  {
    mensaje_salida="1";
    Serial.println(mensaje_salida);
    }
   else if (valor1==LOW && valor2==HIGH) //ETCÉTERA
  {
    mensaje_salida="2";
    Serial.println(mensaje_salida);
    }
  }

  void barrera()
  {
    if (estado==1||estado==0) //VERDE
    {
      digitalWrite(7,HIGH);//ENCIENDE EL LED VERDE
      digitalWrite(2,LOW);//APAGA EL ROJO
      digitalWrite(4,LOW);//APAGA EL AMARILLO
      servo_10.write(90);//BARRERA A 90º, SUBIDA

      int valor=digitalRead(8);
      if (valor==HIGH)
      {
        estado=2;
        }
      }

    else if (estado==2) //INTERMITENTE Y BAJADA DE BARRERA
     {
      digitalWrite(7,LOW);//APAGA EL LED VERDE
      digitalWrite(2,LOW);//APAGA EL ROJO
      digitalWrite(4,HIGH);//ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      digitalWrite(4,HIGH); //ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      digitalWrite(4,HIGH); //ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      servo_10.write(0);//BARRERA A 0º, BAJADA
      estado=4;
      }
      
    else if (estado==3) //INTERMITENTE Y SUBIDA DE BARRERA
      {
        digitalWrite(7,LOW);//APAGA EL LED VERDE
      digitalWrite(2,LOW);//APAGA EL ROJO
      digitalWrite(4,HIGH);//ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      digitalWrite(4,HIGH); //ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      digitalWrite(4,HIGH); //ENCIENDE EL AMARILLO
      delay (1000); //ESPERA 1000 MILISEGUNDOS
      digitalWrite(4,LOW); //SE APAGA EL AMARILLO
      delay (500); //ESPERA 500 MILISEGUNDOS
      servo_10.write(90);//BARRERA A 90º, SUBIDA
      estado=1;
        }
    else if (estado==4) //ROJO
    {
      digitalWrite(7,LOW);//APAGA EL LED VERDE
      digitalWrite(2,HIGH);//ENCIENDE EL ROJO
      digitalWrite(4,LOW);//APAGA EL AMARILLO
      servo_10.write(0);//BARRERA A 0º, BAJADA

       int valor=digitalRead(12);
      if (valor==HIGH)
      {
        estado=3;
        }
      }
    
    }

 
