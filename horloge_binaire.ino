
#include <TimeLib.h>

// Les variables
const int distance_vide = 0;
const int distance_horloge = 0;
const int distance_date = 0;
const int distance_temperature = 0;
unsigned long temps_affichage = 1000000;

//////// Il y a une bibliothèque "time" pour gérer le temps avec ou sans horloge externe.

// Mappage des pins.
// // dizaine des heures.
const int pin_LED11 = 1;
const int pin_LED21 = 2;
// // unité des heures.
const int pin_LED12 = 3;
const int pin_LED22 = 4;
const int pin_LED32 = 5;
const int pin_LED42 = 6;
// // dizaine des minutes.
const int pin_LED13 = 7;
const int pin_LED23 = 8;
const int pin_LED33 = 9;
// // unité des minutes
const int pin_LED14 = 10;
const int pin_LED24 = 11;
const int pin_LED34 = 12;
const int pin_LED44 = 13;
// // // tableau des LEDs.
const int pin_heure[4][4] = {
  {pin_LED11, pin_LED12, pin_LED13, pin_LED14},
  {pin_LED21, pin_LED22, pin_LED23, pin_LED24},
  {0        , pin_LED32, pin_LED33, pin_LED34},
  {0        , pin_LED42, 0        , pin_LED44}
};
// // detecteur de mouvement.
const int pin_detecteur_mouvement = 14;
// // detecteur de distance.
const int pin_detecteur_distance = 15;
// // capteur de température.
const int pin_temperature = 16;

// Fonction pour passer d'un nombre décimal à un nombre binaire et le stocker dans une liste (4 bits donc 0<= nombre < 16). Utilisation de la division euclidienne.
bool* decimalVersBinaire(const int nombre_decimal){
  const int nb_bits = 4;
  bool reste[nb_bits];
  int quotient = nombre_decimal;
  if (nombre_decimal == 0) {
    for (int k=0; k<nb_bits; k++){
      reste[k]=false;
    }
  }
  else {
    int k = 0;
    while (quotient != 1) {
      reste[nb_bits-k] = (quotient % 2) == 1;
      quotient = quotient / 2;
      k=k+1;
    }
  }
  return reste;
}

// Allume la led correspondante au ligne et colonne donnés si a_allumer est vraie.
void allumeLED (const int ligne, const int colonne, bool a_allumer){
  if (a_allumer){
    digitalWrite(pin_heure[ligne][colonne], LOW); // on fait 1-nombre_binaire si on met le pin en sortie c'est-à-dire l'alimentation ne vient pas de l'arduino.
  }
  else {
    digitalWrite(pin_heure[ligne][colonne], HIGH); // on fait 1-nombre_binaire si on met le pin en sortie c'est-à-dire l'alimentation ne vient pas de l'arduino.
  }
}

// Eteint la led correspondant au ligne et colonne donnés.
void eteindreLED (const int ligne, const int colonne){
  allumeLED(ligne, colonne, false); // on remet donc à 1 pour l'éteindre si l'alimentation ne vient pas de l'arduino.
}

// Allume les LEDs en affichage binaire en fonction du nombre décimal donné et dans quelle colonne le placer (dizaine des heures, unité des heures, etc). En MULTIPLEXAGE
void afficheNombreSurLED (const int nombre_decimal, const int colonne){
  bool* nombre_binaire = decimalVersBinaire(nombre_decimal);
  for (int ligne=0; ligne<4; ligne++){
    allumeLED(ligne, colonne, nombre_binaire[ligne]);
//    delay(1000);
//    eteindreLED(ligne, colonne);
  }
}

// Renvoie 2 chiffres lorsqu'on rentre un nombre à 2 chiffres (ex: en entre 15 on retourne 1 et 5).
int separeDeuxChiffres(const int nombre){
  int dizaine = nombre/10;
  int unite = nombre - dizaine*10;
  return dizaine, unite;
}

// Renvoie 4 chiffres lorsqu'on rentre un nombre à 4 chiffres (ex: en entre 1536 on retourne 1, 5, 3 et 6).
int separeQuatreChiffres(const int nombre){
  int millier = nombre/1000;
  int centaine = (nombre - millier*1000)/100;
  int dizaine = (nombre - centaine*100 - millier*1000)/10;
  int unite = nombre - dizaine*10 - centaine*100 - millier*1000;
  return millier, centaine, dizaine, unite;
}

// Affiche l'heure pendant un certains temps tant que le capteur de distance ne reçoit pas de nouvelles valeurs.
void affichageHeure(){
//  int distance = distanceCapteur();  
  int dizaine_heure, unite_heure = separeDeuxChiffres(hour());
  int dizaine_minute, unite_minute = separeDeuxChiffres(minute());
  unsigned long temps, temps_relatif = millis(), millis();
  int k = 0;
  while (1){ //((temps_relatif - temps < temps_affichage) && ((distance == distance_vide) || (distance == distance_horloge))){
//    distance = distanceCapteur();
    dizaine_heure, unite_heure = separeDeuxChiffres(hour());
    dizaine_minute, unite_minute = separeDeuxChiffres(minute());
    afficheNombreSurLED(dizaine_heure, 0);
    afficheNombreSurLED(unite_heure, 1);
    afficheNombreSurLED(dizaine_minute, 2);
    afficheNombreSurLED(unite_minute, 3);
    temps_relatif = millis();
  }
}

// Récupère la distance du capteur à ultrasons et la classe suivant l'affichage voulu correspondant (horloge, date, temperature).
int distanceCapteur(){
  // TODO
}

// Retourne "true" si le detecteur de mouvement detecte un mouvement.
bool detecteurMouvement(){
  // TODO
}

void setup() {
  Serial.begin(9600);
  for (int k=1; k<14; k++){
    pinMode(k, OUTPUT);
    digitalWrite(k, 1);
  }
}

void loop() {
//  // put your main code here, to run repeatedly:
//  int distance = 0;
//  if (detecteurMouvement() == true){
//    distance = distanceCapteur();
//    if (distance == distance_horloge){
//      // affichageHeure(heures_binaire);
//    }
//    if (distance == distance_date){
//      // affichageDate(date);
//    }
//    if (distance == distance_temperature){
//      // affichageTemperature(temperature);
//    }
//  }
  affichageHeure();
  int heure = hour();
  Serial.println(heure);
  Serial.println(minute());
}
