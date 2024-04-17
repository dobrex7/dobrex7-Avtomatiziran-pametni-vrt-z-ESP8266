#include <LiquidCrystal_I2C.h> // library/knjiznica potrebna za I2C display
#include<SoftwareSerial.h> // knjiznica za serijsko komunikacijo (z esp8266)
#include <dht11.h> // knjiznica za temperaturo in vlago
#define DHT11PIN 8 // definiramo dht11 vhod na 8

dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 16, 2); 
SoftwareSerial Arduino_SoftSerial(10, 11); //10 povezano na D2, 11 povezano na D1
String dataIn;

int rele = A2; //definiramo rele pod A2 vhod
int rdeca = 5; //definiramo vhode led luck
int rumena = 6;
int zelena = 7;
int modra_levo = 4;
int modra_sredina = 3;
int modra_desno = 2;
int buzzer = 9; // definiramo vhod zvocnika

void setup() {
  Serial.begin(115200);
  Arduino_SoftSerial.begin(9600); // inicializiramo serijsko komunikacijo za posiljanje iz arduina na esp8266, na vrednosti 115200
  lcd.init();
  lcd.backlight();
  lcd.clear(); // ukaz ki pocisti zaslon
  pinMode (rele, OUTPUT);
  // inicializiramo outpute
  pinMode(rdeca, OUTPUT);
  pinMode(rumena, OUTPUT);
  pinMode(zelena, OUTPUT); 
  pinMode(modra_levo, OUTPUT);
  pinMode(modra_sredina, OUTPUT);
  pinMode(modra_desno, OUTPUT); 
  pinMode(buzzer,OUTPUT);
}

void loop() {
  digitalWrite(rele, HIGH); //izklopimo rele
  delay(0);
   int vrednostSenzorja = analogRead(A0); // vhod , v katerga je priključen senzor
   int procentVlage = map(vrednostSenzorja, 450, 0, 100, 0); //450 je max vrednost, ko je 100% in 0 min vrednost, ko je 0% 
   int chk = DHT11.read(DHT11PIN); // branje iz senzorja

     if(vrednostSenzorja>=450) //if pogoj, ki fiksira, vlažnost na 100%, če dobi vrednost nad 450, kar je avtomatsko max vlažnost

    {
      procentVlage = 100; //procent vlaznosti nastavimo na 100%
    }

    // Izpis vlage v % na LCD displayu
    Serial.print("Vlaga (%):");
    Serial.print((float)procentVlage, 2);
    lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
    lcd.print("Vlaga (%):");
    lcd.print((float)procentVlage, 2);

    Serial.println(" "); //prazna vrsta

    Serial.print("Temp (C): ");
    Serial.print((float)DHT11.temperature, 2); // izpis temperature
    lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
    lcd.print("Temp (C): ");
    lcd.print((float)DHT11.temperature, 2); // izpis temperature

    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Vlaga (%):");
  Arduino_SoftSerial.print((float)procentVlage, 2);
  Arduino_SoftSerial.print(',');

  Arduino_SoftSerial.print("Temp (C): ");
  Arduino_SoftSerial.print((float)DHT11.temperature, 2); // izpis temperature

      //animacija 3 lucke
      delay(1000);
      digitalWrite(rdeca, LOW);
      digitalWrite(rumena, LOW);
      digitalWrite(zelena, LOW);

        // rdeca lucka
      digitalWrite(rdeca, HIGH);   // vklopimo lucko
      digitalWrite(modra_desno, HIGH);
      delay(500);               // pocakamo pol sekunde
      digitalWrite(rdeca, LOW);    // ugasnemo lucko
      digitalWrite(modra_desno, LOW);
      delay(500);               // pocakamo pol sekunde

      // rumena lucka
      digitalWrite(rumena, HIGH);
      digitalWrite(modra_sredina, HIGH);
      delay(500);
      digitalWrite(rumena, LOW); 
      digitalWrite(modra_sredina, LOW);
      delay(500);

      // zelena lucka
      digitalWrite(zelena, HIGH);
      digitalWrite(modra_levo, HIGH);  
      delay(500);
      digitalWrite(zelena, LOW);
      digitalWrite(modra_levo, LOW);  
      delay(500);

      // vse lucke prizgemo 
      digitalWrite(rdeca, HIGH); 
      digitalWrite(rumena, HIGH);
      digitalWrite(zelena, HIGH);

   delay(5000); //delay za prikaz temperature in vlage v ozracju
   lcd.clear(); // ukaz ki pocisti zaslon
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Izpis vlaznosti v ozracju
    // Izpis vlage v enotah od 0 do 1018
    Serial.println(" "); //prazna vrsta
    Serial.print("Vlaga je ");
    Serial.print(analogRead(A0));
    Serial.println(" enot");
    Serial.println(" "); //prazna vrsta

    Serial.print("  Vlaznost v ");
    Serial.println(" "); //prazna vrsta
    Serial.print("ozracju: ");
    Serial.print((float)DHT11.humidity, 2); // izpis vlage
    Serial.print(" %");
    Serial.println(" "); //prazna vrsta

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print("   Vlaznost v ");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("ozracju: ");
      lcd.print((float)DHT11.humidity, 2); // izpis vlage
      lcd.print(" %");

delay(5000); //5 sekund delaya - izpisovanje vlage itd

          // vse lucke ugasnemo 
        digitalWrite(rdeca, LOW); 
        digitalWrite(rumena, LOW);
        digitalWrite(zelena, LOW);
        digitalWrite(modra_levo, LOW);   
        digitalWrite(modra_sredina, LOW);
        digitalWrite(modra_desno, LOW);
// ------------------------------------------------------------------------------------- DHT11 part - vlaznost v ozracju in temperatura pogoji 
// Izpis temperature, ko je temperatura nizka oz. ko je pod 18 stopinj
  if (DHT11.temperature<=18)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println("  Nizko stanje ");
    Serial.print("Temp je ");
    Serial.print((float)DHT11.temperature, 2);
    Serial.print(" C");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Nizko stanje temperature! "); //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print("  Nizko stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("Temp je ");
      lcd.print((float)DHT11.temperature, 2); // izpis temperature
      lcd.print(" C");

        digitalWrite(rdeca, LOW); // če je rdeca lučka ze od prej prizgana od stanja temperature, se ugasne za pol sekunde 
        delay(500);
        digitalWrite(rumena, LOW);
        digitalWrite(zelena, LOW);
        digitalWrite(rdeca, HIGH); // ko je temperatura pod 18 se rdeca prizge, rumena in zelena so ugasnjene

          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          ///////////////////// 2.krog kritičnega stanja
          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge, rumena in zelena se preventivno ugasnejo
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          //////////////// 3.krog kritičnega stanja
            digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge, rumena in zelena se preventivno ugasnejo
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

  delay(2000); 
  lcd.clear(); // ukaz ki pocisti zaslon
  }

// Izpis temperature, ko je temperatura normalna oz. ko je nad 18 stopinj in pod 30 stopinj
  if (DHT11.temperature>=19 & DHT11.temperature<=30)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println("Normalno stanje ");
    Serial.print("Temp je ");
    Serial.print((float)DHT11.temperature, 2);
    Serial.print(" C");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Normalno stanje temperature! "); //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print("Normalno stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("Temp je ");
      lcd.print((float)DHT11.temperature, 2); // izpis temperature
      lcd.print(" C");

        digitalWrite(zelena, LOW); // če je zelena lučka ze od prej prizgana od stanja temperature, se ugasne za pol sekunde 
        delay(500);
        digitalWrite(zelena, HIGH); // ko je temperatura nad 19 in pod 18 se zelena prizge, rdeca in rumena so ugasnjene
        digitalWrite(rdeca, LOW);
        digitalWrite(rumena, LOW);

  delay(5000); 
  lcd.clear(); // ukaz ki pocisti zaslon
  }
// Izpis temperature, ko je temperatura visoka oz. nad 31 stopinj
  if (DHT11.temperature>=31)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println(" Visoko stanje ");
    Serial.print("Temp je ");
    Serial.print((float)DHT11.temperature, 2);
    Serial.print(" C");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Visoko stanje temperature! ");  //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print(" Visoko stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("Temp je ");
      lcd.print((float)DHT11.temperature, 2); // izpis temperature
      lcd.print(" C");

        digitalWrite(rdeca, LOW); // če je rdeca lučka ze od prej prizgana od stanja temperature, se ugasne za pol sekunde 
        delay(500);
        digitalWrite(rumena, LOW);
        digitalWrite(zelena, LOW);
        digitalWrite(rdeca, HIGH); // ko je temperatura nad 31 se rdeca prizge, rumena in zelena so ugasnjene

          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          ///////////////////// 2.krog kritičnega stanja
          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge, rumena in zelena se preventivno ugasnejo
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          //////////////// 3.krog kritičnega stanja
            digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge, rumena in zelena se preventivno ugasnejo
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 1.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); // ko stanje vlage pade pod 20% se rdeca lucka prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja ------------- 2.krog kritičnega stanja buzzer in rdeca lucka
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

  delay(2000); 
  lcd.clear(); // ukaz ki pocisti zaslon
  }
// ------------------------------------------------------------------------------------- Soil Moisture part - vlaga pogoji + 3LED za stanje
// Izpis vlage, ko je stanje kriticno oz. ko je pod 20% vlaznosti
  if (procentVlage<=20)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println("Kriticno stanje");
    Serial.print("  Vlaga je ");
    Serial.print(procentVlage);
    Serial.println(" %");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Kriticno stanje vlage! "); //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print("Kriticno stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("  Vlaga je ");
      lcd.print(procentVlage);
      lcd.print(" %");

        digitalWrite(rdeca, LOW); // če je rdeca lučka ze od prej prizgana od stanja temperature, se ugasne za pol sekunde 
        delay(500);
        digitalWrite(rumena, LOW);
        digitalWrite(zelena, LOW);

          ///////////////////// 1.krog animacije rdeče lučke
          digitalWrite(rdeca, HIGH); // rdeca lucka se prizge
          tone(buzzer, 400); // frekvenca, ki jo zvocnik predvaja 
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW); // rdeca lucka se ugasne
          delay(100);

          digitalWrite(rdeca, HIGH); 
          tone(buzzer, 400); 
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); 
          tone(buzzer, 400); 
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          ///////////////////// 2.krog animacije rdeče lučke
          digitalWrite(rdeca, HIGH); 
          tone(buzzer, 400);
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH); 
          tone(buzzer, 400); 
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH);
          tone(buzzer, 400); 
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          delay(1000);
          //////////////// 3.krog animacije rdeče lučke
            digitalWrite(rdeca, HIGH);
          tone(buzzer, 400);
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH);
          tone(buzzer, 400);
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

          digitalWrite(rdeca, HIGH);
          tone(buzzer, 400);
          delay(400);
          noTone(buzzer);    // zvocnik se ugasne 
          digitalWrite(rdeca, LOW);
          delay(100);

// ---------------Začetek zalivanja ---------------------------------------------------
              digitalWrite(rele,LOW); //vklopimo rele za pol sekunde
              delay(500);
              Serial.println("Zalivanje...");
              Serial.println(" "); //prazna vrsta
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje");

                digitalWrite(modra_levo, HIGH);   
                digitalWrite(modra_sredina, LOW);
                digitalWrite(modra_desno, LOW);
                tone(buzzer, 700);

              delay(50);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje.");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("..");

                digitalWrite(modra_levo, HIGH);  
                digitalWrite(modra_sredina, HIGH);
                digitalWrite(modra_desno, LOW); 
                tone(buzzer, 750);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje..");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("....");

                digitalWrite(modra_levo, HIGH);   
                digitalWrite(modra_sredina, HIGH);
                digitalWrite(modra_desno, HIGH);
                tone(buzzer, 800);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje...");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("......");

                digitalWrite(modra_levo, LOW);  
                digitalWrite(modra_sredina, LOW);
                digitalWrite(modra_desno, LOW);
                tone(buzzer, 850);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje....");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("........");

                digitalWrite(modra_levo, HIGH);   
                digitalWrite(modra_sredina, LOW);
                digitalWrite(modra_desno, LOW);
                tone(buzzer, 700);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje....");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("..........");

                digitalWrite(modra_levo, HIGH);  
                digitalWrite(modra_sredina, HIGH);
                digitalWrite(modra_desno, LOW);
                tone(buzzer, 750);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje.....");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("............");

                digitalWrite(modra_levo, HIGH);   
                digitalWrite(modra_sredina, HIGH);
                digitalWrite(modra_desno, HIGH);
                tone(buzzer, 800);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje......");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("..............");

                digitalWrite(modra_levo, LOW);  
                digitalWrite(modra_sredina, LOW);
                digitalWrite(modra_desno, LOW);
                tone(buzzer, 850);

              delay(100);
              noTone(buzzer);
              lcd.clear(); // ukaz ki pocisti zaslon
              lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
              lcd.print("Zalivanje.......");
              lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni zgoraj
              lcd.print("................");
              digitalWrite(rele, HIGH); //izklopimo rele 
delay(0);
  }
  lcd.clear(); // ukaz ki pocisti zaslon

// Izpis vlage, ko je stanje srednje oz. ko je nad 21% in pod 40% vlaznosti
  if (procentVlage>=21 & procentVlage<=40)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println("Srednje stanje");
    Serial.print("  Vlaga je ");
    Serial.print(procentVlage);
    Serial.println(" %");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Srednje stanje vlage! "); //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print(" Srednje stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("  Vlaga je ");
      lcd.print(procentVlage);
      lcd.print(" %");

        digitalWrite(rumena, LOW); // če je rumena lučka ze od prej prizgana, se ugasne za pol sekunde 
        delay(500);

        digitalWrite(rumena, HIGH); // ko je stanje vlage med 21 in 40% se rumena lucka vklopi, rdeca in zelena se preventivno ugasnejo
        digitalWrite(rdeca, LOW);
        digitalWrite(zelena, LOW);

  delay(5000);
  lcd.clear(); // ukaz ki pocisti zaslon
  }

// Izpis vlage, ko je stanje visoko oz. ko je nad 41% vlaznosti
  if (procentVlage>=41)
  {
    lcd.clear(); // ukaz ki pocisti zaslon
    Serial.println(" "); //prazna vrsta
    Serial.println("Visoko stanje");
    Serial.print("  Vlaga je ");
    Serial.print(procentVlage);
    Serial.println(" %");
    Serial.println(" "); //prazna vrsta

  Arduino_SoftSerial.print("Visoko stanje vlage! "); //posiljanje podatka na esp8266

      lcd.setCursor(0,0); // 0 pomeni od leve proti desni  0 polj, 0 pomeni zgoraj
      lcd.print("  Visoko stanje");
      lcd.setCursor(0,1); // 0 pomeni od leve proti desni  0 polj, 1 pomeni spodaj
      lcd.print("  Vlaga je ");
      lcd.print(procentVlage);
      lcd.print(" %");

        digitalWrite(zelena, LOW); // če je zelena lučka ze od prej prizgana od stanja temperature, se ugasne za pol sekunde 
        delay(500);

        digitalWrite(zelena, HIGH); // ko je stanje vlage nad 41% se zelena lucka prizge, rumena in rdeca se preventivno ugasnejo
        digitalWrite(rumena, LOW);
        digitalWrite(rdeca, LOW);

  delay(5000);
  lcd.clear(); // ukaz ki pocisti zaslon
  }
}
