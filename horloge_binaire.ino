
#include <TimeLib.h>

// Les variables
const int distance_vide = 0; // distance lorsque aucun obstacle est en face du detecteur.
const int distance_horloge = 0; // distance au detecteur pour afficher l'heure.
const int distance_date = 0; // distance au detecteur pour aficher la date.
const int distance_temperature = 0; // distance au detecteur pour afficher la temperature.
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
const int pin_emetteur_distance = 15;
// // capteur de température.
const int pin_temperature = 17;

// Fonction pour passer d'un nombre décimal à un nombre binaire et le stocker dans une liste (4 bits donc 0<= nombre < 16). Utilisation de la division euclidienne.
bool* decimalVersBinaire(const int nombre_decimal){
  const int nb_bits = 4;
  bool* reste = new bool[nb_bits];
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
    digitalWrite(pin_heure[ligne][colonne], LOW); // LED allume : on fait (1 - nombre_binaire) si on met le pin en sortie c'est-à-dire l'alimentation ne vient pas de l'arduino.
  }
  else {
    digitalWrite(pin_heure[ligne][colonne], HIGH); // LED eteinte.
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
   delay(1);
   eteindreLED(ligne, colonne);
  }
  delete(nombre_binaire);
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

// Affiche l'heure.
void affichageHeure(){
  int dizaine_heure, unite_heure = separeDeuxChiffres(hour());
  int dizaine_minute, unite_minute = separeDeuxChiffres(minute());
  int k = 0;
  dizaine_heure, unite_heure = separeDeuxChiffres(hour());
  dizaine_minute, unite_minute = separeDeuxChiffres(minute());
  afficheNombreSurLED(dizaine_heure, 0);
  afficheNombreSurLED(unite_heure, 1);
  afficheNombreSurLED(dizaine_minute, 2);
  afficheNombreSurLED(unite_minute, 3);
}

// recupere la temperature sur le capteur de temperature.
int temperatureCapteur(){
    int temperature = 0; // temperature mesuree.
    int temp[10]; // creation du tableau pour faire une moyenne de mesures.
    int time = 20; // temps entre chaque mesure.
    for (int k=0; k<9; k++){ // On recupere une mesure de temperature pour chaque cases du tableau
        temp[k] = map(analogRead(pin_temperature), 0, 410, -50, 150);
        delay(time);
    }
    temp[9] = map(analogRead(pin_temperature), 0, 410, -50, 150);
    //“map(a,b,c,d,e)” - This is a “Map command”. With this command it is possible
    //to change a read out value (a) from one region between (b) and (c) into a
    //region between (d) and (e). In our case this means: The sensor value gets read out right in the map
    //command “analogRead(pin_temperature)”. The value should be between 0 and 410 (= values
    //between 0V and 2V at the analog port). The sensor outputs this voltage values
    //if it measures temperature between -50°C and 150°C. With the “map command”
    //these values get converted into degree values between -50°C and 150°C.
    for (int k=0; k=9; k++){ // on fait la moyenne des differentes mesures stockees dans le tableau pour obtenir une valeure de temperature.
        temperature += temp[k];
    }
    temperature=temperature/10;
    return temperature
}

// affiche la temperature.
void affichageTemperature(){
    int dizaine_temperature, unite_temperature = separeDeuxChiffres(temperatureCapteur())
  afficheNombreSurLED(dizaine_temperature, 2);
  afficheNombreSurLED(unite_temperature, 3);
}

// Récupère la distance du capteur à ultrasons.
int distanceCapteur(){
    long temps_aller_retour=0;
    long distance=0; //The value “distance” will save the calculated distance. It will
    //start with “0”. Instead of “int” we are using “long” for this value, to save a
    //bigger number.
    digitalWrite(pin_detecteur_distance, LOW); //Low voltage on the trigger pin to produce a
    //clear signal.
    delay(5); //….for 5 milliseconds.
    digitalWrite(pin_detecteur_distance, HIGH); //Creating the soundwave.
    delay(10); //..for 10 milliseconds.
    digitalWrite(pin_detecteur_distance, LOW); //Stop creating the soundwave.
    temps_aller_retour = pulseIn(pin_emetteur_distance, HIGH); //With the command pulseIn (Capital “i” in the
    //front of the “n”) the arduino board measures the time between sending and
    //receiving the soundwave.
    distance = (temps_aller_retour/2) / 29.1; //This calculation transforms the measured time into
    //the distance in centimeter. (The sound needs 29,1 seconds for one centimeter.
    //The time gets divided with two, because we only want to get one distance and
    //not the two ways that the soundwave has to take).
    return distance
    }
}

// Retourne "true" si le detecteur de mouvement detecte un mouvement.
bool detecteurMouvement(){
    int movementstatus=0; //The word “movementstatus” stands for the value 0. Later
    //on there will be saved if a movement is detected or not
    movementstatus = digitalRead(pin_detecteur_mouvement);
    //(command: digitalRead). The result will be saved under “movementstatus”. (HIGH
    //means 5V and LOW means 0V)
    if(movementstatus==HIGH) {//if a movement is detected (voltage signal high) ..
        return true;
    }
    else {
        return false;
    }
}

void setup() {
    setTime(17,35,0,26,7,2016);
    pinMode(pin_detecteur_mouvement, INPUT); // Le pin connecté au detecteur de mouvement est définit comme une entrée.
    pinMode(pin_detecteur_distance, OUTPUT); //”pin_detecteur_distance” est une sortie. (capteur d'ultrasons)
    pinMode(pin_emetteur_distance, INPUT); //”pin_emetteur_distance” est une entrée. (emetteur d'ultrasons)
    pinMode(pin_temperature, INPUT); // Le pin connecté au detecteur de température est définit comme une entrée.

    for (int k=1; k<14; k++){ // On définit les pins responsables de l'alimntation des LEDs comme des sorties.
        pinMode(k, OUTPUT);
        digitalWrite(k, 1);
    }
    int commande_precedente = 2;
}

void loop() {
//  // put your main code here, to run repeatedly:
    if (detecteurMouvement()) {
        unsigned long temps, temps_relatif = millis(), millis();
    }
    while ((temps_relatif-temps)<temps_affichage) {
        switch (distance) {
            case distance_vide:
                // do commande précédente.
                switch (commande_precedente) {
                    case 1:
                        // affichageDate();
                        break;
                    case 2:
                        // affichageHeure();
                        break;
                    case 3:
                        // affichageTemperature();
                        break;
                }
                break;
            case distance_date:
                // affichageDate();
                commande_precedente = 1
                break;
            case distance_horloge:
                // affichageHeure();
                commande_precedente = 2
                break;
            case distance_temperature:
                // affichageTemperature();
                commande_precedente = 3
                break;
        }
        temps_relatif = millis();
    }
}
