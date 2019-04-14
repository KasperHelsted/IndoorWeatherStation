#include <EEPROM.h>

String EEPString = "";
void writeString(String data)
{
  EEPString = data;

  int _size = data.length();
  for(int i=0; i < _size; i++)
  {
    EEPROM.write(i, data[i]);
  }
  EEPROM.commit();
}

bool readString(int memSize)
{
  int i;
  String data;

  for(int i = 0; i < memSize; i++)
  {
    char k = EEPROM.read(i);

    if(k == '\0') return false;
    
    data = data + k;   
  }
  
  EEPString = String(data);
  return true;
}

String letters[]= {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
String idBase = "HAS";
int IDLength = 6;
String random_String(int length=-1){
  if(length == -1)
    length = IDLength;
  String randString = idBase;
  
  for(int i = 0; i < length; i++)
  {
    randString = randString + letters[random(0, 36)];
  }
  
  return randString;
}

void loadUniqueID(){
  EEPROM.begin(512);

  if(!readString(idBase.length() + IDLength))
    writeString(random_String());
  if(EEPString.substring(0, idBase.length()) != idBase)
      writeString(random_String());
}
