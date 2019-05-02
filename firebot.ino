//Ultrasonic Sensor
int ultSonicOutPin = 7;
int ultSonicInPin = 10;
int ultSonicThresh = 25;
long ultDist;

//Infrared Sensor
const int infraPin = A2;
int infraThresh = 0;
float infraVoltage = 0;

//Phototransistor
const int photoPin = A3;
int photoThresh = 0;
float photoVoltage = 0;

//Wheel Motors
int leftMotorDirPin = 12;
int leftMotorSpeedPin = 3;
int rightMotorDirPin = 13;
int rightMotorSpeedPin = 11;

//DC Fan Motor
int fanMotorPin = 4;

void setup() {
  Serial.begin(9600);
  pinMode(ultSonicOutPin, OUTPUT);
  pinMode(ultSonicInPin, INPUT);
  pinMode(infraPin, INPUT);
  pinMode(photoPin, INPUT);
  pinMode(leftMotorDirPin, OUTPUT);
  pinMode(leftMotorSpeedPin, OUTPUT);
  pinMode(rightMotorDirPin, OUTPUT);
  pinMode(rightMotorSpeedPin, OUTPUT);
  pinMode(fanMotorPin, OUTPUT);

}

void loop() {
  ultDist = calculateSonicDist(ultSonicOutPin,ultSonicInPin);
  infraVoltage = (analogRead(infraPin)/1023.0)*5.0;
  photoVoltage = (analogRead(photoPin)/1023.0)*5.0;
  Serial.println(infraPin);
  Serial.println(ultDist);
  Serial.println(infraVoltage);
  Serial.println(photoVoltage);

  if(photoVoltage>photoThresh){ //Found Light
    while(calculateSonicDist(uOut,uIn)<uThresh){
      goForward(ldir,lspeed,rdir,rspeed);
    }
    extinguish(leftMotorSpeedPin,rightMotorSpeedPin,fanMotorPin);
    halt(lspeed,rspeed);
  }
  else if(infraVoltage<infraThresh){ //Right empty
    turnRight(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else if(ultDist<ultSonicThresh){ //Front blocked
    turnLeft(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else{ //Front empty
    goForward(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  
  delay(100);
}

long calculateSonicDist(int speakerPin, int micPin){
  digitalWrite(speakerPin,LOW);
  delayMicroseconds(5);
  digitalWrite(speakerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(speakerPin,LOW);
  return pulseIn(micPin,HIGH)/58;
}

void turnRight(int lmd, int lms, int rmd, int rms){ //This is supposed to be set up to rotate in place
  digitalWrite(rmd,HIGH);
  digitalWrite(rms,HIGH);
  digitalWrite(lms,HIGH);
  digitalWrite(lmd,LOW);
  delay() //Set this for as long as you want it to turn
  halt(lspeed,rspeed);
}

void turnLeft(int lmd, int lms, int rmd, int rms){ //This is supposed to be set up to rotate in place
  digitalWrite(lmd,HIGH);
  digitalWrite(lms,HIGH);
  digitalWrite(rms,HIGH);
  digitalWrite(rmd,LOW);
  delay() //Set this for as long as you want it to turn
  halt(lspeed,rspeed);
}

void goForward(int ldir, int lspeed, int rdir, int rspeed){
  digitalWrite(ldir, HIGH);
  digitalWrite(rdir,HIGH);
  digitalWrite(lspeed,HIGH);
  digitalWrite(rspeed,HIGH);
}

void halt(int lspeed, int rspeed){
  digitalWrite(lspeed,LOW);
  digitalWrite(rspeed,LOW);
}

void extinguish(int lspeed, int rspeed, int fmp){
  halt(lspeed,rspeed);
  digitalWrite(fmp,HIGH);
  delay(5000);
}
