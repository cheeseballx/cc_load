#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    -1

//Screen Values
#define TextStart 20
#define Line1 16
#define Line2 40
#define CircleRad 6


//Rotary Stuff
#define encoderPinA 2
#define encoderPinB 3

#define encoderBtn 7
#define backBtn 8


// for rotating
volatile boolean A_set = false;            
volatile boolean B_set = false;
volatile int encPos = 0;
volatile int lastEncPos = 0;

//the buttons are PulledUp so set them high
int encoderVal = 1;
int lastEncoderVal = 1;

int backVal = 1;
int lastBackVal = 1;

//Screen
volatile int screen = 0;
volatile int screenpos = 0;

//Our main Variables
int set_current=1120;
int is_current=1221;
int is_voltage=1200;


//The Screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {

  //THE BUTTONS
  pinMode(encoderBtn,INPUT_PULLUP);
  pinMode(backBtn,INPUT_PULLUP);
  
  //THE ROTATRY PART
  pinMode(encoderPinA, INPUT_PULLUP); 
  pinMode(encoderPinB, INPUT_PULLUP); 
  
  //THE INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE); // encoder pin on interrupt 1 (pin 3)
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;); // Don't proceed, loop forever
  }

  //diplay init
  display.setTextColor(SSD1306_WHITE);
  display.display();
  delay(1);
  display.clearDisplay();

  // Screen Main start
  //Screen_Main();
  Screen_setCurrent();
}

void loop()
{ 
  encoderVal = digitalRead(encoderBtn);
  backVal = digitalRead(backBtn);
  
  if (encoderVal != lastEncoderVal){
    lastEncoderVal = encoderVal;
    if (encoderVal == 0)
      ok();
  }
  
  if (backVal != lastBackVal) {
    lastBackVal = backVal;
    if (backVal == 0)
      back();
  }

  if (encPos != lastEncPos){
    Serial.println("IND: " + encPos);
    if(encPos > lastEncPos) higher();
    else lower();
    lastEncPos = encPos;
  }
}

void Screen_Main() {
  
  display.clearDisplay();
  display.setTextSize(2);
  
  display.drawCircle(CircleRad + 1, ((screenpos==0) ? Line1 : Line2 )+ CircleRad, CircleRad, SSD1306_WHITE);
  
  display.setCursor(TextStart,Line1);
  display.print("C-Current");
  
  display.setCursor(TextStart,Line2);
  display.print("C-Power");

  display.display();
  delay(1);

}

void Screen_setCurrent() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(TextStart - CircleRad * 2,Line1);
  display.print("Set: ");
  
  display.setTextSize(2);
  display.print(set_current);
  display.setTextSize(1);
  display.print("mA");

  display.setTextSize(2);
  display.setCursor(0,Line2);
  display.print(is_voltage);
  display.setTextSize(1);
  display.print("mV");
  display.print(" ");
  display.setTextSize(2);
  display.print(is_current);
  display.setTextSize(1);
  display.print("mA");

  display.display();
  delay(1);
}


//Iterrupt on A changing state
void doEncoderA(){
  if(B_set){
    B_set = false;
    A_set = false;
    encPos-=1;
  }
  else{
    A_set = true;
    B_set = false;
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if(A_set){
    B_set = false;
    A_set = false;
    encPos+=1;
  }
  else{
    A_set = false;
    B_set = true;
  }
}

//THE ACTIONS
void ok(){
  switch(screen){
     case 0: 
             if (screenpos == 0)
                Screen_setCurrent();
             break;
  }
  
}

void back(){
  
}

void higher(){
  
  switch(screen){
     case 0: 
             screenpos = (screenpos + 1) % 2;
             Screen_Main();
             break;
  }

}

void lower(){
  switch(screen){
     case 0:
            screenpos = (screenpos -1) % 2;
            Screen_Main();
            break;
  }
}
