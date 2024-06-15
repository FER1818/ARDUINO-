/* 
Objetivo:
Desarrollar un programa de monitoreo de temperatura y humedad que detecte y notifique mediante alarmas visuales y auditivas
cualquier variación fuera de los rangos establecidos, garantizando así un ambiente controlado y seguro.
Este sistema utilizará un DISPLAY OLED para visualizar en tiempo real los valores de temperatura y humedad, 
un buzzer para alertas sonoras, un LED rojo para indicar condiciones anormales y un LED verde para señalar condiciones normales.
//**********************************************************************************************************************************
 ELABORADO POR : ING. FERMIN FLORES 
 FECHA: 8 SEP 2022.
//***************************************
Conexión del Dispaly:
//***************************************
 EL DISPLAY OLED SSD1306 DE 128 X 32 pixeles
 
 CONEXIÓN:

 VCC - 5V  //  GND - GND // A5 - SCK // A4 -- SDA

//****************************************************************************************************
//                  PROGRMA FUNCIONANDO CORRECTAMENTE (9 SEP 2022 )
//****************************************************************************************************

// ***************************************************************************************************
// ******************************  DATO TÉCNICO *****************************************************
//****************************************************************************************************
//Los estándares de referencia
//La Sociedad Estadounidense de Ingenieros de Calefacción, Refrigeración y Aire Acondicionado (ASHRAE)
//es el organismo que regula los estándares aceptados para los rangos de temperatura y humedad del aire 
en los entornos de centros de datos.
//En el 2011, el Comité Técnico 9.9 de ASHRAE determinó que un centro de datos clase A1
 deben mantenerse a una temperatura de 59 °F a 73.4 °F, ( 15°C A 23 °C)
//con una humedad relativa de un 20% y un 80%. 
ASHRAE recomienda que las instalaciones no superen estos estándares.
//******************************************************************************************************
*/

//**************************************************************************
//************************** LIBRERIAS *************************************
// INCLUIMOS LA LIBRERIA DE DISPLAY OLED SSD1306 I2c de 128 x 32 pixeles
//**************************************************************************
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128                  // Definimos el tamaño del display OLED - largo -
#define SCREEN_HEIGHT 32                  // Definimos el tamaño del display OLED - ancho-

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // Este constructor es para controlar y definir el display OLED, EL -1 estamos difiniendo que NO hay un pin de reset para el display

//INCLUIMOS LA LIBRERIA DHT SENSOR DE TEMPERATURA Y HUMEDAD.

#include <DHT.h>           // Libreria del sensor - la cual tiene que estar instalada en IDE de arduino -
#define DHTPIN 2           // Definimos a DTH -SEÑAL - VA  al Pin  2 del Arduino

#define DHTTYPE DHT11      // MEDELO DEL SENSOR, EN ESTE CASO ES DHT11 YA QUE EXSITE EL DTH22 (SIENDO MAS PRECISO y MAYOR RANGO DE OPERACIÓN)

DHT dht(DHTPIN, DHTTYPE);   // Asignamos el Objeto dht con los parámetros anteriores

//*************************************************
//***************** DECLARACIÓN DE LAS GPIO *******
//*************************************************

const int ledred=4;             // led rojo de  la alarma en el pin 4

const int buzzer = 6 ;          // buzzer de la alarma en el pin 6

const int ledgreen  = 8;        // led verde de en el pin 8

//************************************************************
//                CONFIGURACIÓN DEL GPIO
//************************************************************
void setup() {
 // INICIAMOS LA VELOCIDAD DEL DISPLAY

 Serial.begin(9600);
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  // Con esta función inicializamos la alimentación y asignamos con el 0x3c la dirección I2C del display
 {
Serial.println(F("SSD1306 allocation failed")); // Si la inicialización del display falla, imprime un mensaje de error "SSD1306 allocation failed"
for(;;);                                        // con este for, si la inicialización del display falla,este bucle infinito deteniene la ejecución del programa, para el programa

//  CONFIGURAMOS LOS ACTUADORES COMO SALIDAS
  pinMode(4,OUTPUT);                            // ledred como salida
  pinMode(6,OUTPUT);                           // el buzzer como salida
  pinMode(8,OUTPUT);                           // ledgreen como salida

 // INICIAMOS EL SENSOR DE TEMPERATURA & HUMEDAD 
  dht.begin();
   }
}

//***************************************************************
//               PROGRAMA PRINCIPAL
//****************************************************************

void loop() {

display.clearDisplay();                   // LIMPIAMOS EL DISPLAY
display.setTextSize(1);                  // SELECCIONAMOS EL TAMAÑO DEL TEXTO
display.setTextColor(WHITE);             // SELECCIONAMOS EL COLOR DEL TEXTO, SOLO ES WHITE, DEBIDO A QUE ES MONOCROMÁTICO
display.setCursor(0, 0);                 // TEXTO INICIE EN LAS COORDENDAS 0,0 DEL DISPLAY
delay(1000);                             // ESPERAMOS UN MINUTO



  // SE LEÉ LA HUMENDAD Y SE ALMACENA EN LA VARIABLE FLOAT "h"

float h = dht.readHumidity();
  
  // SE LE LA TEMPERATURA Y SE ALMACENA EN LA VARIABLE FLOAT "t"

float t = dht.readTemperature();
  

  if (isnan(h) || isnan(t) )  {                             // VERIFICAMOS SI LA TEMPERATURA Y LA HUMEDAD SON DATOS O NUMEROS VÁLIDOS SINO, MARCAR ERROR EN DISPLAY
    display.print("Error en al lectura del sensor DHTxx");
    return;
  }
 
 // MOSTRAMOS LOS VALORES CORRESPONDIENTES A LA HUMEDAD Y TEMPERATURA
  
  Serial.print("Temperatura: ");           // Imprimimos por el monitor serial el texto "Temperatura"
  Serial.print(t);                         // Imprimimos por monitor serial el valor de la temperatura (t)
  Serial.println("°C");                    // Agreagamos el simbolo de grados (°), asi como la letra "C"
 
// VISUALIZAMOS EL DATO TEMPERATURA "t" EN EL DISPLAY

  display.print("Temperatura: ");          // Imprimimos en el display OLED la palabra "Temperatura" 
  display.print(t);                        // Obtenemos el valor de la variable "t" del sensor
  display.print(char(247));                // Con el caracter 247 - char 247 - en ASCII corresponde al simbolo de grado (°)                 
  display.println("C");                    // Mostramos la letra en la pantalla OLED "C"

  display.startscrollleft(0x00,0x07);     // DESPLAZAMOS HORIZONTALMENTE EL TEXTO HACIA LA IZQUIERDA EN TODA LA PANTALLA POR ESO 0X00 Y 0X07
  delay(8000);                            // Detemos la información que esta en la pantalla durante 8 segundos 
  display.stopscroll();                   

  display.setCursor(0, 12);                       // Con las coordenadas 0,12 colocamos abajo del texto temperatura la medición de la humedad.
  display.print("Humedad:  ");                   // Se visualiza la medición "humedad" en la pantalla 
  display.print(h);                             // Mostramos el valor de la variable "h", el pantalla OLED
  display.println(" %");                       // Mostramos el porcentaje en la pantalla OLED "%"

  display.setCursor(0, 23);                   // Con estas coordenadas colocamos el texto "Ing. Fermin Flores B."
  display.println(" Ing. FERMIN FLORES B.");  // DENTRO DE LAS COMILLAS PUEDES COLOCAR TU NOMBRE, EL NOMBRE DE LA EMPRESA,ETC. SOLO TENER CUIDADO DE NO EXCEDER EL NUMERO DE LETRAS
  display.display();                          // TENER EN CUENTA QUE EL TAMAÑO DEL OLED ES PEQUEÑO 128 X 32 PIXELES.
  delay(5000);


//*********************************************************************************************
//*************          INICIO DE CONTROL CON LA CONIDICION IF     **************************
//**********************************************************************************************
  
  
  if (t>=30.0)                // - set -  FIJAMOS  LA TEMPERATURA o LA REFERENCIA DE TEMPERATURA MAXIMA PERMITIDA - umbral -
  {
  
  display.clearDisplay();     // Limpiamos el contenido de el display para poder visualizar la nueva información, es decir, aseguramos que no quede ningún texto o gráficos.
  display.display();          // Actualizamos el display o nos aseguramos que esta limpia la pantalla
  delay(300);                 // Esperamos 300 milisengundos para evitar que la pantalla parpade demasiado rápido


  display.setTextSize(3);       // Establecemos el tamaño del texto grande , para visualizar fácilmente la palabra "ALARMA"
  display.setTextColor(WHITE);  // color del texto, ojo, solo existe este color en este tipo de OLED, por ser monocromático.
  display.setCursor(2, 12);     // Establecemos las coordenadas de donde queremos que empiece el texto ALARMA""
  display.println("ALARMA");     // Se muestra la palabra en la pantalla OLED "ALARMA"
  display.display();                    

   analogWrite(buzzer,50);       // Activamos la alarma AUDITIVA con el buzzer con un ciclo de trabajo del 20% ( 50 sobre 255 bits del PWM)
   
  //*************************************************************************************************************
  //******************************** SECCIÓN DEL PROGRAMA DE ALERTA *********************************************  
  //*************************************************************************************************************
  //***********************INICIA PARPADEO DE LUZ ROJA + BUZZER (AUDIO) ******************************************

  
  digitalWrite(ledred,HIGH);    // Encendemos el led rojo
  digitalWrite(ledgreen,LOW);   // Apagamos el led verde
  digitalWrite(ledred,LOW);     // Apagar el led rojo
  delay (500);                  // Lo dejamos apagado 500 milisegundos
  digitalWrite(ledred,HIGH);   // Volvemos a encender el led rojo inicializando una vez más el ciclo
  delay (500);
  digitalWrite(ledred,LOW);
  delay (500);
  digitalWrite(ledred,HIGH);
  delay (250);                // Aceleramos el encendido y apagado del led para llamar más atención visual
  digitalWrite(ledred,LOW);
  delay (250);
  digitalWrite(ledred,HIGH);
  delay (250);
  digitalWrite(ledred,LOW);
  delay (250);
  digitalWrite(ledred,HIGH);
  delay (250);
  digitalWrite(ledred,LOW);
  digitalWrite(ledred,HIGH);
  
  
  }

//*************************************************************************************************
// SI LA TEMPERATUA ES MENOR A 30°C EL ESPACIO ESTA EN CONDICIONES NORMALES - dentro del Umbral -
//**************************************************************************************************
else  
{
   digitalWrite(ledred,LOW);   // led rojo permanece apagado
  
   digitalWrite(ledgreen,HIGH);  // led verde encendido 

   analogWrite(buzzer,0);        // buzzer sin tono

  display.clearDisplay();         // limpiamos el display,evitamos información almacenda
  display.display();
  delay(250);
  display.setTextSize(2);          // seleccionamos el tamaño del texto a "2"
  display.setTextColor(WHITE);     // encendemos los pixeles - por ser monocromático solo escribe White -
  display.setCursor(0, 0);
  display.print("SERVIDORES");    // VISUALIZAMOS EN EL DISPLAY "SEVIDORES" con las coordenadas 0,0
  display.setCursor(40, 20);      // Con estas coordenadas colocamos el texto "MONITOREO"
  display.setTextSize(1);         // con este tamaño de letra ( 1 ) para que entre en el display 
  display.print("MONITOREO");
 display.display();
  delay(2000); 
  
}

  }

 //************************************************************************************************************
 //******************************* FIN DEL PROGRAMA ***********************************************************
 //************************************************************************************************************
 
