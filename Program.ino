#include <KeyboardFR.h>
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>

File myFile;

const int chipSelect = 10;

const char Button = 21;

const byte ROWS = 2; //four rows
const byte COLS = 4; //four columns
const int MAX_PRINT_SIZE = 127;

char keys[ROWS][COLS] = {
  {'1', '2','3', '4'},
  {'5', '6','7', '8'}
};



byte rowPins[ROWS] = {2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char InitializeSDCard(){
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return -1;
  }
  Serial.println("initialization done.");
  return 0;
}

char WriteFromSDCard(const char FileName[]){
  if (InitializeSDCard() != 0) return -1;
  short int mode = digitalRead(Button);
  
  myFile = SD.open(FileName, FILE_READ);
  
  while (myFile.available() && digitalRead(Button) == mode) {
    Keyboard.write(myFile.read());
    delay(10);
  }
  //close the file:
  myFile.close();
}

char WriteAndEnter(const char Text[]){
    Keyboard.print(Text);
    delay(200);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    return 0;
}

char OpenExe(const char Exe[]){
    // first open run by windows + r
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();

    // adjust delay according to your requierment
    delay(300);

    // write and press enter to open cmd
    WriteAndEnter(Exe);
    return 0;
}



void setup() {  
  Serial.begin(9600);
  
  Keyboard.begin();

  InitializeSDCard();
  
  delay(5000);
}

void loop() {
  
  char key = keypad.getKey();
  short int mode = digitalRead(Button);
  
  if (key != NO_KEY) {
      
    Serial.print("Key pressed = ");
    Serial.println(key);
    Serial.print("Mode = ");
    Serial.println(mode);

    switch(mode){
      case 0:
        switch(key){
          case '1' : //Bouton 1 : Affichage du ping
            WriteFromSDCard("Button1.txt");
            break;
            
          case '2' : 
            WriteFromSDCard("Button2.txt");
            break;

          case '3' :
            WriteFromSDCard("Button3.txt");
            break;
            
          case '4' :
            WriteFromSDCard("Button4.txt");
            break;
            
          case '5' :
            WriteFromSDCard("Button5.txt");
            break;
            
          case '6' :
            WriteFromSDCard("Button6.txt");
            break;
            
          case '7' :
            WriteFromSDCard("Button7.txt");
            break;
        }
        break;
        
      case 1 :
        switch(key){
          case '1' : //Bouton 1 : Affichage du ping
            OpenExe("cmd /k color 0A & ping riot.nl /t");
            break;
            
          case '2' : 
            OpenExe("Taskmgr.exe");
            break;

          case '3' :
            OpenExe("opera.exe");
            break;
            
          case '4' :
            OpenExe("https://open.spotify.com");
            break;
            
          case '5' :
            OpenExe("chrome.exe");
            break;
            
          case '6' :
            OpenExe("chrome.exe");
            break;
            
          case '7' :
            OpenExe("opera.exe");
            break;
            
        }
        break;
    }
  }    
}
