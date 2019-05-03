//Ultrasonic Sensor
int ultSonicOutPin = 7;
int ultSonicInPin = 10;
long ultSonicThresh = 25.0; //To be set
long ultDist;

//Infrared Sensor
int infraPin = A2;
float infraThresh = 1.5;//To be set
float infraVoltage = 0;

//Phototransistor
const int photoPin = A3;
int photoThresh = 5; //To be set
float photoVoltage = 0;

//Wheel Motors
int leftMotorDirPin = 12;
int leftMotorSpeedPin = 3;
int rightMotorDirPin = 13;
int rightMotorSpeedPin = 11;

//DC Fan Motor
int fanMotorPin = 4; //To be set

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
  photoVoltage = (analogRead(photoPin)/1023.0)*5.0; //Is this the right equation?
  Serial.print("ultDist: ");
  Serial.println(ultDist);
  Serial.print("infraVoltage: ");
  Serial.println(infraVoltage);
  Serial.print("photoVoltage: ");
  Serial.println(photoVoltage);

  if(photoVoltage>=photoThresh){ //Found Light
    while(calculateSonicDist(ultSonicOutPin,ultSonicInPin)>ultSonicThresh){
      goForward(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
    }
    extinguish(leftMotorSpeedPin,rightMotorSpeedPin,fanMotorPin);
    //If we think that the current extinguish() is fine, then I'll replace this part with the code in the function.
    halt(leftMotorSpeedPin,rightMotorSpeedPin);
  }
  else if((infraVoltage<infraThresh)){ //Right Empty 
    turnRight(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else if((ultDist<ultSonicThresh)){ //Front blocked
   // goBack(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
    turnLeft(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  }
  else {
    goForward(leftMotorDirPin,leftMotorSpeedPin,rightMotorDirPin,rightMotorSpeedPin);
  } 
  
  delay(25);
}

long calculateSonicDist(int speakerPin, int micPin){
  digitalWrite(speakerPin,LOW);
  delayMicroseconds(5);
  digitalWrite(speakerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(speakerPin,LOW);
  return pulseIn(micPin,HIGH)/58;
}

void turnRight(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin){ //I think we should discuss turning, like if we set one wheel to on and the other off, or to rotate them in opposite directions.
  digitalWrite(leftMotorDirPin,LOW);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(rightMotorSpeedPin,HIGH);
  digitalWrite(rightMotorDirPin,HIGH);
  delay(75);
  halt(leftMotorSpeedPin,rightMotorSpeedPin);
}

void turnLeft(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin){
  digitalWrite(rightMotorDirPin,LOW);
  digitalWrite(rightMotorSpeedPin,HIGH);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(leftMotorDirPin,HIGH);
  delay(75);
  halt(leftMotorSpeedPin,rightMotorSpeedPin);
}

void goForward(int leftMotorDirPin, int leftMotorSpeedPin, int rightMotorDirPin, int rightMotorSpeedPin){
  digitalWrite(leftMotorDirPin, LOW);
  digitalWrite(rightMotorDirPin,LOW);
  digitalWrite(leftMotorSpeedPin,HIGH);
  digitalWrite(rightMotorSpeedPin,HIGH);
  halt(leftMotorSpeedPin,rightMotorSpeedPin);
}
//void goBack(int leftMotorDirPin,int leftMotorSpeedPin,int rightMotorDirPin,int rightMotorSpeedPin)

void halt(int leftMotorSpeedPin, int rightMotorSpeedPin){
  digitalWrite(leftMotorSpeedPin,LOW);
  digitalWrite(rightMotorSpeedPin,LOW);
  delay(15);
}

void extinguish(int leftMotorSpeedPin, int rightMotorSpeedPin, int fanMotorPin){
  halt(leftMotorSpeedPin,rightMotorSpeedPin);
  digitalWrite(fanMotorPin,HIGH);
  delay(500);
  digitalWrite(fanMotorPin, LOW);
}
