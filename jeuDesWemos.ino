#include <Bounce2.h>
#include <Chrono.h>
#include <LightChrono.h>


// INCLUDE ESP8266WiFi:
#include <ESP8266WiFi.h>
//infos reseaux
const char* ssid = "electroMIque";
const char* password = "emery330";
IPAddress ip(10, 5, 0, 133);
//10,5,0,133 CHLOE
//10,5,0,118 MAX
IPAddress recipientIp(10, 5, 0, 118);
IPAddress gateway(10, 5, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// INCLUDE ESP8266 UDP
#include <WiFiUdp.h>
WiFiUDP udp;

// INCLUDE MASSAGE PARSER
#include <AsciiMassageParser.h>
#include <AsciiMassagePacker.h>

// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

// WEMOS MATRIX SHIELD:
#include <Adafruit_GFX.h>
#include <WEMOS_Matrix_GFX.h>

//intensité pixels
MLED matrix(3.5); //set intensity=7 (maximum)

//btn pour générer le nombre de 1 à 6
#define BUTTON D3
Bounce debouncerBtn = Bounce();
bool btnOn = false;

//IMG WIN/LOSE ET DÉS**********************************************************
//Crochet
const unsigned char 
crochet[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000010,
  B01000100,
  B00101000,
  B00010000};

//** X
const unsigned char 
x[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00100100,
  B00011000,
  B00011000,
  B00100100
};

//**EGALE
const unsigned char 
egale[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
  B00111100,
  B00000000};

//FULL
const unsigned char 
full[] PROGMEM={
  B11100000,
  B11100000,
  B11100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};

//** UN
const unsigned char 
un[] PROGMEM={
  B00000000,
  B01000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};

//**DEUX
const unsigned char 
deux[] PROGMEM={
  B10000000,
  B00000000,
  B00100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};

//**TROIS
const unsigned char 
trois[] PROGMEM={
  B10000000,
  B01000000,
  B00100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};

//**QUATRE
const unsigned char 
quatre[] PROGMEM={
  B10100000,
  B00000000,
  B10100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};

//**CINQ
const unsigned char 
cinq[] PROGMEM={
  B10100000,
  B01000000,
  B10100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

//**SIX
const unsigned char 
six[] PROGMEM={
  B10100000,
  B10100000,
  B10100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000};
/*--------------------------anim attente------------------------------*/
const unsigned char 
attente_1[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00100000,
  B00000000,
  B00000000};

const unsigned char 
attente_2[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00010000,
  B00000000,
  B00000000,
  B00000000};

const unsigned char 
attente_3[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00001000,
  B00000000,
  B00000000,
  B00000000};

const unsigned char 
attente_4[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000100,
  B00000000,
  B00000000};

const unsigned char 
attente_5[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000100,
  B00000000};

const unsigned char 
attente_6[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B0000000,
  B00001000};

const unsigned char 
attente_7[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B0000000,
  B00010000};

const unsigned char 
attente_8[] PROGMEM={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00100000,
  B00000000};

//chronos d'anim en attente
Chrono attenteJoueurs;
boolean attenteJoueursOn = false;

//Valeur des dés
int numP1 = 0;
int numP2 = 0;

Chrono tempsVictoire;
boolean attenteVictoireOn = false;

///////////
// SETUP //
///////////
void setup() {
          pinMode(BUTTON,INPUT);
          debouncerBtn.attach(BUTTON);
          debouncerBtn.interval(5);

          

          
        // INITIATE SERIAL COMMUNICATION.
        Serial.begin(57600);

        WiFi.config(ip , gateway , subnet );
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print("Waiting for connection: ");
                Serial.print(millis());
                Serial.println(" ms");
                delay(50);
        }

        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        // START LISTENING ON UDP PORT 7777.
        udp.begin(3333);

    attenteJoueurs.restart();
    attenteJoueurs.stop();
    
    tempsVictoire.restart();
    tempsVictoire.stop();
}


//////////
// LOOP //
//////////
void loop() {
  //remettre la matrix a 0
  matrix.clear();
  //Seed de la génération random du nombre
      randomSeed(analogRead(TX));

    //Vérification du btn
      debouncerBtn.update();
      int valueBtn = debouncerBtn.read();
      if(valueBtn == 0 && btnOn == false){
              //Si btn appuyé, générer le nombre et l'envoyer à l'autre joueur
              btnOn = true;
              if(numP1 ==0){numP1 = floor(random(1,7));}
              sendPacket(numP1);
              //Serial.print(numP1);
            }
            
      //Si relaché, remettre en attente d'être appuyé
      if(valueBtn == 1 && btnOn == true){
              btnOn = false;
            }

      
        //réception du nombre de l'autre joueur
        if ( udp.parsePacket() ) {
                if ( inbound.parseStream( &udp ) ) {
                        if ( inbound.fullMatch("valNum") ) {
                                numP2 = inbound.nextByte();
                                Serial.println(numP2);
                        }
                }
        }
       
              //Affichage du nombre du joueur 1
              switch(numP1){
                 case 0 : matrix.drawBitmap(0, 0, full , 8, 8, LED_ON);
                 break;
                
                 case 1 : matrix.drawBitmap(0, 0, un , 8, 8, LED_ON);
                 break;
      
                 case 2 : matrix.drawBitmap(0, 0, deux , 8, 8, LED_ON);
                 break;
      
                 case 3 : matrix.drawBitmap(0, 0, trois , 8, 8, LED_ON);
                 break;
      
                 case 4 : matrix.drawBitmap(0, 0, quatre , 8, 8, LED_ON);
                 break;
      
                 case 5 : matrix.drawBitmap(0, 0, cinq , 8, 8, LED_ON);
                 break;
      
                 case 6 : matrix.drawBitmap(0, 0, six , 8, 8, LED_ON);
                 break;
                
              }  
        

            //Affichage du nombre du joueur 2
            switch(numP2){
              case 0 : matrix.drawBitmap(5, 0, full , 8, 8, LED_ON);
                 break;
               case 1 : matrix.drawBitmap(5, 0, un , 8, 8, LED_ON);
               break;
               case 2 : matrix.drawBitmap(5, 0, deux , 8, 8, LED_ON);
               break;
    
               case 3 : matrix.drawBitmap(5, 0, trois , 8, 8, LED_ON);
               break;
    
               case 4 : matrix.drawBitmap(5, 0, quatre , 8, 8, LED_ON);
               break;
    
               case 5 : matrix.drawBitmap(5, 0, cinq , 8, 8, LED_ON);
               break;
    
               case 6 : matrix.drawBitmap(5, 0, six , 8, 8, LED_ON);
               break;
              
            }
         
        //Si les 2 joueurs ont generer un nombre
        if(numP1 != 0 && numP2 != 0){
          //Mettre le chrono d'attente a 0
          attenteJoueurs.restart();
          attenteJoueurs.stop();
          attenteJoueursOn = false;
          
          //Demarrer le chrono pour redemarrer le jeu
          if(attenteVictoireOn == false){
            attenteVictoireOn = true;
              tempsVictoire.start();
            }
          //Si le chrono de victoire atteint sa limite, redemarrer le jeu
          if(tempsVictoire.hasPassed(2500)){
            numP1 = 0;
            numP2 = 0;
            tempsVictoire.restart();
            tempsVictoire.stop();
            attenteVictoireOn = false;
          }

          //Comparaison des nombres
          //Si joueur 1 est plus elevé
          if(numP1 > numP2){
            
            matrix.drawBitmap(0, 0, crochet , 8, 8, LED_ON);
          }
          //Si joueur 2 est plus elevé
          else if(numP1 < numP2){
            
            matrix.drawBitmap(0, 0, x , 8, 8, LED_ON);
          }
          //Si les 2 joueurs ont le meme nombre
          else if(numP1 == numP2){
            matrix.drawBitmap(0, 0, egale , 8, 8, LED_ON);
            
          }

          
       }

        //Si un des joueurs n'a pas generé son nombre
        if(numP1 == 0 || numP2 == 0){
          //Demarrer le chrono d'attente pour l'animation
          if(attenteJoueursOn == false){
              attenteJoueursOn = true;
              attenteJoueurs.start();
            }
            //Affichage de l'anmation dépendante du chrono
          if(attenteJoueurs.hasPassed(0)){
               matrix.drawBitmap(0, 0, attente_1 , 8, 8, LED_ON);               
          }
          if(attenteJoueurs.hasPassed(125)){
               matrix.drawBitmap(0, 0, attente_2 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(250)){
               matrix.drawBitmap(0, 0, attente_3 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(375)){
               matrix.drawBitmap(0, 0, attente_4 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(500)){
               matrix.drawBitmap(0, 0, attente_5 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(625)){
               matrix.drawBitmap(0, 0, attente_6 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(750)){
               matrix.drawBitmap(0, 0, attente_7 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(875)){
               matrix.drawBitmap(0, 0, attente_8 , 8, 8, LED_ON);
          }
          if(attenteJoueurs.hasPassed(1000)){//Si le chrono atteint la limite, redemarrage du chrono
               attenteJoueurs.restart();
          }
        }
    //Affichage des pixels
    matrix.writeDisplay();
}

//Envoyer le packet UDP avec le nombre generé
void sendPacket(int val) {
  outbound.beginPacket("valNum");
  outbound.addByte(val);
  
  udp.beginPacket(recipientIp, 3333);
    outbound.streamPacket(&udp);
  udp.endPacket();
  
}
