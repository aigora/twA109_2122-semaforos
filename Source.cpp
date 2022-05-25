#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <cstdint>
#include <inttypes.h>
#include <locale.h>
#include <conio.h>
#include "SerialClass/SerialClass.h"

#define MAX_BUFFER 200
#define PAUSA_MS 200
#define LONGCAD 50
#define SI 1
#define NO 0
#define VERSION_3

//==============================================================================================================================//
//STRUCTS MEMORIA DINÁMICA
typedef struct
{
	char nombre[LONGCAD];
	char apellidos[LONGCAD];
	char identificador[LONGCAD];
	int salario;
}usuario;   

struct item 
{
	usuario usuario;
	struct item* enlace;
};

typedef struct item Nodo;

Nodo* lista = NULL;

//==============================================================================================================================//
//FUNCIONES PROTOTIPO
//Funciones menú
int menu_ppal(void);
int menu_trabajadores(void);

//Funciones Arduino
void comprobar_conexion(Serial*, char*);
void comprobar_sensores(Serial*, char*);
void barrera(Serial*, char*);

//Funciones comunicación con el Arduino 
float float_from_cadena(char* cadena);
int Enviar_y_Recibir(Serial*, const char*, char*);

//Funciones memoria dinámica
usuario leer_datos_usuario(char operacion);
int alta_usuario_v3(Nodo** lista);
void mostrar_datos_usuario(usuario u);
void listado_usuarios_v3(Nodo* lista);
int consulta_usuario_v3(Nodo* lista);
void modificacion_usuario_v3(Nodo* lista);
int baja_usuario_v3(Nodo** lista);

//Funciones persistencia
Nodo* leer_fichero_usuarios_v3(void);
int escribir_fichero_usuarios_v3(Nodo* lista);

//==============================================================================================================================//
int main(void)
{
	Serial* Arduino;  //Representa internamente la placa de Arduino
	char puerto[] = "COM3"; //Puerto serie al que está conectado el Arduino, segundo puerto USB empezando por abajo a la izquierda en este caso
	int opcion_menu, opcion_menu_trabajadores;
	int resultado = -100;  //Valor arbitrario distinto de 0
	lista = leer_fichero_usuarios_v3();

	setlocale(LC_ALL, "es_ES");
	Arduino = new Serial((char*)puerto);
	do
	{
		opcion_menu = menu_ppal();
		switch (opcion_menu)
		{
		case 1:
			comprobar_conexion(Arduino, puerto);
			break;
		case 2:
			comprobar_sensores(Arduino, puerto);
			break;
		case 3:
			barrera(Arduino, puerto);
			break;
		case 4:
			do {
				opcion_menu_trabajadores = menu_trabajadores();
				switch (opcion_menu_trabajadores)
				{
				case 1:
					resultado = alta_usuario_v3(&lista);
					break;
				case 2:
					listado_usuarios_v3(lista);
					break;
				case 3:
					consulta_usuario_v3(lista);
					break;
				case 4:
					modificacion_usuario_v3(lista);
					break;
				case 5:
					baja_usuario_v3(&lista);
					break;
				case 6:
					break;
				default:
						printf("Opción incorrecta, escoja otra opción y pulse ENTER\n");
				}
				if (resultado == 0)
				{
					escribir_fichero_usuarios_v3(lista);
				}
			} while (opcion_menu_trabajadores != 6);
			break;
		case 5:
			break;
		default:
			printf("Opción incorrecta, escoja otra opción y pulse ENTER:\n");
		}
	} while (opcion_menu != 5);
	return 0;
}


//==============================================================================================================================//
//FUNCIÓNES MENÚ PRINCIPAL
int menu_ppal(void)
{
	int opcion;
	printf("\n");
	printf("MENÚ PRINCIPAL\n\n");
	printf("1 - Comprobar conexión con la placa Arduino\n");
	printf("2 - Comprobar los sensores\n");
	printf("3 - Poner en funcionamiento el paso a nivel\n");
	printf("4 - Registro de trabajadores\n");
	printf("5 - Salir\n");
	printf("\nOPCIÓN: ");
	scanf_s("%d", &opcion);
	return opcion;
}

int menu_trabajadores(void)
{
	int opcion, intro;
	printf("\n");
	printf("MENÚ REGISTRO DE TRABAJADORES\n\n");
	printf("1 - Dar de alta un nuevo empleado\n");
	printf("2 - Listado de empleados\n");
	printf("3 - Consulta de empleados\n");
	printf("4 - Modificacion de los datos de un empleado\n");
	printf("5 - Dar de baja a un empleado\n");
	printf("6 - Salir del programa\n");
	printf("\nOPCIÓN: ");
	scanf_s("%d", &opcion);
	scanf_s("%c", &intro);
	return opcion;
}

//==============================================================================================================================//
//FUNCIONES ARDUINO
void comprobar_conexion (Serial* Arduino, char* port)
{
	int bytesrecibidos;
	char mensaje_recibido[MAX_BUFFER];

	if (Arduino->IsConnected())
	{
		bytesrecibidos = Enviar_y_Recibir(Arduino, "COMPROBACION\n", mensaje_recibido);
		if (bytesrecibidos <= 0)
		{
			printf("\n No se ha recibido respuesta de la placa Arduino\n");
		}
		else
		{
			printf("\nLa placa Arduino esta correctamente conectada y comunicándose");
		}
	}
	else
	{
		printf("\nConecte por cable la placa Arduino y vuelva a intentarlo\n");
	}
}

void comprobar_sensores(Serial* Arduino, char* port)
{
	int bytesrecibidos;
	char mensaje_recibido[MAX_BUFFER];
	float resultado;

	if (Arduino->IsConnected())
	{
		bytesrecibidos = Enviar_y_Recibir(Arduino, "COMPROBACION_SENSORES\n", mensaje_recibido);
		if (bytesrecibidos <= 0)
		{
			printf("PULSADOR 1 NO ENCONTRADO\n");
			printf("PULSADOR 2 NO ENCONTRADO\n");
		}
		else 
		{
			resultado = float_from_cadena(mensaje_recibido);
			if (resultado==1)
			{
				printf("PULSADOR 1 ENCONTRADO\n");
				printf("PULSADOR 2 NO ENCONTRADO\n");
			}
			else if (resultado == 2)
			{
				printf("PULSADOR 1 NO ENCONTRADO\n");
				printf("PULSADOR 2 ENCONTRADO\n");
			}

			else if (resultado==3)
			{
				printf("PULSADOR 1 ENCONTRADO\n");
				printf("PULSADOR 2 ENCONTRADO\n");
			}
		}
	}
}

void barrera(Serial* Arduino, char* port)
{
	int bytesrecibidos;
	char mensaje_recibido[MAX_BUFFER];
	int salir = 0;

	if (Arduino->IsConnected())
	{
		bytesrecibidos = Enviar_y_Recibir(Arduino, "BARRERA\n", mensaje_recibido);
		printf("Su selección está en curso, pulse la tecla 6 para salir\n");
		while (salir == 0)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case '6':
					bytesrecibidos = Enviar_y_Recibir(Arduino, "SALIR\n", mensaje_recibido);
					salir = 1;
					break;
				default:
					break;
				}
			}
		}
	}
}

//==============================================================================================================================//
//FUNCIONES COMUNICACIÓN CON ARDUINO
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
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

	//printf("LOG: %d bytes -> %s\nIntentos=%d - EOLN=%d\n", total, mensaje_recibir,intentos,fin_linea);
	return total;
}


float float_from_cadena(char* cadena)
{
	float numero = 0;
	int i, divisor = 10, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
			if (cadena[i] >= '0' && cadena[i] <= '9')
				numero = numero * 10 + cadena[i] - '0';
			else
				if (cadena[i] == '.' || cadena[i] == ',')
					estado = 2;
				else
					estado = 3;
			break;
		case 2: // Parte decimal
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = numero + (float)(cadena[i] - '0') / divisor;
				divisor *= 10;
			}
			else
				estado = 3;
			break;
		}
	return numero;
}

//==============================================================================================================================//
//FUNCIONES MEMORIA DINÁMICA
usuario leer_datos_usuario(char operacion)
{
	usuario usuario;
	char intro;

	printf("ALTA DE UN NUEVO USUARIO\n");
	printf("=============================\n");
	printf("Introduzca el nombre del empleado:\n");
	gets_s(usuario.nombre, LONGCAD);
	printf("introduzca los apellidos del empleado:\n");
	gets_s(usuario.apellidos, LONGCAD);
	if (operacion == 'A')//SOLO SE SOLICITA EL IDENTIFICADOR AL DAR EL ALTA, CON EL CARACTER 'M'. SI NO ES ASÍ SE USA EL CARACTER 'N'
	{
		printf("Introduzca identificador:\n");
		gets_s(usuario.identificador, LONGCAD);
	}
	printf("Introduzca salario del empleado:\n");
	scanf_s("%d", &usuario.salario);
	intro = getchar(); //EVITA QUE SE QUEDE UN '\n' EN EL BUFFER
	return usuario;
}

int alta_usuario_v3(Nodo** plista)
{
	Nodo* cab = *plista;
	Nodo* p;
	int cod_error = 0;

	p = (Nodo*)malloc(sizeof(Nodo));
	if (p == NULL)
	{
		printf("No se ha podido reservar memoria para un nuevo usuario\n");
		cod_error = -1;
	}
	else
	{
		p->usuario = leer_datos_usuario('A');
		p->enlace = cab;
		cab = p;
	}
	*plista = cab;
	return cod_error;
}

void mostrar_datos_usuario(usuario u)
{
	printf("%s\t%s\t\t", u.nombre, u.apellidos);
	printf("%d\n", u.salario);
}

void listado_usuarios_v3(Nodo* lista)
{
	Nodo* p;
	if (lista == NULL)
	{
		printf("En este momento no hay usuarios en la aplicacion\n");
	}
	else
	{
		for (p = lista; p != NULL; p = (Nodo*)p->enlace)
			mostrar_datos_usuario(p->usuario);
	}
}

Nodo* posicion_usuario_v3(Nodo* lista, char* identificador)
{
	Nodo* p;

	for (p = lista; p != NULL; p = p->enlace)
		if (strcmp(identificador, p->usuario.identificador) == 0)
			return p;
	return p;
}

int consulta_usuario_v3(Nodo* lista)
{
	char identificador[LONGCAD];
	Nodo* nodo;
	int cod_error = 0;
	printf("Introduzca identificador a buscar:\n");
	gets_s(identificador, LONGCAD);
	nodo = posicion_usuario_v3(lista, identificador);
	if (identificador == NULL)
	{
		printf("No existe ningún usuario con ese identificador\n");
		cod_error = -1;
	}
	else
	{
		mostrar_datos_usuario(nodo->usuario);
	}
	return cod_error;
}

void modificacion_usuario_v3(Nodo* lista)
{
	char identificador[LONGCAD];
	Nodo* nodo;
	usuario nuevos_datos;
	printf("Introduzca identificador a buscar:\n");
	gets_s(identificador, LONGCAD);
	nodo = posicion_usuario_v3(lista, identificador);
	if (nodo == NULL)
	{
		printf("No existe ningun usuario con ese identificador\n");
	}
	else
	{
		mostrar_datos_usuario(nodo->usuario);
		nuevos_datos = leer_datos_usuario('M');
		if (strcmp(nuevos_datos.nombre, "") != 0)
		{
			strcpy_s(nodo->usuario.nombre, LONGCAD, nuevos_datos.nombre);
		}
		if (strcmp(nuevos_datos.apellidos, "") != 0)
		{
			strcpy_s(nodo->usuario.apellidos, LONGCAD, nuevos_datos.apellidos);
		}
		if (nuevos_datos.salario != 0)
		{
			nodo->usuario.salario = nuevos_datos.salario;
		}
	}
}

int baja_usuario_v3(Nodo** lista)
{
	char identificador[LONGCAD], respuesta[2];
	int encontrado = NO;
	Nodo* p, * q;

	if (*lista == NULL)
	{
		printf("La lista esta vacia\n");
	}
	else
	{
		printf("Introduzca identificador a eliminar:\n");
		gets_s(identificador, LONGCAD);
		q = *lista;
		for (p = q; p != NULL && encontrado == NO; p = p->enlace)
		{
			if (strcmp(identificador, p->usuario.identificador) == 0)
			{
				printf("Datos del usuario encontrado:\n");
				mostrar_datos_usuario(p->usuario);
				printf("¿Desea borrar este usuario? Pulse 'S para si o 'N' para no:");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					if (p == q) //SI SE TRATA DEL PRIMERO
					{
						*lista = p->enlace;
					}
					else
					{
						q->enlace = p->enlace;
						free(p);
						break;
					}
				}
				else
					if (p != q)
						q = q->enlace;
			}
			if (encontrado == NO)
				printf("No se ha encontrado el usuario\n");
		}
		return encontrado;
	}
}

//==============================================================================================================================//
//FUNCIONES PERSISTENCIA
Nodo* leer_fichero_usuarios_v3(void)
{
	Nodo* p, * cab = NULL;
	usuario u;
	FILE* fichero;
	errno_t cod_error;

	cod_error = fopen_s(&fichero, "Usuarios.bin", "rb");
	if (cod_error != 0)
	{
		cab = NULL;
	}
	else
	{
		fread_s(&u, sizeof(usuario), sizeof(usuario), 1, fichero);
		while (!feof(fichero))
		{
			p = (Nodo*)malloc(sizeof(Nodo));
			if (p == NULL)
			{
				printf("Memoria insuficiente para leer el fichero\n");
				break;
			}
			else
			{
				p->usuario = u;
				p->enlace = cab;
				cab = p;
			}
			fread_s(&u, sizeof(usuario), sizeof(usuario), 1, fichero);
		}
		fclose(fichero);
	}
	return cab;
}

int escribir_fichero_usuarios_v3(Nodo* lista)
{
	{
		FILE* fichero;
		errno_t err;

		err = fopen_s(&fichero, "Usuarios.bin", "wb");
		if (err == 0)
		{
			while (lista != NULL)
			{
				fwrite(&lista->usuario, sizeof(usuario), 1, fichero);
				lista = lista->enlace;
			}
			fclose(fichero);
		}
		else
		{
			printf("Se ha producido un prolem a la hora de grabar el fichero de usuarios\n");
		}
		return err;
	}

}