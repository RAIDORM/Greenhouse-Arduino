//Inclusion des bibliothèques
#include <DHT.h>
#include "DHT.h" // inclusion de la librairie DHT.h+
//bibliothèque de l'écran
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>


//définitions de valeurs
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

//Définition des couleurs

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

//Paramétrage mode de communication SPIts et tft

#define STMPE_CS 8
// ts=tout ce qui concerne le tactile
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define TFT_CS 10
#define TFT_DC 9
//tft=tout ce qui concerne le graphique
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int ActivationBouton = 20;
int ActivationBouton2 = 0;
int ActivationBouton3 = 0;
#define DHTPIN A3     // la sortie sur laquelle nous somme connecté
#define DHTTYPE DHT22     // la sortie sur laquelle nous somme connecté

DHT dht(DHTPIN, DHTTYPE);
int AnalgoHuTerre = A1;     // initialiser la variable analogHuTerre à la broche analogique 1
int ValeurHuTerre1 = 0;     //initialiser la variable ValeurHuTerre1 à 0
int ValeurHuTerre2 = 0;     // initialiser la variable ValeurHuTerre2 à 0

int Pompe = 2;
int brumisateur = 3;
int Ventilateur = 4;
int MoteurS1 = 5; // Ouverture
int MoteurS2 = 6; // Fermeture
int COuverture = 7; //capteur si le volet est ouvert
int CFermeture = 1;

int BrumiAll = 0;
int PompeAll = 0;
int VentilAll = 0;
int VoletAll = 0;

float h = 0;
float temp = 0;

long unsigned int mili = 0;
int millis_before = 0;
int intervale = 60000;

void setup()
{
  pinMode(brumisateur,OUTPUT);
  pinMode(Pompe,OUTPUT);
  pinMode(Ventilateur,OUTPUT);
  pinMode(MoteurS1,OUTPUT);
  pinMode(MoteurS2,OUTPUT);
  pinMode(COuverture,INPUT);
  pinMode(CFermeture,INPUT);
  Serial.begin(9600);// initialiser la communicaton avec le moniteur séria à 9600 bauds.
  dht.begin(); //met en marche le capteur.
  tft.begin(); //démarrer l'affichage écran
  ts.begin(); // démarrer l'écran tactile
  Ac(); //écran de démarrage
}

void loop()
{
//PARTIE WEB

mili = millis();
if (mili - millis_before >= intervale){
    millis_before = millis;
    Serial.print("BOT" + String(h,2) + "/" + String(temp,2) + "/" + String(ValeurHuTerre2) + "/" + String(PompeAll) + "/" + String(VoletAll) + "/" + String(BrumiAll) + "/" + String(VentilAll));
    // BOThygrométrie/température/humiditée/pompe/volet/brumisateur/ventilateur
}

//FIN PARTIE WEB

LecInfo();
Action();
Arret();
//PARTIE ECRAN TACTILE
  //Bouton INFO
  if(!ts.bufferEmpty())
  {
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  int x = p.x;
  int y = p.y;
  if (ActivationBouton == 20)
  {
    if (ts.touched())
  {
    if (x < 170 && x > 70)
    {
      if (y < 290 && y > 230)
      {
        Serial.println("On a appuyé sur le bouton info");
        ActivationBouton = 0;
        ActivationBouton2 = 20;
        Info();
      }
    }
  }
  }
  }
 


}





void Info2()
  {
  tft.fillScreen(WHITE);

//info sur la pompe
  tft.drawRect(10,10,115,50,BLACK);
  tft.setCursor(15,25);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("POMPE");

//info sur le brumisateur
  tft.drawRect(10,70,115,50,BLACK);
  tft.setCursor(15,85);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("BRUMIS.");

//info sur le ventilateur
  tft.drawRect(10,130,115,50,BLACK);
  tft.setCursor(15,145);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("VENTIL.");

//info sur le volet
  tft.drawRect(10,190,115,50,BLACK);
  tft.setCursor(15,205);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("VOLET");

  //touche retour
tft.fillRect(70,280,115,50,MAGENTA);
  tft.drawRect(70,280,115,50,BLACK);
  tft.setCursor(100,290);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("RETOUR");
  
while(ActivationBouton2 == 0)
{
  LecInfo();
  Action();
  Arret();
//case 2 pompe
  if (PompeAll == 1)
    {
    tft.fillRect(130,10,90,50,GREEN);
    tft.drawRect(130,10,90,50,BLACK);
    tft.setCursor(135,25);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("ON");
    }
  if (PompeAll == 0)
    {
    tft.fillRect(130,10,90,50,RED);
    tft.drawRect(130,10,90,50,BLACK);
    tft.setCursor(135,25);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("OFF");
    }
    
//case 2 brumisateur
  if (BrumiAll == 1)
    {
    tft.fillRect(130,70,90,50,GREEN);
    tft.drawRect(130,70,90,50,BLACK);
    tft.setCursor(135,85);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("ON");
    }
  if (BrumiAll == 0)
    {
    tft.fillRect(130,70,90,50,RED);
    tft.drawRect(130,70,90,50,BLACK);
    tft.setCursor(135,85);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("OFF");
    }
//case 2 ventilateur
  if (VentilAll == 1)
    {
    tft.fillRect(130,130,90,50,GREEN);
    tft.drawRect(130,130,90,50,BLACK);
    tft.setCursor(135,145);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("ON");
}
if (VentilAll == 0)
  {
    tft.fillRect(130,130,90,50,RED);
    tft.drawRect(130,130,90,50,BLACK);
    tft.setCursor(135,145);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("OFF");
  }
   
//case 2 volet
if (VoletAll == 1)
  {
  tft.fillRect(130,190,90,50,GREEN);
  tft.drawRect(130,190,90,50,BLACK);
   tft.setCursor(135,205);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Ouvert");
  }
if ( VoletAll == 0);
  {
  tft.fillRect(130,190,90,50,RED);
  tft.drawRect(130,190,90,50,BLACK);
   tft.setCursor(135,205);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Ferme");
  }

//A t'on appuyé sur le bouton retour ?
if (ActivationBouton2 == 0 )
  {
    TS_Point z = ts.getPoint();
  z.x = map(z.x, TS_MINX, TS_MAXX, 0, tft.width());
  z.y = map(z.y, TS_MINY, TS_MAXY, 0, tft.height());
  int c = z.x;
  int d = z.y;
    if (ts.touched())
    {
    if (c > 50 && c < 250)
    {
      if (d > 220 && d < 320)
      {
        ActivationBouton = 00;
        ActivationBouton2 = 20;
        Info();
      }
    }
    }
    }
}
}






 
void Ac()
{
  //Mettre le fond en blanc
 tft.fillScreen(WHITE);

   //1ère écran
  //affichage bienvenue
tft.fillRect(5,70,228,60,BLACK);
tft.setCursor(14,90);
tft.setTextColor(WHITE,BLACK);
tft.setTextSize(4);
tft.print("BIENVENUE");

//affichage suivant
tft.setCursor(50,170);
tft.setTextColor(BLACK,WHITE);
tft.setTextSize(2);
tft.print("Maintenir INFO   pour continuer");

//affichage bouton INFO
tft.fillRect(70,230,100,60,RED);
tft.drawRect(70,230,100,60,BLACK);
tft.setCursor(85,250);
tft.setTextColor(WHITE);
tft.setTextSize(3);
tft.print("INFO");
}









void Info()
{
  tft.fillScreen(WHITE);
  tft.setCursor(10,10);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.print(" Type  Info  ");
  
  // Affichage humiditée terre 1ère case
  tft.fillRect(10,70,115,50,GREEN);
  tft.drawRect(10,70,115,50,BLACK);
  tft.setCursor(15,85);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Hum.Terre");
  //2ème case
  tft.drawRect(130,70,90,50,BLACK);

  //affichage humiditée air
  //1ère case
  tft.fillRect(10,140,115,50,BLUE);
  tft.drawRect(10,140,115,50,BLACK);
  tft.setCursor(15,155);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Hum.Air");
  //2ème case
  tft.drawRect(130,140,90,50,BLACK);

  //affichage température
  //1ère case
  tft.fillRect(10,210,115,50,RED);
  tft.drawRect(10,210,115,50,BLACK);
  tft.setCursor(15,225);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Tempera");
  //2ème case
  tft.drawRect(130,210,90,50,BLACK);
  Serial.println("j'ai affiché les info");

  tft.fillRect(70,280,115,50,MAGENTA);
  tft.drawRect(70,280,115,50,BLACK);
  tft.setCursor(100,290);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("SUITE");
 
 
  while(ActivationBouton == 0)
  {
  LecInfo();
  Action();
  Arret();
  //affichage valeur humTerre
  tft.fillRect(130,70,90,50,WHITE);
  tft.drawRect(130,70,90,50,BLACK);
  tft.setCursor(160,90);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(ValeurHuTerre2);

  //Affichage valeur température
  tft.fillRect(130,210,90,50,WHITE);
  tft.drawRect(130,210,90,50,BLACK);
  tft.setCursor(150,230);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(dht.readTemperature());

  //affichage valeur HumAir
  tft.fillRect(130,140,90,50,WHITE);
  tft.drawRect(130,140,90,50,BLACK);
  tft.setCursor(150,160);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(dht.readHumidity());
  
   if (ActivationBouton2 == 20)
  {
    TS_Point f = ts.getPoint();
  f.x = map(f.x, TS_MINX, TS_MAXX, 0, tft.width());
  f.y = map(f.y, TS_MINY, TS_MAXY, 0, tft.height());
  int a = f.x;
  int b = f.y;
    if (ts.touched())
  {
    if (a > 50 && a < 250)
    {
      if (b > 220 && b < 320)
      {
        ActivationBouton = 20;
        ActivationBouton2 = 0;
        Info2();
        Serial.println("///////////////On passe à l'écran 2\\\\\\\\\\\\\\\\\\\\");
      }
    }
  }
  }
    
  }
}










void LecInfo()
{
  //Partie Humiditée terre
  ValeurHuTerre1 = analogRead(AnalgoHuTerre);
  ValeurHuTerre2 = map(ValeurHuTerre1,000,900,0,100);
  //affichage sur le moniteur série valeur du capteur
  Serial.print("valeur avant map humTerre = " );
  Serial.println(ValeurHuTerre1);
  //affichage sur le moniteur série Humiditée de la terre en %
  Serial.print("Humiditée Terre = " );
  Serial.print(ValeurHuTerre2);
  Serial.println("%");



//partie température
  temp = dht.readTemperature();
  if (isnan(temp)) //vérifie si  le contenu lu dans la variable float n'est pas un nombre.
  {
      Serial.println("échec de lecture du DHT");//écrire "échec de lecture du DHT" si le contenu lu n'est pas un nombre.
  }
  else // si le contenu lu est un nombre
  {
      Serial.print("Température:");
      Serial.print(temp);
      Serial.println("°C");
     
  }



//Partie hygrométrie
  h = dht.readHumidity();
  if (isnan(h)) // vérification du retour, si il n'est pas un nombre, c'est qu'il y a une erreur
  {
      Serial.println("Echec de la lecture depuis le DHT"); // écrire sur le moniteur série "Fialed to read from DHT"
  }
  else
  {
      Serial.print("Humidité de l'air: ");
      Serial.print(h);
      Serial.println(" %\t");
      // écrire au final sur le moniteur série grâce aux précedentes commandes "Humidité: (h) %\t"

  }
}





void Action()
{
  VoletAll = 0
 
  //partie Humidité sol
   if (ValeurHuTerre2 < 65 )
    {      
      digitalWrite (Pompe,HIGH);
      PompeAll = 1;
    }

   
  //partie Hygrométrie
  if (h < 60)
      {
      digitalWrite(brumisateur,HIGH);
      Serial.println("   le   BRUMISATEUR EST     ALLUME");
      BrumiAll = 1;
      }


   //partie Température
   if ((temp)>20)
{
  digitalWrite (Ventilateur,HIGH);
  digitalWrite (MoteurS1,HIGH);
  digitalWrite (MoteurS2,LOW);
  VentilAll = 1;
}
   if ((temp)<20)
  {
  digitalWrite (Ventilateur,LOW);
  digitalWrite (MoteurS1,LOW);
  digitalWrite (MoteurS2,HIGH);
  VentilAll = 0;
  }

  //Partie Volet
  
}


void Arret()
{
  
   if (ValeurHuTerre2 > 65 )
    {      
      digitalWrite (Pompe,LOW);
      PompeAll = 0;
    }
    
  if (h > 60)
      {
      digitalWrite(brumisateur,LOW);
      Serial.println("  on ETEINT         LE    RBUMISATEUR");
      BrumiAll = 0;
      }
      
  if ((temp)>20)
  {
  int Ouverture = digitalRead(COuverture);
  if (Ouverture == 1)
  {
  digitalWrite (MoteurS1,LOW);
  VoletAll = 0;
  }
  }
  
  if ((temp)<20)
    {
    int Fermeture = digitalRead(CFermeture);
    if (Fermeture == 1)
    {
      digitalWrite (MoteurS2,LOW);
      VoletAll = 1;
      }
    }
}