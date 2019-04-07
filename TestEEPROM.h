/*
 * Circuits4you.com
 * Reading and Writing String to EEPROM Example Code
 * Oct 2018
 */

#include <EEPROM.h>

void writeString(String data)
{
  Serial.print("writeString: ");
  Serial.println(data);

  int _size = data.length();
  for(int i=0; i < _size; i++)
  {
    EEPROM.write(i, data[i]);
  }
  EEPROM.commit();
}

String EEPString = "";
bool read_String(int memSize)
{
  int i;
  String data;

  for(int i = 0; i < memSize; i++)
  {
    char k = EEPROM.read(i);
    if(k == '\0'){
      return false;
    }
    data = data + k;   
  }
  
  EEPString = String(data);
  return true;
}

String letters[]= {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
String random_String(int length=6){
  String randString = "HAS";
  
  for(int i = 0; i < length; i++)
  {
    randString = randString + letters[random(0, 36)];
  }
  
  return randString;
}
