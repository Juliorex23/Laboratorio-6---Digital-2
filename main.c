#include <Arduino.h>         
#include <LiquidCrystal.h>

//Pines para la LCD
#define RS 33
#define EN 32
#define D4 12
#define D5 13
#define D6 23
#define D7 22


//Pines para el potenciómetro
#define pot1 34
#define pot2 35

//Pines de botones 

#define BTN1 18
#define BTN2 19

//Variables Globales
LiquidCrystal LCD(RS, EN, D4, D5, D6, D7);
uint8_t decenas1, unidades1, decimales1;
uint8_t decenas2, unidades2, decimales2;

float voltaje1;
float voltaje2;

//***********************************************************

volatile bool aumentar = false; // Bandera que indica si se debe aumentar el contador
volatile bool restar = false; // Bandera que indica si se debe disminuir el contador

// Funciones ISR para aumentar
void IRAM_ATTR onButton1Press() {
    aumentar = true;
}

// Funciones ISR para restar
void IRAM_ATTR onButton2Press() {
    restar = true;
}

//Función del botón 
void button (void);

int contador = 0;

void setup()
{
  Serial.begin(115200);
  LCD.begin(16,2);
  LCD.clear();

  LCD.print("Hello world!");
  delay(5000);
  LCD.noCursor();

  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);

  button();

}

//***********************************************************

void loop()
{
//Cálculos para el potenciómetro 1
  voltaje1 = analogReadMilliVolts(pot1)/10;

  int temp1 = voltaje1;                    //declaración de la variable temporal para operar para decenas, unidades y decimales

  decenas1 = temp1/100;                //cálculo de las decenas para el 1er potenciómetro

  temp1 = temp1 - decenas1 * 100.0;      //preparo temp1 para operar para las unidades

  unidades1 = temp1/10.0;                //cálculo de las unidades para el 1er potenciómetro

  temp1 = temp1 - unidades1 * 10.0;     //preparo temp1 para operar para los decimales

  decimales1 = temp1;                      //cálculo de decimales para el 1er potenciómetro


//Cálculos para el potenciómetro 2
  voltaje2 = analogReadMilliVolts(pot2)/10;
  int temp2 = voltaje2;                    //declaración de la variable temporal para operar para decenas, unidades y decimales

  decenas2 = temp2/100;              //cálculo de las decenas para el 2do potenciómetro

  temp2 = temp2 - decenas2 * 100;      //preparo temp1 para operar para las unidades

  unidades2 = temp2/10.0;               //cálculo de las unidades para el 2do potenciómetro

  temp2 = temp2 - unidades2 * 10.0;     //preparo temp1 para operar para los decimales

  decimales2 = temp2;                      //cálculo de los decimales para el 2do potenciómetro

//Contador 
if (aumentar) {
        aumentar = false;
      contador = contador + 1;
      if (contador > 255){
        contador = 0;
      }
}

if (restar) {
        restar = false;
      contador -= 1;
      if (contador < 0){
        contador = 255;
      }
}

//Comienzo a mostrar los valores obtenidos de voltaje en la LCD 

//potenciómetro 1
LCD.clear();
LCD.setCursor(0, 0);
LCD.print("Pot1: ");
LCD.setCursor(0, 1);
LCD.print(decenas1);
LCD.setCursor(1, 1);
LCD.print(".");
LCD.setCursor(2, 1);
LCD.print(unidades1);
LCD.setCursor(3, 1);
LCD.print(decimales1);
LCD.setCursor(4, 1);
LCD.print("v");

//potenciómetro 2
LCD.setCursor(6, 0);
LCD.print("POT2: ");
LCD.setCursor(6, 1);
LCD.print(decenas2);
LCD.setCursor(7, 1);
LCD.print(".");
LCD.setCursor(8, 1);
LCD.print(unidades2);
LCD.setCursor(9, 1);
LCD.print(decimales2);
LCD.setCursor(10, 1);
LCD.print("v");

//Contador
LCD.setCursor(12, 0);
LCD.print("CPU:");
LCD.setCursor(12, 1);
LCD.print(contador);
LCD.noCursor();
delay(3000);

}


//Función del botón
void button(void) {
  pinMode(BTN1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN2), onButton1Press, FALLING);
  pinMode(BTN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN1), onButton2Press, FALLING);
}
