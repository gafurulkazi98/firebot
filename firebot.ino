//Ultrasonic Sensor
int ultSonicOutPin = 7;
int ultSonicInPin = 10;
int ultSonicThresh = 25; //To be set
long ultDist;

//Infrared Sensor
const int infraPin = A2;
int infraThresh = 0; //To be set
float infraVoltage = 0;

//Phototransistor
const int photoPin = A3;
int photoThresh = 0; //To be set
float photoVoltage = 0;

//Wheel Motors
int leftMotorDirPin = 12;
int leftMotorSpeedPin = 3;
int rightMotorDirPin = 13;
int rightMotorSpeedPin = 11;

//DC Fan Motor
int fanMotorPin = 4; //To be set

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
  infraVoltage = (analogRead(infraPin)/1023)*5;
  photoVoltage = (analogRead(photoPin)/1023)*5; //Is this the right equation?
  Serial.println(infraPin);
  Serial.println(ultDist);
  Serial.println(infraVoltage);
  Serial.println(photoVoltage);

  if(photoVoltage>photoThresh){ //Found Light
    extinguish(leftMotorDirPin,ultSonicOutPin,ultSonicInPin,ultSonicThresh,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin,fanMotorPin);
    //If we think that the current extinguish() is fine, then I'll replace this part with the code in the function.
    stopMoving(0,0);
  }
  else if(infraVoltage<infraThresh){ //Right Empty
    turnRight(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else if(ultDist>ultSonicThresh){ //Front blocked
    turnLeft(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else{
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

void turnRight(int lmd, int lms, int rmd, int rms){ //I think we should discuss turning, like if we set one wheel to on and the other off, or to rotate them in opposite directions.
  digitalWrite(lmd,HIGH);
  digitalWrite(lms,HIGH);
  digitalWrite(rms,LOW);
}

void turnLeft(int lmd, int lms, int rmd, int rms){
  digitalWrite(rmd,HIGH);
  digitalWrite(rms,HIGH);
  digitalWrite(lms,LOW);
}

void stopMoving(int lspeed, int rspeed){
  digitalWrite(lspeed,LOW);
  digitalWrite(rspeed,LOW);
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

void extinguish(int fmp, int uOut, int uIn, int uThresh, int ldir, int lspeed, int rdir, int rspeed){
  while(calculateSonicDist(uOut,uIn)<uThresh){
    goForward(ldir,lspeed,rdir,rspeed);
  }
  halt(lspeed,rspeed);
  digitalWrite(fmp,HIGH);
  delay(5000);
}