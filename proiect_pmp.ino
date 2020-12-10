#include <DHT.h> // libraria pt senzorul de temperatura si umiditate
#include <Wire.h> // librarie standard
#include <LiquidCrystal_I2C.h> // libraria pt LCD


#define DHTPIN 3 // definirea pinului pt senzorul de temperatura/umiditate
#define DHTTYPE DHT11 // definirea tipului de Senzor de temperatura

float t; // declararea temperaturii curente


LiquidCrystal_I2C lcd(0x27,20,4);  
DHT dht(DHTPIN, DHTTYPE); // definire pt senzor de temperatura

void setup()
{
  // setup senzor
  dht.begin();
  
  // setup display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("hello ");
  
}

void afisareTemperatura_curenta()
{
  lcd.setCursor(3,1);
  t = dht.readTemperature();
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C ");
}

 

void loop(){
  afisareTemperatura_curenta();
  }
