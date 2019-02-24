/*
* MineControl v1.1
* Made by Big-Cheeri-Os
* At M.I.T. Blueprint 2019
* 
* February 24th, 2019
* 
* PINS
* d0  - No wire (needed for TX & RX)
* d1  - No wire (needed for TX & RX) 
* d2  - Button 1 (Walk)
* d3  - Button 2 (Jump)
* d4  - Screen D4
* d5  - Screen D5
* d6  - Screen D6
* d7  - Screen D7
* d8  - Screen RS
* d9  - Screen EN
* d10 - 
* d11 - Button 3 (Scroll)
* d12 - Button 4 (Destroy)
* d13 - Button 5 (Place)
* a0  - 
* a1  - 
* a2  - 
* a3  - 
* a4  - Joystick Y
* a5  - Joystick X
*/

/*Declare Variables*/
int jump = 3;
int walk = 2;
int scroll = 11;
int destroy = 12;
int place = 13;
int invPlace = 1;
bool jumpBool = false, walkBool = false, scrollBool = false, destroyBool = false, placeBool = false;

/*Include LiquidCrystal library for the LCD screen*/
#include <LiquidCrystal.h>

/*Screen initiation*/
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*Functions to change variables when a button is pressed/held*/
void doJump(){
  jumpBool = !jumpBool;
  if(!jumpBool){
    Serial.println("Jump!");
  }
}

void doWalk(){
  walkBool = !walkBool;
  Serial.println("Walking: "+ String(walkBool));
}

void doScroll(){
  scrollBool = !scrollBool;
  if(!scrollBool){
    invPlace++;
    if(invPlace > 9){
      invPlace = 1;
    }
    Serial.println("ChangedInventory: " + String(invPlace));
  }
}

void doDestroy(){
  destroyBool = !destroyBool;
  Serial.println("Destroy: "+ String(destroyBool));
}

void doPlace(){
  placeBool = !placeBool;
  if(!placeBool){
    Serial.println("Place!");
  }
}

void displayStatus(){
  String txt = "";
  if(jumpBool){
    txt = "Jumping";
  }
  if(scrollBool){
    txt = "Scrolling";
  }
  if(placeBool){
    txt = "Placing";
  }
  if(destroyBool){
    txt += " and\nHitting";
  }
  if(walkBool){
    txt += " and\nWalking";
  }

  if(txt != "" && (millis() % 100 == 0)){
    printToScreen(txt);
  } else if(millis() % 100 == 0){
    lcd.clear();
  }
}

/*General init and declarations*/
void setup() {
  /*LCD Startup*/
  lcd.begin(16, 2);
  printToScreen("MineControl\nVersion 1.1");
  delay(5000);
  printToScreen("Starting setup...");
  printToScreen("Initializing\nvariables...");

  /*Setting pinModes to input*/  
  pinMode(jump, INPUT);
  pinMode(walk, INPUT);
  pinMode(scroll, INPUT);
  pinMode(destroy, INPUT);
  pinMode(place, INPUT);

  /*Attaching interrupts for the pins for jumping and walking*/
  attachInterrupt(digitalPinToInterrupt(jump), doJump, CHANGE);
  attachInterrupt(digitalPinToInterrupt(walk), doWalk, CHANGE);

  /*Opening serial port*/
  printToScreen("Setting up serial port...");
  Serial.begin(9600);

  printToScreen("Done with setup...\nRunning now...");
}

/*Loop*/
void loop() {

//  printToScreen("testing...");
  
  /*Special code for looping functions -- scroll, destroy, place*/
  if((digitalRead(scroll)&&scrollBool == false)||(!digitalRead(scroll)&&scrollBool == true)){
    doScroll();
  }
  if((digitalRead(destroy)&&destroyBool == false)||(!digitalRead(destroy)&&destroyBool == true)){
    doDestroy();
  }
  if((digitalRead(place)&&placeBool == false)||(!digitalRead(place)&&placeBool == true)){
    doPlace();
  }

  displayStatus();

  /*Joystick movement detection and serial output*/
  int x = analogRead(A5);
  int y = analogRead(A4);

  int xAvg = 513, yAvg = 498;
  
  float xSpeed = (float) (x - xAvg) / 512;
  float ySpeed = (float) (y - yAvg) / 512 - 0.03;

  if(x > (xAvg + 20) || x < (xAvg - 20)){
    Serial.println("xS: " + String(xSpeed));
  }
  if(y > (yAvg + 20) || y < (yAvg - 20)){
    Serial.println("yS: " + String(ySpeed));
  }

  /*Some delay before next loop begins*/
  delay(1);
}



/*Printing text to screen*/
void printToScreen(String txt){
  lcd.clear();
  int newLine = txt.indexOf("\n");
  lcd.setCursor(0, 0);
  lcd.print(txt.substring(0, newLine));
  if(newLine != -1){
    lcd.setCursor(0, 1);
    lcd.print(txt.substring(newLine + 1));
  }
  
}
