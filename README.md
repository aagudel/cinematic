# CINEMATIC
A robot arm simulator that solves the inkerse kinematic problem for a 3dof robot arm on DOS

Note: This was a school project written in October 2001 and only works for the old DOS over Win95/98. It used 3D graphics.

# INTRODUCCIÓN

Cinematic es un simulador del movimiento de un brazo robot, el programa resuelve el problema cinemático inverso para un robot con tres grados de libertad, es decir que calcula y simula los movimientos que debe realizar para situar el extremo superior del brazo en un punto en el espacio que requiera el operador, si es posible.

El programa toma la configuración del brazo que se requiera, ésta consiste en las longitudes que deben tener las extremidades. Luego el robot se ubica en una posición inicial equivalente a cero y se empieza a simular el recorrido.

Cinematic fue desarrollado en el lenguaje c para el entorno DOS, y presenta una simple interfaz que facilita su manejo. Así permite observar el movimiento que realizará el brazo robot en un pequeño espacio tridimensional gráficamente.

En su elaboración se requirió análisis numérico para realizar todo el proceso de cálculo de la nueva posición por medio de las ecuaciones necesarias. 

Cinematic se encuentra en una versión muy básica y cualquier persona es libre de utilizarlo y modificarlo, por tanto proveemos los códigos fuente. Es  posible también que tenga errores, por eso invitamos a las personas a que lo corrijan y lo mejoren.

# ESPECIFICACIONES

Para usar Cinematic se requiere cómo mínimo:
- Procesador 486 
- Tarjeta de video que soporte svga (casi todas) 
- Windows 95/98 o DOS
- 200Kb de espacio en disco
- 8 Mb de RAM

# INSTALACIÓN

El programa requiere que los siguientes archivos sean copiados en una misma carpeta, ésta puede crearse en el directorio raíz con el nombre cmat.

- cmat.exe: El ejecutable del programa
- svga256.bgi: Driver para el manejo del video
- Natural.fn: Fuente de caracteres para el modo gráfico
- Cinematic.doc	Este documento
Opcional: 	(Códigos fuente del programa)
- cin.c: 	Módulo principal
- halla.c: 	Módulo de cálculos numéricos
- orange.c: Motor gráfico 3d
- interfaz.c: Módulo de la interfaz

Una vez estén copiados los archivos no opcionales el programa debe, al menos ejecutar. Los códigos fuente fueron escritos en c y se compilaron en Borland C++ activando la librería BGI.  

Puede ser necesario (si se trata de Windows 95 o en algunos casos de Windows 98) configurar el programa para que se ejecute en pantalla completa,  pues entra en conflicto con el  modo de video de Windows. Esto se hace así:

1. Clic con el botón derecho de mouse en el icono del ejecutable cmat.exe.

2. Seleccionar propiedades.

3. En la ventana que sale seleccionar la etiqueta pantalla.

4. En uso escoger pantalla completa y dar aceptar.

# INGRESO AL PROGRAMA

El ingreso al programa puede hacerse desde DOS o Windows. Si es con DOS se ingresa normalmente por la línea de comandos. Cinematic no recibe parámetros de la línea de comandos, por lo que puede ingresarse con solo el ejecutable. Si por ejemplo se encuentra en la unidad C: en la carpeta cmat el ingreso se haría:

C:\CMAT\cmat.exe

Para entrar desde Windows se ejecuta normalmente. Puede ser necesario (si se trata de Windows 95 o en algunos casos de Windows 98) configurar el programa para que se ejecute en pantalla completa (véase Instalación.)

Si todo sale bien observará una pantalla de texto con el título de Cinematic y los autores, presione una tecla para empezar, en ese momento se le se le darán las especificaciones y se le pedirán los datos necesarios para realizar la simulación.

# MANEJO DEL PROGRAMA

## Ingreso de Parámetros

El ingreso de parámetros se hace por medio de seis o ocho preguntas en las que se pide la configuración del robot. El programa pregunta primero si desea modificar la tolerancia y el número de iteraciones para el cálculo que maneja el programa por defecto. En esta parte debe responder con la tecla ‘s’ para cambiarlo o ‘n’ o ‘Enter’ para no modificar esos valores. Si escoge cambiarlos el programa le pedirá nuevos valores. Es recomendado que la tolerancia sea aun número pequeño para mayor precisión en los cálculos. El número de iteraciones debería ser grande pero no tampoco excederse. Valores entre 10 y 100 serían adecuados.    

El programa le preguntará luego los 3 parámetros para la construcción del brazo robot. El valor l1 es la longitud de la base rotatoria del brazo, se aconseja que esta sea mayor a las demás longitudes. Además no puede ser negativa ni cero por que no podría construirse el robot. Para ingresar el valor digítelo y presione 'Enter'.
Igualmente se le preguntara por los valores de l2 y l3 que se ingresan de la misma manera y teniendo en cuenta las mismas especificaciones.  El valor l2 es la longitud de la extremidad ensamblada a la base y l3 la extremidad superior.

El siguiente paso es la coordenada en el espacio de donde se ubicará el robot. Deben ser valores consistentes a la configuración previamente definida, en otro caso el programa expresará error y dará oportunidad para nuevos valores. La coordenada en z del espacio debe ser diferente y mayor que cero pues en esos casos se trataría de posiciones bajo tierra. Las coordenadas x y y no podrán ser cero al mismo tiempo pues se trata de un lugar que el robot ya ocupa.

Ingresados los datos el programa hará los cálculos necesarios para comprobar si el punto es alcanzable por el extremo del robot, de otra forma habrá un mensaje de error y será necesario reingresar los datos.

## Errores

Es normal que durante el proceso de ingreso de parámetros y cálculo  de la nueva posición se encuentren errores, ya sea por valores ingresados inválidos o inconsistentes, cálculos imposibles o  posiciones inalcanzables. Por eso presentamos la lista de los posibles errores durante la etapa de ingreso de datos con su respectiva explicación:
-“No se puede calcular la raíz pues no se puede dividir por cero”

Quiere decir que durante el cálculo iterativo de la raíz la expresión se redujo a cero haciendo imposible el cálculo del siguiente valor.

-“No se encontró el resultado de la raíz para X iteraciones"

Este mensaje se presenta cuando no se logra alcanzar la tolerancia de error en el cálculo iterativo de la raíz.

-"No se puede sacar la raíz de un numero negativo"

Cuando se le pasa un número negativo como parámetro a la función de sacar raíz.

-"No ha introducido correctamente los datos, intente de nuevo"

Se pusieron valores erróneos para la posición en el espacio (véase ingreso de datos.) 

-“No se puede realizar los cálculos. El punto esta fuera del alcance del brazo”

Cuando por cálculos se descubre que físicamente el robot no puede alcanzar esa posición en el espacio por limitaciones en sus extremidades.

-“No se puede calcular qX por que no se puede dividir por cero”  X=1,2,3

Los valores qX son los ángulos que debe seguir el robot para posicionarse, estos se encuentran por cálculos trigonométricos que incluyen divisiones, si en algún caso se llega a una división por cero se presentara este error.

"No se puede calcular qX por que no se puede sacar una raíz a un numero negativo"

Se llegó  a un numero negativo dentro de una raíz en el cálculo.

"No se hallo un buen valor de qX para tol: t y n iteraciones"

El número de iteraciones no fue suficiente para encontrar una cifra adecuada para alguno de los ángulos q. Intente ingresando de nuevo al programa y cambiando los valores de iteraciones y tolerancia.

## Descripción del Funcionamiento

Si los datos de entrada son correctos el programa calcula los ángulos que deberá recorrer el robot para posicionarse en el lugar buscado. Esos ángulos se indican como q1, q2 y q3. El ángulo q1 es la rotación de la base alrededor de su centro, este será el primer movimiento que realizará el robot.

El ángulo q2 es la inclinación que deberá lograr el segundo extremo. Este rota en la cabeza de la base de arriba abajo. Para q3 la base será el anterior extremo, así que cuando ese termine de moverse realizará su propio movimiento y localizará el extremo en el punto solicitado.

 
## La Interfaz Gráfica

Espacio 
El espacio es el lugar en que se hace la simulación. Es una representación tridimensional donde la altura es el eje Z y la profundidad el eje Y (Puede observarse en las flechas al extremo inferior izquierdo.)La perspectiva de esta gráfica puede confundir al ojo, por eso puede rotarse por medio del teclado numérico (tecla BloqNum activada.)

Menú
Esta barra en el extremo superior izquierdo presenta las opciones de salir o observar el modelo, ésta última se activa automáticamente. Para salir se pueden utilizar las flechas del teclado ubicándose en ‘salir’ y presionar ‘Enter’.

Barra valores
Presenta los valores en grados que se encontraron para q1, q2 y q3, la posición que se buscaba y la configuración del robot. Esta barra es útil para comprobar y obtener los valores resultantes.


## Uso del Teclado

Para desplazarse por el menú: Flechas, Enter
Para observar el robot desde distintos puntos: Teclas numéricas 4, 6, 8, 2 (BlocNum activado)
Para salir 	ESC

Copyright 2001 Andres Agudelo Toro, Diego Alvarez Restrepo, Jose Rodriguez Gallo
