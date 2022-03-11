#include <stdio.h>
#include <stdlib.h>

int menu_ppal(void); //Funciones prototipo

int main(void)
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
        printf("FUNCIONA\n"); //Si se escoge esta opción, la barrera sube y baja para dos trenes y se mide la diferencia de tiempo entre el valor elegido y el real.
        break;
    case 4:
        printf("FUNCIONA\n"); //Si se escoge esta opción, el programa se acaba.
        break;
    }
	return 0;
	}
int menu_ppal(void) //Función para imprimir por pantalla el menú principal
{
    int opcion;
	printf("ESCOJA UNA OPCION Y PULSE ENTER:\n");
	printf("Opcion 1: Barrera sin horario.\nOpcion 2: Tren pasa cada 15 minutos.\nOpcion 3: Tren pasa cada 30 minutos\nOpcion 4: Salir\n");
	scanf_s("%d", &opcion);
    while (opcion != 1 && opcion != 2 && opcion != 3 && opcion != 4)
    {
        printf("OPCION INCORRECTA\n");
        printf("ESCOJA UNA OPCION Y PULSE ENTER:\n");
        printf("Opcion 1: Barrera sin horario.\nOpcion 2: Tren pasa cada 15 minutos.\nOpcion 3: Tren pasa cada 30 minutos\nOpcion 4: Salir\n");
        scanf_s("%d", &opcion);
   }
    return opcion;
}