#include <Wire.h>
#include <EVShield.h>
//#include <EVShieldAGS.h>
#include <EVs_EV3Ultrasonic.h>

const int trigPin = 8; // Les pins du capteur ulrason
const int echoPin = 12;

//  Declare shield variable for this program
EVShield evshield(0x34,0x36);


// Caractéristiques geometriques du robot à renseigner
const float wheelsradius = 40; //mm
const float betweenWheels = 180; //mm

// Juste pour appeler les moteurs left et right plutot que 1 et 2
const SH_Motor Motor_Left = SH_Motor_1;
const SH_Motor Motor_Right = SH_Motor_2;

// fonction pour faire avancer le robot tout droit d'une certaine distance en mm
void moveTo (float distance){
  float a = distance/wheelsradius * 180/3.14;
  // fait tourner les deux roues dans le meme sens
  evshield.bank_a.motorRunTachometer ( SH_Motor_Both,
		SH_Direction_Forward,
		SH_Speed_Medium,
		a,
		SH_Move_Relative,
		SH_Completion_Wait_For,
		SH_Next_Action_BrakeHold  
  );
  
}

// fonction pour faire tourner le robot sur lui meme d'un certain angle en degres
void rotate(float angle) {
  //Serial.println (angle);
  float a = angle*betweenWheels/(2*wheelsradius);
  // fait tourner les deux roues dans des sens opposes
  evshield.bank_a.motorRunTachometer ( Motor_Right,
		SH_Direction_Forward,
		SH_Speed_Medium,
		a,
		SH_Move_Relative,
		SH_Completion_Dont_Wait  	,
		SH_Next_Action_BrakeHold  
	);
  evshield.bank_a.motorRunTachometer ( Motor_Left,
		SH_Direction_Reverse,
		SH_Speed_Medium,
		a,
		SH_Move_Relative,
		SH_Completion_Wait_For  	,
		SH_Next_Action_BrakeHold  
	);

}

// la fonction setup est executee au demarage du programme (upload, allumage de la carte ou reset)
void setup()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    char            str[50];

    Serial.begin(115200);       // start serial for output (ouvrir le serial monitor (ctr+shift+M) et selectionner 115200 pour afficher les infos)

    Serial.println ("-----------------------------------------------");

    //
    // initialize the shield i2c interface.
    //
    evshield.init( SH_HardwareI2C );
    //
    // reset motors.
    //
    evshield.bank_a.motorReset();
    evshield.bank_b.motorReset();

    // pause attend GO
    while (!evshield.getButtonState(BTN_GO)) {
        if (millis() % 1000 < 3) {
            Serial.println("Press GO button to continue");
        }
    }
    
}

// la fonction loop est executee en boucle juste apres setup
void loop()
{
  float distance, duration;

  // see distance from sensor.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; // calcul de la distance à partir de la durée de l'aller/retour du signal sonor
  Serial.print(distance);
  Serial.println(" cm");

  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else{
    if (distance < 15) {
      // si la distance à l'obstacle est inférieure à 15cm (distance à adapter), on arrete les moteurs
      Serial.println("Obstacle!!");
      evshield.bank_a.motorStop(SH_Motor_Both, SH_Next_Action_BrakeHold);
    }
    else {
      // sinon on peut faire des actions deplacement, rotations
      rotate(180);
      delay(100);
      moveTo(1000);
      delay(100);  // petit temps de pause en ms
      
    }
  }

delay(100);
       
}
