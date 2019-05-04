//Ultrasonic Sensor
int ultSonicOutPin = 7;
int ultSonicInPin = 10;
long ultSonicThresh = 15.0;
long ultDist;

//Infrared Sensor
int infraPin = A2;
float infraThresh = 1.25;
float infraVoltage = 0;

//Phototransistor
const int photoPin = A3;
int photoThresh = 1.4;
float photoVoltage = 0;

//Wheel Motors
int leftMotorDirPin = 12;
int leftMotorSpeedPin = 3;
int rightMotorDirPin = 13;
int rightMotorSpeedPin = 11;

//DC Fan Motor
int fanMotorPin = 4;

int slowMode = 0;
int rt=250;
int lt=300;
int bt=50;
int ht=15;

void setup() {
 
  pinMode(ultSonicOutPin, OUTPUT);
  pinMode(ultSonicInPin, INPUT);
  pinMode(infraPin, INPUT);
  pinMode(photoPin, INPUT);
  pinMode(leftMotorDirPin, OUTPUT);     
  pinMode(leftMotorSpeedPin, OUTPUT);
  pinMode(rightMotorDirPin, OUTPUT);
  pinMode(rightMotorSpeedPin, OUTPUT);
  pinMode(fanMotorPin, OUTPUT);
 Serial.begin(9600);
}

void loop() {
 
  ultDist = calculateSonicDist(ultSonicOutPin,ultSonicInPin);
  infraVoltage = (analogRead(infraPin)/1023.0)*5.0;
  photoVoltage = (analogRead(photoPin)/1023.0)*5.0;
  Serial.print("ultDist: ");
  Serial.println(ultDist);
  Serial.print("infraVoltage: ");
  Serial.println(infraVoltage);
  Serial.print("photoVoltage: ");
  Serial.println(photoVoltage);

  if(photoVoltage>photoThresh){ //Found Light
    while(calculateSonicDist(ultSonicOutPin,ultSonicInPin)>ultSonicThresh){
      Serial.println("DING!");
      goForward(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
    }
    extinguish(leftMotorSpeedPin,rightMotorSpeedPin,fanMotorPin);
    halt(leftMotorSpeedPin,rightMotorSpeedPin,ht);
    exit(1);
  }
  else if((ultDist<ultSonicThresh)){ //Front blocked
   // goBack(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
   halt(leftMotorSpeedPin,rightMotorSpeedPin, ht);
   
    turnLeft(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin, lt);
  }
  else if((infraVoltage<infraThresh)){ //Right Empty 
    halt(leftMotorSpeedPin,rightMotorSpeedPin,ht);
    turnRight(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin, rt);
  }
  else {
    goForward(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  delay(75);
}

long calculateSonicDist(int speakerPin, int micPin){
  digitalWrite(speakerPin,LOW);
  delayMicroseconds(5);
  digitalWrite(speakerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(speakerPin,LOW);
  return pulseIn(micPin,HIGH)/58;
}

void turnRight(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin, int t){
  digitalWrite(leftMotorDirPin,LOW);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(rightMotorSpeedPin,HIGH);
  digitalWrite(rightMotorDirPin,HIGH);
  delay(250);
  halt(leftMotorSpeedPin,rightMotorSpeedPin, ht);
}

void turnLeft(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin, int t){
  goBack(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin,bt);
  digitalWrite(rightMotorDirPin,LOW);
  digitalWrite(rightMotorSpeedPin,HIGH);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(leftMotorDirPin,HIGH);
  delay(300);
  halt(leftMotorSpeedPin,rightMotorSpeedPin, ht);
}

void goForward(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin){
  digitalWrite(leftMotorDirPin, LOW);
  digitalWrite(rightMotorDirPin,LOW);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(rightMotorSpeedPin,HIGH);
//  

}
void goBack(int leftMotorDirPin,int leftMotorSpeedPin,int rightMotorDirPin,int rightMotorSpeedPin, int t){
  digitalWrite(leftMotorDirPin, HIGH);
  digitalWrite(rightMotorDirPin,HIGH);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(rightMotorSpeedPin,HIGH);
  delay(50);
}

void halt(int leftMotorSpeedPin, int rightMotorSpeedPin, int t){
  digitalWrite(leftMotorSpeedPin,LOW);
  digitalWrite(rightMotorSpeedPin,LOW);
  delay(15);
}

void extinguish(int leftMotorSpeedPin, int rightMotorSpeedPin, int fanMotorPin){
  halt(leftMotorSpeedPin,rightMotorSpeedPin, ht);
  digitalWrite(fanMotorPin,HIGH);
  delay(500);
  digitalWrite(fanMotorPin, LOW);
}
