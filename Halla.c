#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

long double l1,l2,l3;
long double x,y,z;
long double sq3,cq3;
long double q1,q2,q3;
long double r,l,d;
long double tol = 0.0001;
int ite=10;
int bandera=0;
int imposible=0;
int vraiz=0;
//Retorna -1 si tiene algun problema
long double raiz(long double k)
{
	 long double error;
	 int n;
	 long double x0,x1;
	 vraiz=0;
	 if(k>0)
	 {
		n=0;
		error=tol+1;
		x0=x1=1.0;
		while((n<ite)&&(error>tol))
		{
			if(x0!=0)
			{
				x0=x1;
				x1 = ((0.5)*(x0 + (k/x0)));
					error = (x1-x0);
				if(error<0)
					error=error*(-1);
				n++;
			}
			else
			{
				printf("\nNo se puede calcular la raiz pues no se puede dividir por cero, x0=%Lf",x0);
				vraiz=1;
				break;
			}
		}
		if(error<=tol)
			return x0;
		else
		{
			if(n>=ite)
				printf("\nNo se encontro el resultado de la raiz para %d iteraciones",n);
			vraiz=1;
		}
	}
	else
	{
		if(k==0)
			return 0;
		else
		{
			printf("\nNo se puede sacar la raiz de un numero negativo");
			return -1;
			vraiz=1;
		}
	}
	return -1;
}

void ingresarTol()
{
	char c;
	printf("\nDesea modificar la tolerancia (%Lf) y el numero de iteraciones (%d)?(s/N)\n",tol,ite);
	recdait:
	c=getch();
	if(c=='s')
	{
		printf("\nIngrese la tolerancia con que desea trabajar.");
		printf("\n(Recuerde que es preferible que sea un numero peque¤o (p.ej. 0.00001))");
		printf("\nTolerancia: ");
		scanf("%Lf",&tol);

		printf("\nIngrese el numero de iteraciones que desea que el programa haga para");
		printf("\nlos calculos.");
		printf("\nIteraciones: ");
		scanf("%d",&ite);
	}else if(c!='n'&&c!=13) goto recdait;
	fflush(stdin);
}

void ingresarDatos()
{
	long double temp1, temp2, temp3, temp4, temp5;
	int entrada = 0;
	printf("\nIngreso de los valores l1, l2, l3 longitudes del brazo");
	l1=l2=l3=-5;
	 printf("\n\nIngrese el valor de:(Recuerde que l1,l2,l3 son mayores de 0)");
	 while(l1<=0||l2<=0||l3<=0)
	 {
		printf("\nl1: ");
		 scanf("%Lf",&l1);
		printf("l2: ");
		 scanf("%Lf",&l2);
		printf("l3: ");
		 scanf("%Lf",&l3);
	 }
	 printf("Ingreso del punto (x,y,z) a ser alcanzado por el brazo mecanico");
	 printf("\nIngrese el valor de:");
	 while(!entrada)
	 {
		printf("\nx: ");
		 scanf("%Lf",&x);
		printf("y: ");
		 scanf("%Lf",&y);
		printf("z: ");
		 scanf("%Lf",&z);
		if((z<=0 || x==0) || y==0)
		{
			clrscr();
			printf("\nNo ha introducido correctamente los datos, intente de nuevo");
			printf("\nIngrese el valor de:");
		}
		else
		{
			entrada=1;
		}
	 }

	 temp1 = ((l2*l2)+(2*l2*l3)+(l3*l3));
	 temp2 = ((l1*l1)-(2*l1*z)+(z*z))+x*x+y*y;
	 if(temp1<temp2)
	 {
		printf("\n** No se puede realizar los calculos **\nEl punto esta fuera del alcance del brazo");
		imposible=1;
	 }
	 else
	 {
		temp3=raiz((x*x+y*y));
		if(temp3==-1)
		{
			printf("\nNo se puede calcular la raiz de un numero negativo");
			imposible=1;
		}
		temp4=l2+(l3*cq3);
		temp5=1-(cq3*cq3);
		if(temp3==0||temp4==0)
		{
			printf("\nNo se puede calcular q2 por que no se puede dividir por cero (0)");
			imposible=1;
		}
		else
		{
			if(temp5<0)
			{
				printf("\nNo se puede calcular q2 por que no se puede sacar una raiz a un numero negativo");
				imposible=1;
			}
			else
			{
				d=(2*l2*l3);
				if(d==0)
				{
						printf("No se puede calcular q3  debido a una division por cero");
						imposible=1;
				}
				r=(x*x)+(y*y)+(z*z);
				l=(l2*l2)+(l3*l3);
				if(r==l)
				{
					printf("\nPunto invalido");
					imposible=1;
				}
			//	while((r==l) /*|| (r<(l-d))) || (r > (d+l))*/)
			/*	{
						printf("\nPunto invalido");
						 //	if(r==l)
							printf("\n x^2+y^2+z^2 debe ser diferente de l2^2+l3^2");
					   //		else
					   //			printf("\n se encontro una raiz negativa con esos puntos");
						printf("\nIngrese un nuevo x:");
								scanf("%Lf",&x);
						printf("\nIngrese un nuevo y:");
								scanf("%Lf",&y);
						printf("\nIngrese un nuevo z:");
								scanf("%Lf",&z);
						r=(x*x)+(y*y)+(z*z);
				}*/
			}
		}
	 }
	//z=l1-z;
}

long double tanInv(long double x)
{
	long double e,rpta,rptaN,xtem,nuevo,copia;
	int uno;
		int i=0;
	int n=1;
	int bandera=0;
		copia=x;
	e=tol+1;				//Valor inicial del error

	if(x>1)
		{
		x=1/x;
		bandera=1;
	}
	else
		{
		if(x<-1)
			{
			x=-1/x;
			bandera=1;
		}
		}
	rpta = x;

	while((e>=tol)&&(n<=ite))       	// ciclo
	{

		uno=1;
				xtem=x;
		for(i=0;i<n;i++)               	//calculo de (-1)^n
			uno=uno*(-1);
		for(i=1;i<(2*n+1);i++)         	//calculo de x^(2n+1)
					xtem=xtem*x;
		nuevo=(uno*xtem/(2*n+1));
		rptaN=rpta+nuevo;   		//nuevo elemento de la serie
		rpta=rptaN;                     //cambio de vbles para continuar el ciclo
		n=n+1;
	}
		if(bandera==1)
		rpta=(PI/2)-rpta;
	if(copia<-1)
		return (-rpta);
	else
			return rpta;
}

//calculo de q3
void calcularQ3()
{
	long double temp;
	long double x;
	cq3 = (r-l)/d;
	x= 1-(cq3*cq3);
    	x = raiz(x);			       //aqui va la raiz iterativa
	x = x/cq3;
	q3 = tanInv(x);
	if(bandera==-1)
		printf("\nNo se hallo un buen valor de q3 para tol:%.10Lf y %d iteraciones",tol,ite);
}

void calcularQ2()
{
	long double temp1, temp2, temp3;

	 temp1=raiz((x*x+y*y));
	 temp2=l2+(l3*cq3);
	 temp3=1-(cq3*cq3);
	 if(temp1==0||temp2==0)
		printf("\nNo se puede calcular q2 por que no se puede dividir por cero (0)");
	 else
	 {
		if(temp3>=0)
		{
		q2=(tanInv(z/temp1)-tanInv((l3*raiz(temp3))/temp2));
		}
        else
			printf("\nNo se puede calcular q2 por que no se puede sacar una raiz a un numero negativo");
	 }
}

void calcularQ1()
{
	long double m;
	m=y/x;
	q1=tanInv(m);
	if(x<0)
	q1=q1+PI;
}

void presentacion()
{
	clrscr();
	printf("\n----------------------------------------------------------");
	printf("\n-----------------------CINEMATIC--------------------------");
	printf("\n----------------------------------------------------------");
	printf("\n-------------  SIMULACION DE UN BRAZO MECANICO  ----------");
	printf("\n----------------------------------------------------------");
	printf("\n-------------------------  POR  --------------------------   #####");
	printf("\n----------------------------------------------------------  ##   ##");
	printf("\n---------------------------------------------------------- ##");
	printf("\n---------      Andres Yovanny Agudelo Toro      ---------- ## inematic");
	printf("\n---------      Diego Alejandro Alvarez Restrepo ----------  ##   ##");
	printf("\n---------      Jose Manuel Rodriguez Gallo      ----------   #####");
	printf("\n----------------------------------------------------------");
	printf("\n----------------------------------------------------------");
	printf("\n----------------------------------------------------------");
	printf("\n----------------------------------------------------------");
	printf("\n\n");
	printf("Presione una tecla para comenzar");
	getch();
        clrscr();
}


/*void main()
{
	int d;
		presentacion();
	ingresarDatos();
	while(imposible==1)
	{
			clrscr();
		imposible=0;
		ingresarDatos();
	}
	calcularQ1();
	calcularQ3();
	calcularQ2();
//Hasta aquí vienen los calculos de q1, q2 y q3 con los datos iniciales válidos
	printf("\n\nfinalizo Q1:%Lf Q2:%Lf Q3:%Lf",q1,q2,q3);
	getch();
}*/