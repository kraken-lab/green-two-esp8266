/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <Arduino.h>
#include "LittleFS.h"

bool print = true;

void setup() {
  Serial.begin(115200);
}
 
void printFileSystemContent() {
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  File file = LittleFS.open("/text.txt", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("\n\n\nFile Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void loop() {  
  if (print)
  {
    delay(1000);
    Serial.write("1");
    delay(1000);
    Serial.write("2");
    delay(1000);
    Serial.write("3");
    delay(1000);
    Serial.write("4");
    delay(1000);
    Serial.write("5");

    printFileSystemContent();
    print = false;
  }
}