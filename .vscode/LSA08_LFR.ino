const int sensorPins[8] = {A0, A1, A2, A3, A4, A5, 4, 6};

const int AIN1 = 10;   
const int AIN2 = 11;   
const int PWMA = 5;    

const int BIN1 = 12;   
const int BIN2 = 13;   
const int PWMB = 3;

float Kp = 4.5;
float Ki = 0.0;
float Kd = 1.2;

float error = 0.0;
float previousError = 0.0;
float integral = 0.0;
float derivative = 0.1;
float correction = 0.0;

int baseSpeed = 100;

void setup() {

  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

void loop() {

  float weightedSum = 0.0;
  float totalWeight = 0.0;
  const float positionValues[8] = {0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0};

  for (int i = 0; i < 8; i++) {
    int sensorValue = digitalRead(sensorPins[i]);
    if (sensorValue == 1) { 
      weightedSum += positionValues[i];
      totalWeight += 1.0;
    }
  }

  float linePosition = (totalWeight > 0.0) ? (weightedSum / totalWeight) : 35.0; 

  error = linePosition - 35.0;

  integral += error;
  derivative = error - previousError;
  correction = Kp * error + Ki * integral + Kd * derivative;

  
  int leftMotorSpeed = baseSpeed + correction;   
  int rightMotorSpeed = baseSpeed - correction;  

  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  if (leftMotorSpeed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    leftMotorSpeed = -leftMotorSpeed;
  }
  analogWrite(PWMA, leftMotorSpeed);

  if (rightMotorSpeed > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    rightMotorSpeed = -rightMotorSpeed;
  }
  analogWrite(PWMB, rightMotorSpeed);

  previousError = error;
}