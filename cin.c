#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include "orange.c"

#define PI	   3.1415926536
#define DOS_PI 6.2831853072
#define PI_RAD 0.0174539252
#define RAD_PI 57.2957795131

/*Animacion*/
#define PASOS 4
#define DELAY 120

typedef struct robot
{
	float ang;
	CAJA *c1;
	CAJA *c2;
	CAJA *c3;
	CUNHA *ch1;
}ROBOT;

ROBOT *lima;
/*long double l1,l2,l3;
long double x,y,z;
long double sq3,cq3;
long double q1,q2,q3;*/


ROBOT *crear_robot(int l1,int l2, int l3,float q1,float q2,float q3)
{
	ROBOT *r;
	r=(ROBOT*)malloc(sizeof(ROBOT));

	r->ang=q1;

	r->c1=crear_caja_espacio_z(0,0,0,90*PI_RAD,q1,l1,6,VERDE+6);

	r->c2=crear_caja_espacio_z(0,0,0,q2,q1,l2,4,ROJO);

	r->c3=crear_caja_espacio_z(r->c2->p2->x,r->c2->p2->y,r->c2->p2->z,q2+q3,q1,l3,6,CIAN+5);
	r->c3->p1=r->c2->p2;

	r->ch1=crear_cunha_espacio_z(r->c3->p2->x,r->c3->p2->y,r->c3->p2->z-5,q2+q3,q1,9,5,AMARILLO+1);
	r->ch1->p1=r->c3->p2;

	return r;
}

void mover_robot(ROBOT *r,float nq1, float nq2, float nq3)
{
	double inc;
	LINEA *com;

	inc=nq1/PASOS;

	while(fabs(EZA)<fabs(nq1))
	{
		borrar_mundo();
		EZA+=inc;

		borrar_cunha(r->ch1);
		borrar_caja(r->c3);
		borrar_caja(r->c2);
		borrar_caja(r->c1);

		r->c1->anterior=ultima_linea;
		r->c1->ultima=dibujar_caja_struct_espacio_z(r->c1);

		r->c2->anterior=ultima_linea;
		r->c2->ultima=dibujar_caja_struct_espacio_z(r->c2);

		r->c3->anterior=ultima_linea;
		r->c3->ultima=dibujar_caja_struct_espacio_z(r->c3);

		r->ch1->anterior=ultima_linea;
		r->ch1->ultima=dibujar_cunha_struct_espacio_z(r->ch1);

		dibujar_mundo();
		//sound(100);
		delay(DELAY);
		//nosound();
	}

	inc=nq2/PASOS;

	while(fabs(r->c2->q)<fabs(nq2))
	{
		r->c2->q+=inc;
		r->c3->q+=inc;
		r->ch1->q+=inc;

		borrar_mundo();

		borrar_cunha(r->ch1);
		borrar_caja(r->c3);
		borrar_caja(r->c2);


		r->c2->anterior=ultima_linea;
		r->c2->ultima=dibujar_caja_struct_espacio_z(r->c2);

		r->c2->p2->x=r->c2->p1->x+r->c2->l*cos(r->c2->q);
		r->c2->p2->y=r->c2->p1->y+r->c2->l*sin(r->c2->q);

		r->c3->anterior=ultima_linea;
		r->c3->ultima=dibujar_caja_struct_espacio_z(r->c3);

		r->c3->p2->x=r->c3->p1->x+r->c3->l*cos(r->c3->q);
		r->c3->p2->y=r->c3->p1->y+r->c3->l*sin(r->c3->q);

		r->ch1->anterior=ultima_linea;
		r->ch1->ultima=dibujar_cunha_struct_espacio_z(r->ch1);

		dibujar_mundo();
		//sound(200);
		delay(DELAY);
		//nosound();

	}

	inc=nq3/PASOS;

	while(fabs(r->c3->q)<fabs(nq2+nq3))
	{
		r->c3->q+=inc;
		r->ch1->q+=inc;
		borrar_mundo();

		borrar_cunha(r->ch1);
		borrar_caja(r->c3);

		r->c3->anterior=ultima_linea;
		r->c3->ultima=dibujar_caja_struct_espacio_z(r->c3);

		r->c3->p2->x=r->c3->p1->x+r->c3->l*cos(r->c3->q);
		r->c3->p2->y=r->c3->p1->y+r->c3->l*sin(r->c3->q);

		r->ch1->anterior=ultima_linea;
		r->ch1->ultima=dibujar_cunha_struct_espacio_z(r->ch1);

		dibujar_mundo();
		//sound(600);
		delay(DELAY);
		//nosound();
	}
}

#include "halla.c"
#include "interfaz.c"

void iniciar_motor_grafico(void)
{
	PUNTO *p1,*p2;

	primera_linea=(LINEA *)malloc(sizeof(LINEA));
	p1=(PUNTO *)malloc(sizeof(PUNTO));		/*Se reserva la memoria*/
	p2=(PUNTO *)malloc(sizeof(PUNTO));

	p1->x=0;
	p1->y=0;
	p1->z=0;
	p2->x=0;
	p2->y=0;
	p2->z=0;

	primera_linea->p1=p1;
	primera_linea->p2=p2;
	primera_linea->sig=NULL;
	ultima_linea=primera_linea;

	primer_punto=p2;
	primer_punto->sig=NULL;
	ultimo_punto=primer_punto;

	 ctrlbrk(control_break);
	cargar_fuente("natural.fn");
	iniciar_graficas();
	cargar_paleta();
	interfaz();
	crear_superficie();
   crear_ejes();
}


int main()
{
	/*Parte texto***********************************************************/

	presentacion();
	ingresarTol();
	ingresarDatos();
	while(imposible==1)
	{
		fflush(stdin);
		getch();
		clrscr();
		imposible=0;
		ingresarDatos();
	}
	calcularQ1();
	calcularQ3();
	calcularQ2();
	//Hasta aqui vienen los calculos de q1, q2 y q3 con los datos iniciales validos
	printf("\n\nfinalizo q1=%Lf q2=%Lf q3=%Lf",q1,q2,q3);
	getch();

	/*Parte grafica*********************************************************/

	iniciar_motor_grafico();

	EZX=0;
	EZY=0;
	EZZ=l1;
	EZA=0*PI_RAD;

	lima=crear_robot(-EZZ,l2,l3,x_z.ang,0*PI_RAD,0*PI_RAD);

	crear_linea(x,y,0,x,y,z+l1,MAGENTA);
	crear_linea(x-10,y,z+l1,x+10,y,z+l1,10);
	crear_linea(x,y-10,z+l1,x,y+10,z+l1,10);

	limpiar();
	imprimir(2,400,"Las posicion es correcta, presione una tecla para comenzar...");
	getch();
	limpiar();
	imprimir(2,400,"Use el teclado numerico para rotar, las flechas para seleccionar, ESC para salir.");


	mover_robot(lima,q1,q2,q3);

	observar_modelo();

	restorecrtmode();
	return 0;
}

