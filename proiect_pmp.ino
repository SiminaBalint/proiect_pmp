#include <DHT.h> // libraria pt senzorul de temperatura si umiditate
#include <Wire.h> // librarie standard
#include <LiquidCrystal_I2C.h> // libraria pt LCD
#include <Relay.h> // libraria pt releu
#include <AltSoftSerial.h> // libraria pt Bluetooth



#define DHTPIN 3 // definirea pinului pt senzorul de temperatura/umiditate
#define DHTTYPE DHT11 // definirea tipului de Senzor de temperatura

int relayPin=7; // PINUL la care releul este conectat
bool stareReleu = false; //starea initiala a releului este inchisa
float target_temp=19.00; // declararea unei temperaturi dorite de incepere
float t; // declararea temperaturii curente


AltSoftSerial altSerial; // definire pt Bluetooth
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE); // definire pt senzor de temperatura

void setup()
{
  // setup senzor
  dht.begin();
  
  // setup display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  
  // setup releu
  pinMode(relayPin, OUTPUT);

  // setup Bluetooth
  altSerial.begin(9600);

}

void afisareTemperatura_curenta()
{
  lcd.setCursor(2,1);
  t = dht.readTemperature();
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C ");
}

void afisareTemperatura_target()
{
  lcd.setCursor(2, 2);
  lcd.print(target_temp);
  lcd.print((char)223);
  lcd.print("C ");
}
void afisareReleu()  
{
  lcd.setCursor(2,3);
  if(target_temp>t)
  {
    digitalWrite(relayPin, LOW);
    lcd.print("PORNIT");
    stareReleu=true;
  }
  else
  {
    digitalWrite(relayPin, HIGH);
    lcd.print("OPRIT ");
    stareReleu=false;
  }
}



void loop()
{
 char c;
 if (altSerial.available()) {
   c = altSerial.read();
   Serial.print(c);

   if(c=='1'){
      target_temp+=0.25;
    }
    if(c=='0'){
      target_temp+=-0.25;
    }
  }
  
  afisareTemperatura_curenta();
  afisareTemperatura_target();
  afisareReleu();

}
