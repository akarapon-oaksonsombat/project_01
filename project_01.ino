/* Libraries */
/* KeyPad */ 
#include <Keypad.h>
/* LCD Display */ 
#include <LiquidCrystal_I2C.h>
/* Others */ 
#include <Wire.h> 
/* Servo */ 
#include <Servo.h>


/* Constants */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define RESET_BUT_PIN 12
#define ANALOG_KEY_PIN A0
#define BRUSH_SERVO_PIN 8
#define UP_DOWN_SERVO_PIN 9
#define DIR_PIN_RIGHT 2
#define STEP_PIN_RIGHT 3
#define DIR_PIN_LEFT 4
#define STEP_PIN_LEFT 5
#define STEPS_PER_MM 1
#define STEPS_PER_DEGREE 8
#define DELAY 200


/* Parameters do not change */
/* Keypad Parameters */
const char kp4x4Keys[KEYPAD_ROWS][KEYPAD_COLS]  = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte rowKp4x4Pin [4] = {26, 27, 28, 29};
byte colKp4x4Pin [4] = {22, 23, 24, 25};
/* LCD */
byte blackbox[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};


/* Define Parameters */


/* Variables */
Keypad kp4x4  = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, KEYPAD_ROWS, KEYPAD_COLS);
LiquidCrystal_I2C lcd(0x27,20,4);
Servo brushServo, upDownServo;
int currentMenu = 0;

void setup() {
  /* Init Serial USB */ 
  Serial.begin(9600); Serial.println(F("Initialize System"));
  /* SteppingMotor Setup */ 
  pinMode(DIR_PIN_RIGHT, OUTPUT); pinMode(STEP_PIN_RIGHT, OUTPUT); pinMode(DIR_PIN_LEFT, OUTPUT); pinMode(STEP_PIN_LEFT, OUTPUT);
  /* ServoMotor Setup */ 
  brushServo.attach(BRUSH_SERVO_PIN); upDownServo.attach(UP_DOWN_SERVO_PIN);
  /* LCD Setup */ 
  lcd.init(); lcd.createChar(0, blackbox); lcd.backlight();
  /* Analog Key Setup */ 
  pinMode(ANALOG_KEY_PIN,INPUT_PULLUP);
  /* */
  pinMode(RESET_BUT_PIN, INPUT);
  start();
//  plate_rotate(1000, 0);
 
}

void loop() {
//  plate_move(10, 0);
//  plate_rotate(10, 0);
  
}
void start(){
//  lcd_welcome_page();
//  delay(3000);
//  lcd_loading();
  menu();
}
void menu(){
  char keyPress;
  while(1){
    if(currentMenu==0) menu1();
    if(currentMenu==1) menu2();
    if(currentMenu==2) menu3();
    keyPress = readKeypad();
    if(keyPress=='A') currentMenu++;
    if(keyPress=='3'){
      manual();
    }
    if(keyPress=='1'){
      square();
    }
    currentMenu = currentMenu%3;
  }
}
void square(){
  plate_move(120, 1); delay(1000);
  plate_rotate(90, 1);delay(1000);
  plate_move(120, 1);delay(1000);
  plate_rotate(90, 1);delay(1000);
  plate_move(120, 1);delay(1000);
  plate_rotate(90, 1);delay(1000);
  plate_move(120, 1);delay(1000);
}
void manual(){
   manual_des();
   String btn;
   while(1){
     btn = readAnalogKeypad();
//     Serial.println(btn);
     if(btn=="RIGHT"){
      plate_rotate(10, 0);
      
     }else if(btn=="LEFT"){
      plate_rotate(10, 1);
//      delay(1000);
     }else if(btn=="UP"){
      plate_move(10, 1);
//      delay(1000);
     }else if(btn=="DOWN"){
      plate_move(10, 0);
//      delay(1000);
     }
   }
   
}
void manual_des(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("2| UP");
    lcd.setCursor(0,1);
    lcd.print("4| LEFT");
    lcd.setCursor(0,2);
    lcd.print("6| RIGHT");
    lcd.setCursor(0,3);
    lcd.print("8| DOWN");
}
void menu1(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1| QUICK START");
    lcd.setCursor(0,1);
    lcd.print("2| EXAMPLE PROJECT");
    lcd.setCursor(0,2);
    lcd.print("3| MANUAL MODE");
    lcd.setCursor(0,3);
    lcd.print("A| NEXT PAGE");
}
void menu2(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("4| ");
    lcd.setCursor(0,1);
    lcd.print("5| ");
    lcd.setCursor(0,2);
    lcd.print("6| ");
    lcd.setCursor(0,3);
    lcd.print("A| NEXT PAGE");
}
void menu3(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("7| ");
    lcd.setCursor(0,1);
    lcd.print("8| ");
    lcd.setCursor(0,2);
    lcd.print("9| ");
    lcd.setCursor(0,3);
    lcd.print("A| NEXT PAGE");
}
/* Plate Functions */
void plate_rotate(int degree, int clockwise){
  /* if clockwise = 1 will rotate >>> and if clockwise = 0 will rotate <<< */
  int steps = degree*STEPS_PER_DEGREE;
  if(!clockwise){
    digitalWrite(DIR_PIN_RIGHT, HIGH);  digitalWrite(DIR_PIN_LEFT, HIGH);
  }else{
    digitalWrite(DIR_PIN_RIGHT, LOW);  digitalWrite(DIR_PIN_LEFT, LOW);
  }
  for(int i=0; i<steps;i++){
    digitalWrite(STEP_PIN_RIGHT, HIGH); digitalWrite(STEP_PIN_LEFT, HIGH); delayMicroseconds(DELAY);
    digitalWrite(STEP_PIN_RIGHT, LOW); digitalWrite(STEP_PIN_LEFT, LOW); delayMicroseconds(DELAY);  
  }
}
void plate_move(int mm, int forward){
  /* forward can be only 1 or 0 */
  int steps = mm*STEPS_PER_MM;
  if(!forward){
    digitalWrite(DIR_PIN_RIGHT, HIGH); digitalWrite(DIR_PIN_LEFT, LOW);
  }else{
    digitalWrite(DIR_PIN_RIGHT, LOW); digitalWrite(DIR_PIN_LEFT, HIGH);
  }
  for(int i=0; i<steps;i++){
    digitalWrite(STEP_PIN_RIGHT, HIGH); digitalWrite(STEP_PIN_LEFT, HIGH); delayMicroseconds(DELAY);
    digitalWrite(STEP_PIN_RIGHT, LOW); digitalWrite(STEP_PIN_LEFT, LOW); delayMicroseconds(DELAY);  
  }
}
/* Arm Functions */
void reset_arm(){
  lcd_loading_text("RESETING");
  brushServo.write(90);
  upDownServo.write(0);
  delay(1000);
  lcd_text("RESET COMPLETE");
  delay(1000);
}
void brushing(int from, int to){
  lcd_arm_status("DISABLE", 90);
  brushServo.write(from);
  delay(2000);
  lcd_arm_status("ENABLE", 0);
  push_brush(1);
  delay(1000);
  lcd_arm_status("ENABLE", 180);
  brushServo.write(to);
  delay(2000);
  lcd_arm_status("DISABLE", 180);
  push_brush(0);
}
void push_brush(int isDown){
  if(isDown) upDownServo.write(90);
  else upDownServo.write(0);
  delay(1000);
}
/* Keypad Functions */
//void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

char readKeypad(){
  while(1){
    char customKey = kp4x4.getKey();
    if(!digitalRead(RESET_BUT_PIN)){
      return 'E';
    }
//    
    if (customKey) {
      Serial.println(customKey);
      return customKey;
    }
  }
}
String readAnalogKeypad(){
  /* 1: UP 2: LEFT 3: RIGHT 4: DOWN 0: SELECT */
  while(1){
    int val = analogRead(ANALOG_KEY_PIN);
    Serial.println(val);
    if(val <= 20) return "LEFT";
    else if(val <= 30) return "UP";
    else if(val <= 40) return "DOWN";
    else if(val <= 70) return "RIGHT";
    else if(val >= 100 && val <= 200) return "SELECT";
  }
}
/* LCD Functions */
void lcd_welcome_page(){
  lcd.setCursor(6,1);
  lcd.print("SAWADDEE");
  delay(1000);
  lcd.clear();
}
void lcd_text(String text){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(text);
}
void lcd_loading(){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("LOADING");
  for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(byte(0));
    delay(100);
  }
  delay(1000);
  lcd.clear();
}
void lcd_loading_text(String text){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(text);
  for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(byte(0));
    delay(50);
  }
  delay(1000);
  lcd.clear();
}
void lcd_arm_status(String str, int degree){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("BRUSH "+str);
  lcd.setCursor(0,3);
  String cc = "BRUSH AT "+String(degree)+" DEGREE";
  lcd.print(cc);
}
