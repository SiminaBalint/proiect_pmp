#include <DHT.h> // libraria pt senzorul de temperatura si umiditate
#include <Wire.h> // librarie standard
#include <LiquidCrystal_I2C.h> // libraria pt LCD
#include <Relay.h> // libraria pt releu
#include <AltSoftSerial.h> // libraria pt Bluetooth
#include <DS3231.h>

// initializare DS3231 
DS3231  rtc(SDA, SCL); 



#define DHTPIN 3 // definirea pinului pt senzorul de temperatura/umiditate
#define DHTTYPE DHT11 // definirea tipului de Senzor de temperatura

int relayPin=7; // PINUL la care releul este conectat
bool stareReleu = false; //starea initiala a releului este inchisa
float target_temp=19.00; // declararea unei temperaturi dorite de incepere
float t; // declararea temperaturii curente


AltSoftSerial altSerial; // definire pt Bluetooth
LiquidCrystal_I2C lcd(0x27,20,4);  // setarea adresei LCD-ului la 0x27 pentru 20 de caractere in 4 linii de display
DHT dht(DHTPIN, DHTTYPE); // definire pt senzor de temperatura

void setup()
{
  //conexiunea seriala pentru RTC
  Serial.begin(115200);
  rtc.begin();
  
  // urmatoarele linii au fost utilizate pentru initializarea RTC-ului, dar acum le putem lasa comentate deoarece ceasul a fost deja setat
  // rtc.setDOW(TUESDAY);     
  //rtc.setTime(22, 6, 0);    
  //rtc.setDate(22, 12, 2020);   
  
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

// functie pentru afisarea temperaturii curente
void afisareTemperatura_curenta()
{
  lcd.setCursor(2,1);
  t = dht.readTemperature();
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C ");
}

// afisarea temperaturii target
void afisareTemperatura_target()
{
  lcd.setCursor(2, 2);
  lcd.print(target_temp);
  lcd.print((char)223);
  lcd.print("C ");
}

// afisare stare releu, daca target_temp < t => termostat oprit, target_temp > t => termostat pornit
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
  //citirea din terminalul de pe telefon
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

  // afisare data si temperatura curenta
  lcd.setCursor(10,0);
  lcd.print("Data:");
  lcd.setCursor(10,1);
  lcd.print(rtc.getDOWStr());
  lcd.setCursor(10,2);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(10,3);
  lcd.print(rtc.getTimeStr());
  

}
