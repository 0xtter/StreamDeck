#include <KeyboardFR.h>


void setup() {
  Keyboard.begin();
  delay(2000);
  Keyboard.println("Bonjour !");
}

/*
 * Bonjour !
 * #include<stdlib.h>
 * Bonjour !
 * #include<stdlib.h>
 * #include<stdlib.h>
 * 0
 * 
 */
void loop() {
  Keyboard.println("#include<stdlib.h>");
  delay(5000);
}
