#define BARRA 0,20,639,479,1
#define TITULO 0,0,639,19,1
#define RELOJ 574,201,639,221,1
#define NBOTONES 2
#define FLECHA_ABAJO 0x5000
#define FLECHA_ARRIBA 0x4800
#define ENTER 0x1C0D
#define EENTER 13

/*FUNCIONES DE INICIALIZACION DEL VIDEO************************************/
int huge detectar()
{
	return 2;
}

void iniciar_graficas(void)
{
   int contr=0;
   int modo=0;
   installuserdriver("svga256",detectar);
   initgraph(&contr,&modo,"");
}

void obtener_pal256(char *paleta)
{
	struct REGPACK reg;

	reg.r_ax = 0x1017;
	reg.r_bx = 0;
	reg.r_cx = 256;
	reg.r_es = FP_SEG(paleta);
	reg.r_dx = FP_OFF(paleta);
	intr(0x10,&reg);
}

void modificar_pal256(char *paleta)
{
	struct REGPACK reg;

	reg.r_ax = 0x1012;
	reg.r_bx = 0;
	reg.r_cx = 256;
	reg.r_es = FP_SEG(paleta);
	reg.r_dx = FP_OFF(paleta);
	intr(0x10,&reg);
}

void cargar_paleta(void)
{
	int i;
	char fondo[]=FONDO;
	char texto[]=TEXTO;
	char paleta[256][3];

	obtener_pal256(paleta[0]);

	for(i=16;i<256;i++)
	{
		paleta[i][0]=0;
		paleta[i][1]=0;
		paleta[i][2]=0;
	}
	paleta[0][0]=fondo[0];
	paleta[0][1]=fondo[1];
	paleta[0][2]=fondo[2];

	paleta[7][0]=texto[0];
	paleta[7][1]=texto[1];
	paleta[7][2]=texto[2];


	for(i=0;i<16;i++)
	{
	//ROJO
		paleta[i+ROJO][0]=63-i*ESCALA_DEGR;
	//VERDE
		paleta[i+VERDE][1]=63-i*ESCALA_DEGR;
	//AZUL
		paleta[i+AZUL][2]=63-i*ESCALA_DEGR;
	//AMARILLO
		paleta[i+AMARILLO][0]=63-i*ESCALA_DEGR;
		paleta[i+AMARILLO][1]=63-i*ESCALA_DEGR;
	//CIAN
		paleta[i+CIAN][1]=63-i*ESCALA_DEGR;
		paleta[i+CIAN][2]=63-i*ESCALA_DEGR;
	//MAGENTA
		paleta[i+MAGENTA][0]=63-i*ESCALA_DEGR;
		paleta[i+MAGENTA][2]=63-i*ESCALA_DEGR;
	//GRIS
		paleta[i+GRIS][0]=63-i*ESCALA_DEGR;
		paleta[i+GRIS][1]=63-i*ESCALA_DEGR;
		paleta[i+GRIS][2]=63-i*ESCALA_DEGR;
	}
	modificar_pal256(paleta[0]);
}

/*PRIMITIVAS DE TEXTO********************************************************/

int cargar_fuente(char* nombre)
{
	FILE *archivo_fuente;
	archivo_fuente=fopen(nombre,"rb");
	if(archivo_fuente==NULL) return 0;
	fread(fuente,sizeof fuente,1,archivo_fuente);
	fclose(archivo_fuente);
	return 1;
}

void caracter(int x,int y,unsigned char c)
{
	int i,j,k,linea,potencia=128;
	char letra[8];
	for(i=0;i<8;i++)
	{
		linea=fuente[c*8+i];
		for(j=0;j<8;j++)
		{
			if(linea>=potencia)
			{
				letra[j]=7;
				linea-=potencia;
			}
			else letra[j]=0;
			potencia=potencia/2;
		}
		potencia=128;
		for(k=0;k<8;k++)
		{
			putpixel(x,y,letra[k]);
			x++;
		}
		x=x-8;
		y++;
	}
}

void imprimir(int x,int y,unsigned char* cadena)
{
	int i,l,nx;
	nx=x;
	l=strlen(cadena);
	for(i=0;i<l;i++)
	if(cadena[i]=='\n')
	{
		nx=x;
		y=y+12;
	}
	else
	{
		caracter(nx,y,cadena[i]);
		nx+=8;
	}
}

void imprimir_int(int x,int y,int dato)
{
	unsigned char cadena[6];
	sprintf(cadena,"%d",dato);
	imprimir(x,y,cadena);
}

void imprimir_float(int x,int y,float f)
{
   char buf[256];
   sprintf(buf,"%f",f);
   imprimir(x,y,buf);
}

void imprimir_grados(int xx,int yy,long double x)
{
   long double z;
   int y = x;
   int ent;
   int n=0;
   int iter=4;
   int xxx;			//Aqui va la presicion, se supone que la ingresa el usuario
   z = x-y;
   while(n<iter)
   {
      ent=x;
      z=x-ent;
      x=z*60;
      xxx=xx+36*n;
      imprimir_int(xxx,yy,ent);
      imprimir(xxx-12,yy,".");
      n++;
   }
}


char* leer(int x,int y,char* cadena,int limite)
{
	unsigned char tecla;
	int nx=x,i=0;
	do
	{
	caracter(nx,y,CURSOR);
	tecla=getch();
	if(tecla==BACKSPACE)
	{
		if(nx>x)
		{
			caracter(nx,y,'#');
			nx-=8;
			caracter(nx,y,'#');
			i--;
			cadena[i]=0;
		}
	}
	else
	{
		if(i<limite)       //Control del limite
		{
			caracter(nx,y,tecla);
			nx+=8;
			cadena[i]=tecla;
			i++;
		}
	}
	}while(tecla!=EENTER);

	strcat(cadena,"\0");
	return cadena;

}

void limpiar(void)
{
	int i;
	for(i=351;i<479;i+=8)
		imprimir(1,i,"                                                               ");
}


/*PRIMITIVAS IDE************************************************************/

struct boton
{
	int sw;
	int x1,y1,a,h;
	char* til;
};


void dibujar_boton(struct boton *b)
{
	int ctex,cfon,cline,len,posx,posy;
	cline=8;
	if(b->sw==0)
	{
		ctex=0;
		cfon=GRIS+5;
	}
	else
	{
		ctex=15;
		cfon=AZUL+5;
	}

	setfillstyle(1,cfon);

	len = strlen(b->til);
	posx = b->x1 + b->a/2 - (len*8)/2;
	posy = b->y1 + b->h/2 - 4;

	setcolor(cline);
	rectangle(b->x1,b->y1,b->x1+b->a,b->y1+b->h);
	rectangle(b->x1+2,b->y1+2,b->x1+b->a-2,b->y1+b->h-2);

	floodfill(b->x1+3,b->y1+3,cline);

	setcolor(ctex);
	outtextxy(posx,posy,b->til);
}

void crear_boton(struct boton *b,int x,int y,int an,int ha,char* n)
{
	b->x1=x;
	b->y1=y;
	b->a=an;
	b->h=ha;
	b->til=n;
	b->sw=0;
	dibujar_boton(b);
}

void push(struct boton *b)
{
	if(b->sw==1)b->sw=0;
	else b->sw=1;
	dibujar_boton(b);
}

void barra_titulo(char* til)
{
	int color_actual,len;
	len = strlen(til);

	setcolor(8);
	rectangle(0,0,639,19);
	setfillstyle(1,4);
	floodfill(2,2,8);
	setcolor(15);
	imprimir(300-(len*8)/2 ,6,til);

}

void limpiar_pantalla(void)
{
	cleardevice();
}

/*void barra_estado(int nc)
{

} */
/*COMANDANTE****************************************************************/
/*void ejecutar_comando(char *com)
{
	float p1,p2,p3;
	char nombre[64];
	sscanf(com,"%s%f%f%f",nombre,&p1,&p2,&p3);
	mover_robot(lima,p1,p2,p3);

} */

/*INTERFAZ CON EL USUARIO***************************************************/

void interfaz(void)
{

	//	limpiar_pantalla();
	setviewport(TITULO);
	clearviewport();
	barra_titulo("=*CINEMATIC*=");
	setviewport(BARRA);
	clearviewport();

	/*setcolor(AZUL+3);
	line(0,0,639,0);
	line(0,18,639,18);
	line(0,0,0,479);
	line(639,0,639,479);
	line(0,479,639,479);
	line(0,340,639,340);*/
}

int barra_comandos(int id)
{
	int m,i;
	struct boton b[NBOTONES];
	char tile[][16]={"Observar",
				 "Salir"};


	for(i=0;i<NBOTONES;i++)
		crear_boton(&b[i],0,i*20,350,20,tile[i]);

	push(&b[id]);

	for(i=0;i<NBOTONES;i++)
		dibujar_boton(&b[i]);

	setcolor(VERDE);
	rectangle(16,296,178,376);
	imprimir(20,300,"q1=\nq2=\nq3=\nl1=\nl2=\nl3=");
	imprimir_grados(50,300,q1*RAD_PI);
	imprimir_grados(50,312,q2*RAD_PI);
	imprimir_grados(50,324,q3*RAD_PI);
	imprimir_float(50,336,l1);
	imprimir_float(50,348,l2);
	imprimir_float(50,360,l3);

	rectangle(178,296,298,376);
	imprimir(185,300,"x=\ny=\nz=\n");
	imprimir_float(205,300,x);
	imprimir_float(205,312,y);
	imprimir_float(205,324,z);

	do
	{
		//mouse();
		if(bioskey(1)!=0)
		{
			m=bioskey(0);

			switch(m)
			{
				case FLECHA_ABAJO:
					push(&b[id]);
					id++;
					if(id>NBOTONES-1)id=0;
					push(&b[id]);
				break;

				case FLECHA_ARRIBA:
					push(&b[id]);
					id--;
					if(id<0)id=NBOTONES-1;
					push(&b[id]);
				break;

				case ENTER:
					return id;
				break;

				default:
					return m;
			}
			m=0;
		}
	}while(1);

	return 10;
}

void observar_modelo(void)
{
	char m;
	char com[64];

	do
	{
		//m=getch();
		m=barra_comandos(m);
		switch(m)
		{

			case 'y':
				borrar_mundo();
				rotar_mundo_ym(+1);
			break;

			case '-':
				borrar_mundo();
				D+=10;
				dibujar_mundo();
			break;

			case '+':
				borrar_mundo();
				if(D>200)D-=10;
				dibujar_mundo();
			break;

			case 'o':
				/*borrar_mundo();
				transformar_mundo(+10);
				getch();
				borrar_mundo();
				transformar_mundo(-10);
				interfaz();
				imprimir(2,360,"Use las flechas para rotar, \"o\" para aumentar, ESC para salir.");*/
			break;

			case 'c':
			   /*	imprimir(10,0,"COM>");
				leer(50,0,com,64);
				ejecutar_comando(com);*/
			break;

			case '6':            //flecha derecha
				borrar_mundo();
				rotar_mundo_zm(-1);
				dibujar_mundo();
			break;

			case '4':           //flecha izquierda
				borrar_mundo();
				rotar_mundo_zm(+1);
				dibujar_mundo();
			break;

			case '8':   		 	//flecha arriba
				borrar_mundo();
				rotar_mundo_xm(-1);
				dibujar_mundo();
			break;

			case '2':           	//flecha abajo
				borrar_mundo();
				rotar_mundo_xm(+1);
				dibujar_mundo();
			break;

			case 1:
				return;
			break;
		}

	}while(m!=27);
	//restorecrtmode();
}

int control_break(void)
{
   restorecrtmode();
   printf("Programa abortado.\n");
	return (0);
}




