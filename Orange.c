#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>
#include <string.h>

#define DISTANCIA_VISTA 250
#define DISTANCIA_MUNDO	0
#define ANGULO_MUNDO 0
#define ALTURA_CAMARA 30
#define ANGULO_INCREMENTO 0.174532
#define OSCURIDAD 2000
#define INFINITO 1000

#define X_P	319
#define	Y_P	200 //239

#define X1  l->p1->x
#define Y1  l->p1->y
#define Z1  l->p1->z
#define X2  l->p2->x
#define Y2  l->p2->y
#define Z2  l->p2->z

#define ROJO 16
#define VERDE 32
#define AZUL 48
#define AMARILLO 64
#define CIAN 80
#define MAGENTA 96
#define GRIS 112
#define NEGRO 0
#define ESCALA_DEGR 4
#define FONDO {0,0,0} //{28,29,34}
#define TEXTO {63,63,63} //{40,50,40}

#define CURSOR 126
#define BACKSPACE 8
#define ENTER 13

/*Definicionmes MECH*/
#define ANG_90 1.570796327
#define EZX x_z.p.x
#define EZY x_z.p.y
#define EZZ x_z.p.z
#define EZA x_z.ang


/*extern void modo_svga(void);
extern void color_actual(int c);
extern void modo_texto(void);
extern void pixel(int,int);
extern void obtener_paleta(void*);

extern void linea_a(int,int,int,int);
extern void linea_b(int,int,int,int);
extern void linea_c(int,int,int,int);
extern void linea_d(int,int,int,int);*/

typedef struct punto	/*puntos para coordenadas de vision*/
{
	 float x;
	 float y;
	 float z;
	 struct punto *sig;
}PUNTO;

typedef struct linea   /*lineas para coordenadas de vision*/
{
	PUNTO *p1;
	PUNTO *p2;
	char color;
	struct linea *sig;
}LINEA;

/*Estructuras Mech*/
typedef struct espacio_z
{
	PUNTO p;
	float ang;
}ESPACIO_Z;

typedef	struct caja
{
	PUNTO *p1;
	PUNTO *p2;
	float q; 	//angulo 1
	float r; 	//angulo 2
	int l;  	//longitud
	int a;  	//a=(ancho/2)
	char color;
	/*Adicional*/
	LINEA *anterior;
	LINEA *ultima;
}CAJA;

typedef	struct cunha
{
	PUNTO *p1;
	PUNTO *p2;
	float q; 	//angulo 1
	float r; 	//angulo 2
	int l;  	//longitud
	int a;  	//a=(ancho/2)
	char color;
	/*Adicional*/
	LINEA *anterior;
	LINEA *ultima;
}CUNHA;


long int D=DISTANCIA_VISTA;
long int DM=DISTANCIA_MUNDO;

static PUNTO  *primer_punto,*ultimo_punto;
static LINEA  *primera_linea,*ultima_linea;

unsigned char fuente[1024];

/*Globales Mech*/
ESPACIO_Z x_z;

/*FUNCIONES BASICAS********************************************************/

void dibujar_linea(struct linea *l,int color)  //linea en 3d->linea2d
{
	int xp1,yp1;  /*posiciones en pantalla*/
	int xp2,yp2;
	long int h,m;

	h=Y1+D;
	xp1=(X1*D)/h;
	yp1=(Z1*D)/h;

	h=Y2+D;
	xp2=(X2*D)/h;
	yp2=(Z2*D)/h;

	setcolor(color);
	line(xp1+X_P,yp1+Y_P,xp2+X_P,yp2+Y_P);
}

void dibujar_mundo(void)
{
	LINEA *l;
	l=primera_linea->sig;
	while(l!=NULL)
	{
		dibujar_linea(l,l->color);
		l=l->sig;
	}
}

void borrar_mundo(void)
{
	LINEA *l;
	l=primera_linea;
	while(l!=NULL)
	{
		dibujar_linea(l,NEGRO);
		l=l->sig;
	}
}
/*FUNCIONES DE TRANSFORAMCION************************************************/

void transformar_mundo(int temp)
{
	LINEA *l;
	l=primera_linea->sig;
	while(l!=NULL)
	{
		Y1+=temp;
		Y2+=temp;
		dibujar_linea(l,l->color);
		l=l->sig;
	}
}

void rotar_mundo_xm(int sentido)
{
	LINEA *l;
	float radio,angulo;

	l=primera_linea->sig;
	while(l!=NULL)
	{

		radio=sqrt(pow(DM-Z1,2)+pow(Y1,2));
		angulo=asin(Y1/(radio+0.000001));
		if((DM-Z1)<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		Y1=radio*sin(angulo);
		Z1=DM-radio*cos(angulo);

		radio=sqrt(pow(DM-Z2,2)+pow(Y2,2));
		angulo=asin(Y2/(radio+0.000001));
		if (DM-Z2<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		Y2=radio*sin(angulo);
		Z2=DM-radio*cos(angulo);

		dibujar_linea(l,l->color);
		l=l->sig;
	}
}


void rotar_mundo_zm(int sentido)
{
	LINEA *l;
	float radio,angulo;

	l=primera_linea->sig;
	while(l!=NULL)
	{

		radio=sqrt(pow(X1,2)+pow(Y1,2));
		angulo=asin(Y1/(radio+0.000001));
		if(X1<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		Y1=radio*sin(angulo);
		X1=radio*cos(angulo);

		radio=sqrt(pow(X2,2)+pow(Y2,2));
		angulo=asin(Y2/(radio+0.000001));
		if (X2<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		Y2=radio*sin(angulo);
		X2=radio*cos(angulo);

		dibujar_linea(l,l->color);
		l=l->sig;
	}
}

void rotar_mundo_ym(int sentido)
{
	LINEA *l;
	float radio,angulo;

	l=primera_linea->sig;
	while(l!=NULL)
	{

		radio=sqrt(pow(DM-Z1,2)+pow(X1,2));
		angulo=asin(X1/(radio+0.000001));
		if((DM-Z1)<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		X1=radio*sin(angulo);
		Z1=DM-radio*cos(angulo);

		radio=sqrt(pow(DM-Z2,2)+pow(X2,2));
		angulo=asin(X2/(radio+0.000001));
		if (DM-Z2<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		X2=radio*sin(angulo);
		Z2=DM-radio*cos(angulo);

		dibujar_linea(l,l->color);
		l=l->sig;
	}
}

void rotar_mundo_pp_ym(int sentido)
{
	PUNTO *p;
	float radio,angulo;

	p=primer_punto->sig;
	while(p!=NULL)
	{
		radio=sqrt(pow(DM-p->z,2)+pow(p->x,2));
		angulo=asin(p->x/(radio+0.000001));
		if((DM-p->z)<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		p->x=radio*sin(angulo);
		p->z=DM-radio*cos(angulo);

		p=p->sig;
	}
}

void rotar_mundo_pp_xm(int sentido)
{
	PUNTO *p;
	float radio,angulo;

	p=primer_punto->sig;
	while(p!=NULL)
	{
		radio=sqrt(pow(DM-p->y,2)+pow(p->z,2));
		angulo=asin(p->z/(radio+0.000001));
		if((DM-p->y)<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		p->z=radio*sin(angulo);
		p->y=DM-radio*cos(angulo);

		p=p->sig;
	}
}

void rotar_mundo_pp_zm(int sentido)
{
	PUNTO *p;
	float radio,angulo;

	p=primer_punto->sig;
	while(p!=NULL)
	{
		radio=sqrt(pow(DM-p->y,2)+pow(p->x,2));
		angulo=asin(p->x/(radio+0.000001));
		if((DM-p->y)<0) angulo = 3.14159265 - angulo;
		angulo=angulo+(sentido*ANGULO_INCREMENTO);

		p->x=radio*sin(angulo);
		p->y=DM-radio*cos(angulo);

		p=p->sig;
	}
}

void rotar_linea_z(LINEA *l,float angulo_z,int x,int y)
{
	float radio,angulo;

	X1=X1-x;
	Y1=Y1-y;
	X2=X2-x;
	Y2=Y2-y;

	radio=sqrt(pow(X1,2)+pow(Y1,2));
	angulo=asin(Y1/(radio+0.000001));
	if(X1<0) angulo = 3.14159265 - angulo;
		angulo=angulo-angulo_z;

	Y1=radio*sin(angulo)+y;
	X1=radio*cos(angulo)+x;

	radio=sqrt(pow(X2,2)+pow(Y2,2));
	angulo=asin(Y2/(radio+0.000001));
	if (X2<0) angulo = 3.14159265 - angulo;
		angulo=angulo-angulo_z;

	Y2=radio*sin(angulo)+y;
	X2=radio*cos(angulo)+x;
}


/*FUNCIONES DE MEMORIA*******************************************************/


LINEA *crear_linea(int xm1,int ym1,int zm1,int xm2,int ym2,int zm2, int color)
{
	PUNTO *p1,*p2;
	LINEA *l;
	float radio,angulo;

	p1=(PUNTO *)malloc(sizeof(PUNTO));		/*Se reserva la memoria*/
	p2=(PUNTO *)malloc(sizeof(PUNTO));
	l=(LINEA *)malloc(sizeof(LINEA));

	l->p1=p1;
	l->p2=p2;
	l->color=color;


	X1=xm1;		  /*Conversion de las coordenadas mundiales a */
	Y1=DM-ym1;        /*coordenadas de vision*/
	Z1=-zm1+ALTURA_CAMARA;
	X2=xm2;
	Y2=DM-ym2;
	Z2=-zm2+ALTURA_CAMARA;

	ultimo_punto->sig=p1;
	p1->sig=p2;
	p2->sig=NULL;
	ultimo_punto=p2;

	l->sig=NULL;
	ultima_linea->sig=l;
	ultima_linea=l;

	dibujar_linea(l,l->color);
	return l;
}

LINEA *crear_linea_mem(int xm1,int ym1,int zm1,int xm2,int ym2,int zm2, int color)
{
	PUNTO *p1,*p2;
	LINEA *l;
	float radio,angulo;

	p1=(PUNTO *)malloc(sizeof(PUNTO));		/*Se reserva la memoria*/
	p2=(PUNTO *)malloc(sizeof(PUNTO));
	l=(LINEA *)malloc(sizeof(LINEA));

	l->p1=p1;
	l->p2=p2;
	l->color=color;


	X1=xm1;		  /*Conversion de las coordenadas mundiales a */
	Y1=DM-ym1;        /*coordenadas de vision*/
	Z1=-zm1+ALTURA_CAMARA;
	X2=xm2;
	Y2=DM-ym2;
	Z2=-zm2+ALTURA_CAMARA;

	ultimo_punto->sig=p1;
	p1->sig=p2;
	p2->sig=NULL;
	ultimo_punto=p2;

	l->sig=NULL;
	ultima_linea->sig=l;
	ultima_linea=l;

	//dibujar_linea(l,l->color);
	return l;
}


void crear_linea_rel(int xm2,int ym2,int zm2,int color)
{
	PUNTO *p1,*p2;
	LINEA *l;
	float radio,angulo;

	p2=(PUNTO *)malloc(sizeof(PUNTO));		//Se reserva la memoria
	l=(LINEA *)malloc(sizeof(LINEA));

	l->p1=ultima_linea->p2;
	l->p2=p2;
	l->color=color;

	X2=xm2;   		//Conversion de las coordenadas mundiales a
	Y2=-ym2;        //coordenadas de vision
	Z2=DM*zm2;

	//Z2=DM-zm2;

	l->sig=NULL;
	ultima_linea->sig=l;
	ultima_linea=l;

	ultimo_punto->sig=p2;
	p2->sig=NULL;
	ultimo_punto=p2;


	dibujar_linea(l,l->color);
}

/*FUNCIONES DE TEXTO/INTERFAZ***********************************************/


void crear_superficie(void)
{
	int i;
	for(i=-8;i<=8;i++)
	{
		crear_linea(i*10,80,0,i*10,-80,0,ROJO+4);
		crear_linea(-80,i*10,0,80,i*10,0,ROJO+4);
	}
}

void crear_ejes(void)
{
   crear_linea(-105,100,0,-80,100,0,GRIS+2);
   crear_linea(-100,95,0,-100,120,0,GRIS+2);
   crear_linea(-100,100,-5,-100,100,20,GRIS+2);

	/*X*/
   crear_linea(-70,100,3,-75,100,-3,AMARILLO);
   crear_linea(-75,100,3,-70,100,-3,AMARILLO);

   /*Y*/
   crear_linea(-103,123,3,-100,123,0,MAGENTA);
   crear_linea(-97,123,3,-100,123,0,MAGENTA);
   crear_linea(-100,123,-3,-100,123,0,MAGENTA);

   /*Z*/
   crear_linea(-103,100,30,-97,100,30,CIAN);
   crear_linea(-97,100,30,-103,100,23,CIAN);
   crear_linea(-103,100,23,-97,100,23,CIAN);
}
/*FUNCIONES PARA ABRIR/GUARDAR MUNDOS**************************************/

void salvar_mundo(char *nombre)
{
	LINEA *l;
	PUNTO *p1,*p2;
	FILE *archivo;

	archivo=fopen(nombre,"wb");

	l=primera_linea->sig;
	while(l!=NULL)
	{
		fwrite(l->p1,sizeof (PUNTO),1,archivo);
		fwrite(l->p2,sizeof (PUNTO),1,archivo);
		fwrite(&l->color,1,1,archivo);
		l=l->sig;
	}
	fclose(archivo);
}

void abrir_mundo(char* nombre)
{
	PUNTO p1,p2;
	FILE *archivo;
	char color;

	archivo=fopen(nombre,"rb");

	while(!feof(archivo))
	{
		fread(&p1,sizeof (PUNTO),1,archivo);
		fread(&p2,sizeof (PUNTO),1,archivo);
		fread(&color,1,1,archivo);
		crear_linea(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,color);
	}
	fclose(archivo);
}

/*FIGURAS******************************************************************/

LINEA *dibujar_linea_espacio_z(int x1,int y1,int z1,int x2,int y2,int z2,float angulo_z,int color)
{
	LINEA *l;
	l=crear_linea_mem(x1+EZX,z1+EZY,y1+EZZ,x2+EZX,z2+EZY,y2+EZZ,color);
	rotar_linea_z(l,angulo_z,EZX,EZY);
	dibujar_linea(l,color);
	return l;
}

LINEA *dibujar_caja_espacio_z(int x,int y,int z,float q,float angulo_z,int l,int a,int color)
{
	LINEA *li;
	float r=ANG_90-q;
	float l_sin_q=l*sin(q);
	float l_cos_q=l*cos(q);
	float a_sin_r=a*sin(r);
	float a_cos_r=a*cos(r);
	float x2=x+l_cos_q;
	float y2=y+l_sin_q;

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x2+a_cos_r,y2-a_sin_r,z+a,angulo_z,color);
	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z+a,x2-a_cos_r,y2+a_sin_r,z+a,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z-a,x2+a_cos_r,y2-a_sin_r,z-a,angulo_z,color);
	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z-a,x2-a_cos_r,y2+a_sin_r,z-a,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x-a_cos_r,y+a_sin_r,z+a,angulo_z,color);
	dibujar_linea_espacio_z(x2+a_cos_r,y2-a_sin_r,z+a,x2-a_cos_r,y2+a_sin_r,z+a,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z-a,x-a_cos_r,y+a_sin_r,z-a,angulo_z,color);
	dibujar_linea_espacio_z(x2+a_cos_r,y2-a_sin_r,z-a,x2-a_cos_r,y2+a_sin_r,z-a,angulo_z,color);

	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z+a,x-a_cos_r,y+a_sin_r,z-a,angulo_z,color);
	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x+a_cos_r,y-a_sin_r,z-a,angulo_z,color);

	dibujar_linea_espacio_z(x2-a_cos_r,y2+a_sin_r,z+a,x2-a_cos_r,y2+a_sin_r,z-a,angulo_z,color);
	dibujar_linea_espacio_z(x2+a_cos_r,y2-a_sin_r,z+a,x2+a_cos_r,y2-a_sin_r,z-a,angulo_z,color);

	li=dibujar_linea_espacio_z(x,y,z,x2,y2,z,angulo_z,GRIS+10);	//Eje central

	return li;
}

LINEA *dibujar_caja_struct_espacio_z(CAJA *c)
{
	return dibujar_caja_espacio_z((c->p1)->x,(c->p1)->y,(c->p1)->z,c->q,EZA,c->l,c->a,c->color);
}

CAJA *crear_caja_espacio_z(int x,int y,int z,float q,float angulo_z,int l,int a,char color)
{
	 CAJA *c;
	 LINEA *li;

	 c=(CAJA*)malloc(sizeof(CAJA));
	 c->p1=(PUNTO*)malloc(sizeof(PUNTO));
	 c->p2=(PUNTO*)malloc(sizeof(PUNTO));

	 c->anterior=ultima_linea;
	 c->q=q;
	 c->r=angulo_z;
	 c->l=l;
	 c->a=a;
	 c->color=color;

	 c->p1->x=x;
	 c->p1->y=y;
	 c->p1->z=z;

	 c->p2->x=x+l*cos(q);
	 c->p2->y=y+l*sin(q);
	 c->p2->z=z;

	 li=dibujar_caja_espacio_z(x,y,z,q,angulo_z,l,a,color);

	 c->ultima=li;
	 return c;
}

void borrar_caja(CAJA *c)	//Borra imagen y lineas en memoria
{
	LINEA *l=c->anterior->sig;//->sig?

	if(c->ultima->sig==NULL)crear_linea(0,0,0,0,0,0,NEGRO); //comodin
	(c->anterior)->sig=(c->ultima)->sig;

	while(l!=c->ultima)
	{
		free(l->p1);
		free(l->p2);
		free(l);
		l=l->sig;
	}
}

LINEA *dibujar_cunha_espacio_z(int x,int y,int z,float q,float angulo_z,int l,int a,int color)
{
	LINEA *li;
	float r=ANG_90-q;
	float l_sin_q=l*sin(q);
	float l_cos_q=l*cos(q);
	float a_sin_r=a*sin(r);
	float a_cos_r=a*cos(r);
	float x2=x+l_cos_q;
	float y2=y+l_sin_q;

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x2+a_cos_r,y2-a_sin_r,z,angulo_z,color);
	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z+a,x2-a_cos_r,y2+a_sin_r,z,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z-a,x2+a_cos_r,y2-a_sin_r,z,angulo_z,color);
	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z-a,x2-a_cos_r,y2+a_sin_r,z,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x-a_cos_r,y+a_sin_r,z+a,angulo_z,color);
	dibujar_linea_espacio_z(x2+a_cos_r,y2-a_sin_r,z,x2-a_cos_r,y2+a_sin_r,z,angulo_z,color);

	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z-a,x-a_cos_r,y+a_sin_r,z-a,angulo_z,color);


	dibujar_linea_espacio_z(x-a_cos_r,y+a_sin_r,z+a,x-a_cos_r,y+a_sin_r,z-a,angulo_z,color);
	dibujar_linea_espacio_z(x+a_cos_r,y-a_sin_r,z+a,x+a_cos_r,y-a_sin_r,z-a,angulo_z,color);

	li=dibujar_linea_espacio_z(x,y,z,x2,y2,z,angulo_z,GRIS+10);	//Eje central

	return li;
}

LINEA *dibujar_cunha_struct_espacio_z(CUNHA *c)
{
	return dibujar_caja_espacio_z((c->p1)->x,(c->p1)->y,(c->p1)->z,c->q,EZA,c->l,c->a,c->color);
}

CUNHA *crear_cunha_espacio_z(int x,int y,int z,float q,float angulo_z,int l,int a,char color)
{
    CUNHA *c;
	 LINEA *li;

	 c=(CAJA*)malloc(sizeof(CAJA));
	 c->p1=(PUNTO*)malloc(sizeof(PUNTO));
	 c->p2=(PUNTO*)malloc(sizeof(PUNTO));

	 c->anterior=ultima_linea;
	 c->q=q;
	 c->r=angulo_z;
	 c->l=l;
	 c->a=a;
	 c->color=color;

	 c->p1->x=x;
	 c->p1->y=y;
	 c->p1->z=z;

	 c->p2->x=x+l*cos(q);
	 c->p2->y=y+l*sin(q);
	 c->p2->z=z;

	 li=dibujar_caja_espacio_z(x,y,z,q,angulo_z,l,a,color);

	 c->ultima=li;
	 return c;

}

void borrar_cunha(CUNHA *c)	//Borra imagen y lineas en memoria
{
   LINEA *l=c->anterior->sig;//->sig?

	if(c->ultima->sig==NULL)crear_linea(0,0,0,0,0,0,NEGRO); //comodin
	(c->anterior)->sig=(c->ultima)->sig;

	while(l!=c->ultima)
	{
		free(l->p1);
		free(l->p2);
		free(l);
		l=l->sig;
	}
}


/*FIN**********************************************************************/
