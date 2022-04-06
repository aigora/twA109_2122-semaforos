#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "SerialClass/SerialClass.h"

#define MAX_BUFFER 200
#define PAUSA_MS 200

//Funciones prototipo
int menu_ppal(void);
void verifica_sensores(Serial*, char*);
float leer_pulsador_1(Serial*);
float leer_pulsador_2(Serial*);
int enviar_y_recibir(Serial*, const char*, char*);

int main(void)
{
    Serial* Arduino;  //Representa internamente la placa de Arduino
    char puerto[] = "COM3"; //Puerto serie al que está conectado el Arduino, segundo puerto USB empezando por abajo a la izquierda en este caso

    setlocale(LC_ALL, "es-ES");
    Arduino = new Serial((char*)puerto);
    do
    {
        switch (menu_ppal())
        {
        case 1:
            printf("FUNCIONA\n"); //Si se escoge esta opción, la barrera sube y baja para dos trenes pero no se mide la diferencia de tiempo entre trenes.
            break;
        case 2:
            printf("FUNCIONA\n"); //Si se escoge esta opción, la barrera sube y baja para dos trenes y se mide la diferencia de tiempo entre el valor elegido y el real.
            break;
        case 3:
            verifica_sensores(Arduino, puerto);; //Si se escoge esta opción, se verifica el funcionamiento de los sensores.
            break;
        case 4:
            break; //Si se escoge esta opción, el programa se acaba.
        }
    } while (menu_ppal() != 4);
	return 0;
	}


int menu_ppal(void) //Función para imprimir por pantalla el menú principal
{
    int opcion;
	printf("ESCOJA UNA OPCION Y PULSE ENTER:\n");
	printf("Opcion 1: Barrera sin horario.\nOpcion 2: Tren pasa cada 15 minutos.\nOpcion 3: Verificar sensores.\nOpcion 4: Salir.\n");
	scanf_s("%d", &opcion);
    while (opcion != 1 && opcion != 2 && opcion != 3 && opcion != 4)
    {
        printf("OPCION INCORRECTA\n");
        printf("ESCOJA UNA OPCION Y PULSE ENTER:\n");
        printf("Opcion 1: Barrera sin horario.\nOpcion 2: Tren pasa cada 15 minutos.\nOpcion 3: Verificar sensores.\nOpcion 4: Salir.\n");
        scanf_s("%d", &opcion);
   }
    return opcion;
}

void verifica_sensores(Serial* Arduino, char* port) //Función para verificar el funcionamiento de los sensores
{
    float presion1, presion2;

    if (Arduino->IsConnected())
    {
        presion1 = leer_pulsador_1(Arduino); //Llama a la función que se comunica con el Arduino
        if (presion1 == 1) //Si la función devuelve un 1 significa que el sensor está recibiendo presión
        { 
            printf("Sensor 1 conectado.\n");
        }
        else {
            printf("No se encuentra el sensor 1.\n");
        }
        presion2 = leer_pulsador_2(Arduino);
        if (presion2 == 1)  //Se repite para el segundo sensor
        {
            printf("Sensor 2 conectado.\n");
        }
        else {
            printf("No se encuentra el sensor 2.\n");
        }
    }
}

float leer_pulsador_1(Serial* Arduino)
{
    float presion;
    int bytesrecibidos;
    char mensaje_recibido[MAX_BUFFER];

    bytesrecibidos = enviar_y_recibir(Arduino, "GET_PRESION_1", mensaje_recibido); //Llama a la función encargada de transmitir y recibir mensajes

    if (bytesrecibidos<=0)
    {
        presion = 0;
    }
    else
    {
        presion = 1;
    }
    return presion; //Devuelve un 1 si el pulsador recibe presión y un 0 si no es así
}

float leer_pulsador_2(Serial* Arduino)
{
    float presion = 0;
    int bytesrecibidos;
    char mensaje_recibido[MAX_BUFFER];

    bytesrecibidos = enviar_y_recibir(Arduino, "GET_PRESION_2", mensaje_recibido); //Llama a la función encargada de transmitir y recibir mensajes

    if (bytesrecibidos <= 0)
    {
        presion = 0;
    }
    else
    {
        presion = 1;
    }
    return presion; //Devuelve un 1 si el pulsador recibe presión y un 0 si no es así
}

int enviar_y_recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
    int bytes_recibidos = 0, total = 0;
    int intentos = 0, fin_linea = 0;

    Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
    Sleep(PAUSA_MS);
    bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);

    while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
    {
        if (bytes_recibidos > 0)
        {
            total += bytes_recibidos;
            if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
                fin_linea = 1;
        }
        else
            intentos++;
        Sleep(PAUSA_MS);
        bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
    }
    if (total > 0)
        mensaje_recibir[total - 1] = '\0';

    return total;
}