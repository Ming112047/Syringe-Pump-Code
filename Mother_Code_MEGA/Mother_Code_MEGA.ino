/////////////////////////Function declare//////////////////////////

  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Keypad.h>
//  #include "SSD1306Ascii.h"
//  #include "SSD1306AsciiAvrI2c.h"

  double DPType(char a , float d);
  double INTType (char a, long int b);
  double DoRoll (long int a, double b, int Direction);
  
/////////////////////////Display Setting///////////////////////////

  #define SCREEN_WIDTH 128 // pixel ความกว้าง
  #define SCREEN_HEIGHT 64 // pixel ความสูง 

  // กำหนดขาต่อ I2C กับจอ OLED
  #define OLED_RESET -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
  Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  //Adafruit_SSD1306 display(128, 64, &Wire, -1);

//-------------------------------------------------------------------

  // 0X3C+SA0 - 0x3C or 0x3D
//  #define I2C_ADDRESS 0x3C

  // Define proper RST_PIN if required.
//  #define RST_PIN -1

//SSD1306AsciiAvrI2c oled;
  
/////////////////////////KeyPad Setting///////////////////////////

  const byte ROWS = 4; //four rows
  const byte COLS = 4; //four columns
  char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
  };
  byte rowPins[ROWS] = {45, 43, 41, 39}; //กำหนด Pin
  byte colPins[COLS] = {53, 51, 49, 47};
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

////////////////////Variables///////////////////////////////////////

char key;

float Tnum, DivSta; 

double ASize=0, ASizeI=0, ASizeD=0, AFlowrate=0 , AFlowrateD=0 , AFlowrateI=0 , AVol=0 , AVolD=0 , AVolI=0,
       DivStat=10, TimeDelayA=0, MotorStepA=0, STIME=0, ETIME=0, TimeStepDelay=0, DataCalibration=0;

int DecimalPointStat=0, EnterDataState=1, i=0;

//////////////////pin setup/////////////////////////////////////////

const int stepPin1 = 48, 
          dirPin1  = 50, 
          enPin1   = 52;


///////////////////////////////////////////////////////////////////

void setup() {
  
    pinMode(stepPin1,OUTPUT); 
    pinMode(dirPin1,OUTPUT);
    pinMode(enPin1,OUTPUT);
    digitalWrite(enPin1,LOW);

///////////////////////Display Setup///////////////////////////////

    Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  
/*  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0
    // Call oled.setI2cClock(frequency) to change from the default frequency.

    oled.setFont(Adafruit5x7);

    uint32_t m = micros();
    oled.clear();
    oled.print("Di.Sy. (mm) : ");oled.println(ASize, 1);
    oled.print("FR (ml/s)   : ");oled.println(AFlowrate , 4);
    oled.print("Volume (ml) : ");oled.println(AVol , 2);*/
}

void loop() {
      
    key = keypad.getKey();
  
  if (EnterDataState <= 3){
    
    if(EnterDataState==1 && key!=NO_KEY){ //syringe size

      switch(DecimalPointStat){
        case 0 : ASizeI = INTType(key, ASizeI);break; //get Syringe size (ml)
        case 1 : ASizeD = DPType(key, ASizeD);break;
      }key=NO_KEY;
      ASize=ASizeI+ASizeD;
    }

    if(EnterDataState==2 && key!=NO_KEY){ //flow rate      
      switch(DecimalPointStat){
        case 0 : AFlowrateI = INTType(key, AFlowrateI);break; //get liquid flow rate (ml/min)
        case 1 : AFlowrateD = DPType(key, AFlowrateD);break;
      }key=NO_KEY;
     AFlowrate = AFlowrateI + AFlowrateD;
    }

    if(EnterDataState==3 && key!=NO_KEY){ //flow rate      
      switch(DecimalPointStat){
        case 0 : AVolI = INTType(key, AVolI);break; //get liquid flow rate (ml/min)
        case 1 : AVolD = DPType(key, AVolD);break;
      }key=NO_KEY;
      AVol=AVolI+AVolD; 
    }

    
      OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
      OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
      OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
      OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
    // if EnterDataState <= 3 show VVV
      OLED.print("Di.Sy. (mm) : ");OLED.println(ASize , 1); //Syringe Diameter
      OLED.print("FR (ml/s)   : ");OLED.println(AFlowrate , 4); //flow Rate
      OLED.print("Volume (ml) : ");OLED.println(AVol , 2); //Max flowing Vloume
      OLED.display(); // สั่งให้จอแสดงผล
      
  }
    
    
    if(EnterDataState == 4){    
           
            ////          calculation            ////

      TimeDelayA = AVol / AFlowrate;
      DataCalibration = 1 - ( ( ( 0.3033 * ( AFlowrate * AFlowrate ) ) + ( 0.0127 * AFlowrate ) + 0.001 ) / AFlowrate ) ;
      MotorStepA = ( AVol * 64000 ) / ( 3.141592653 * ( ASize / 20 ) * ( ASize / 20 ) );
      
      if ( AFlowrate > 0.01 ) {
        TimeStepDelay = ( ( 1000000.00 * ( TimeDelayA * DataCalibration ) ) / ( 2 * MotorStepA ) );// - ( AVol / ( 2 * MotorStepA * DataCalibration ) ) ;
      }
      if (AFlowrate <= 0.05 ) {
       TimeStepDelay = ( ( 1000000.00 * ( TimeDelayA - ( ( 1 - DataCalibration ) * AFlowrate ) ) ) / ( 2 * MotorStepA ) );
      }
      
            ////        Screen settung         ////

      OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
      OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
      OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
      OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
      OLED.println("Press # to start     or D to edit value");
      OLED.print("Run Time : ");OLED.print(TimeDelayA, 4);OLED.println(" s");
      OLED.println(" ");
      OLED.print("Step : ");OLED.print(MotorStepA, 0);OLED.println(" step");
      OLED.println(" ");
      OLED.print("step delay : ");OLED.print(TimeStepDelay, 0);OLED.println(" us");
      OLED.display();
                  
            if(key == '#'){

              EnterDataState++;
              key=NO_KEY;
            }
            if(key == 'D'){

              EnterDataState = 1;
              key=NO_KEY;
            }
    }

    if(EnterDataState == 5){
      
        OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
        OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
        OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
        OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
        OLED.println("Operating");
        OLED.println(" ");
        OLED.println("Time Estimation : ");OLED.print(TimeDelayA);OLED.println(" s");
        OLED.display();
        
        DoRoll(MotorStepA,TimeStepDelay,0);//0=forward, 1=backward
    }
      
    if(EnterDataState == 6){

        OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
        OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
        OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
        OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
        OLED.println("Time");
        OLED.print("Exper  : ");OLED.print(ETIME-STIME, 0);OLED.println(" us");
        OLED.print("Theory : ");OLED.print(TimeDelayA*1000000, 0);OLED.println(" us");
        OLED.print("Error  : ");OLED.print((((ETIME-STIME)/(TimeDelayA*1000000))*100)-100, 2);OLED.println(" %");
        OLED.println("edit value : A");
        OLED.println("Continue -> : B");
        OLED.println("Return <- : C");
        OLED.display();
        
       if(key == 'B'){

              EnterDataState = 5;
              key=NO_KEY;
        
        }
       if(key == 'A'){
        
              EnterDataState = 1;
              key=NO_KEY;
              
              }
       if(key == 'C'){

              DoRoll(MotorStepA,TimeStepDelay,1);//0=forward, 1=backward
              EnterDataState = 6;key=NO_KEY;
        
        }

       
      
              /*if(i>>2){
        
              OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
              OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
              OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
              OLED.setTextSize(3); // กำหนดขนาดตัวอักษร
              OLED.println("Error  Process");
              
              OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
              OLED.println("please Reset");

              OLED.display();
              }
              
              OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
              OLED.setTextColor(WHITE, BLACK); //กำหนดข้อความสีขาว ฉากหลังสีดำ
              OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
              OLED.setTextSize(1); // กำหนดขนาดตัวอักษร
              OLED.println("Time Estimation : ");OLED.print(TimeDelayA);OLED.println(" s");
              OLED.display();*/
              

              ////////////////////////////////////////////////////////////////////////////
    }
  }          
