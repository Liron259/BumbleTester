#include <Servo.h>
#include <LiquidCrystal.h>

Servo victor1, spark1, victor2, spark2;
const int speed1 = A0, speed2 = A1, forward1 = A2, backward1 = A3, forward2 = A4, backward2 = A5;
int direction1 = 0, direction2 = 0, power1 = 0, power2 = 0, lastPower1 = 0, lastPower2 = 0, lastDirection1 = 0, lastDirection2 = 0;  
long lastOperationTime = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  pinMode(speed1, INPUT);
  pinMode(speed2, INPUT);
  pinMode(forward1, INPUT_PULLUP);
  pinMode(backward1, INPUT_PULLUP);
  pinMode(forward2, INPUT_PULLUP);
  pinMode(backward2, INPUT_PULLUP);

  victor1.attach(6);
  spark1.attach(7);
  victor2.attach(8);
  spark2.attach(9);
  
  lcd.begin(16, 2);
  lcd.print("Starting");
  lcd.setCursor(0, 1);
  lcd.print("BumbleTester");
  Serial.println("Starting BumbleTester");
  delay(2000);
  lcd.clear();
}

void loop() {
  readSpeeds();
  readToggles();
  if(power1 != lastPower1 || power2 != lastPower2 || direction1 != lastDirection1 || direction2 != lastDirection2){
    if(millis() >= 3000 + lastOperationTime){
      writePowerToControllers();
      showOutputPower();
    }
  }
  lastPower1 = power1;
  lastPower2 = power2;
  lastDirection1 = direction1;
  lastDirection2 = direction2;
  delay(20);
}

void readSpeeds(){
  power1 = analogRead(speed1);
  power2 = analogRead(speed2);
}

void readToggles(){
  // reading toggle 1 direction
  if(!digitalRead(forward1)){
    direction1 = 1;
  }else if(!digitalRead(backward1)){
    direction1 = 2;
  }else{
    direction1 = 0;
  }

  // reading toggle 2 direction
  if(!digitalRead(forward2)){
    direction2 = 1;
  }else if(!digitalRead(backward2)){
    direction2 = 2;
  }else{
    direction2 = 0;
  }
}

void writePowerToControllers(){
  // getting output for controller 1
  int maxOut = 1500;
  if(direction1 == 1){
    maxOut = 2000;
  }else if(direction1 == 2){
    maxOut = 1000;
  }
  int output1 = map(power1, 0, 1023, 1500, maxOut);

  // getting output for controller 2
  maxOut = 1500;
  if(direction2 == 1){
    maxOut = 2000;
  }else if(direction2 == 2){
    maxOut = 1000;
  }
  int output2 = map(power2, 0, 1023, 1500, maxOut);

  // writing to controllers
  victor1.writeMicroseconds(output1);
  spark1.writeMicroseconds(output1);
  victor2.writeMicroseconds(output2);
  spark2.writeMicroseconds(output2);
  lastOperationTime = millis();
}

void showOutputPower(){
  int powerPercentage1 = map(power1, 0, 1023, 0, 100);
  int powerPercentage2 = map(power2, 0, 1023, 0, 100);

  // print motor 1 power
  lcd.print("Power 1: ");
  for(int x = 0; x > 3; x++){
    lcd.setCursor(0, 12);
    lcd.print("");
  }
  if(powerPercentage1 == 100){
    lcd.setCursor(0, 12);
  }else{
    lcd.setCursor(0, 13);
  }
  lcd.print(powerPercentage1);
  lcd.print("%");
  Serial.print("Power 1: ");
  Serial.print(powerPercentage1);
  Serial.println("%");

  
  // print motor 2 power
  lcd.setCursor(0,1);
  lcd.print("Power 1: ");
  for(int x = 0; x > 3; x++){
    lcd.setCursor(0, 12);
    lcd.print("");
  }
  if(powerPercentage2 == 100){
    lcd.setCursor(0, 12);
  }else{
    lcd.setCursor(0, 13);
  }
  lcd.print(powerPercentage2);
  lcd.print("%");
  Serial.print("Power 2: ");
  Serial.print(powerPercentage2);
  Serial.println("%");
}
