/*
Programme arduino pour faire un tourelle avec un capteur UltraSon et un servomoteur

Pas d'obstacle devant le capteur => LED allumée (peut etre remplacer par les moteurs qui font avancés le robot) et balayage de la vision
Obstacle => la LED s'etteint et le balayage s'arrete (le capteur continu à regarder l'obstacle tant qu'il est là)

Si on utilise pas les moteurs et capteurs légo il faut bricoler un truc pour que le capteur tourne avec l'axe du moteur et fixer le moteur à la structure (impression 3D par ex)
*/


#include <Servo.h>  // inclure la bibliotheque de controle du servo

Servo myservo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position

// Les pins du capteur ulrason
const int trigPin = 8; // quand on met un rectangle sur le pin trigger, le capteur envoie un signal UltraSon
const int echoPin = 12;  // et renvoi une sortie sur le pin echo quand il recoit l'echo, on peut donc calculer la distance de l'obstacle

const int ledPin = 2; // pin de la LED

const int servoPin = 9; // pin du servo (utiliser uniquement les pins PWM (3,5,6,9,19 ou 11 avec la petite vague sur l'arduino))

int sens=1; // 1 ou -1 selon le sens de rotation de la tourelle (on pourrait utiliser un bool pour optimiser) augmenter la valeur pour augmenter la vitesse
float duration; // duree de l'aller/retour du signal UltraSon
float distance; // distance à l'obstacle mesurée


void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600); // communication serie pour info de debbugage sur le PC
} 


void loop() 
{
  // see distance from sensor.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; // calcul de la distance à partir de la durée de l'aller/retour du signal sonore
  Serial.print(distance);
  Serial.println(" cm");

  if (distance >= 15 || distance <= 0){ // si pas d'obstacle dans les 15 cm
    pos += sens; // tourner
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    digitalWrite(ledPin, HIGH); // allumer la LED

  }
  else{
    digitalWrite(ledPin, LOW); // sinon éteint la LED et ne tourne pas
  }

  if (pos >= 180 or pos <= 0) { // changement de sens en butée
    sens *= -1;
    if (pos>180){
      pos=180;
    }
    else if (pos<0){
      pos=0;
    }
  }

  delay(15);                       // petite attente de 15ms pour laisser au moteur le temps de tourner et au capteur de se preparer à une nouvelle mesure

} 



