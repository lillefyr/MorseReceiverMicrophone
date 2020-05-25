#include <TimeLib.h>

const int soundPin = A0;

unsigned long theTime = 0;
unsigned long endspace = 0;
unsigned long startspace = 0;
unsigned long signalStart;
unsigned long signalEnd;
unsigned long noiseSignal = 0;
//int long min=65500;
//int long max=0;
int long sum = 0;
int dash=0;
int dot=0;
int space=0;
int samples = 60;
int dotmin;
int dotmax;
int dashmin; //dotmin*3;
int dashmax; // dotmax*3;
int averageTime;

int minsum=1024;
int maxsum=0;

String lettervalue="";

// 20 wpm == 100 tegn pr 60 sec

// 5 wpm  dash 240 dot 80 +/- 10 %
// 15 wpm dash 100 dot 33 +/- 20 %

byte skipSamples = 10;

void getSpeed(){
  sum = 0;
//  Serial.print("start=");Serial.print(millis());
  for (int i=0;i<samples;i++){
    sum += analogRead(soundPin);
  }
  sum = sum / samples;

  if ( skipSamples == 0 ) {
    if ( sum < 900 ) {
      if (signalStart == 0 ) {
        signalStart = millis();
        //Serial.println();
        //Serial.print(sum); Serial.print(" signalStart=");Serial.println(signalStart);
      }
    }
    else
    {
      if ( signalStart > 0 ){
        theTime = millis() - signalStart;
        if ( theTime > 12 ) {
          endspace = signalStart; //debug
          if ((endspace-startspace) < space ) { space = endspace-startspace; }
          
          //Serial.print(sum); Serial.print(" timeNow="); Serial.print(millis());
          if ( theTime < dot )  { dot = ((dot + theTime) / 2); }
          if ( theTime > dash ) { dash = ((dash + theTime) / 2) - 1; }
          if ( dash/3 > dot ) { dot = dash/3; }
          Serial.print(" theTime="); Serial.print(theTime);
          if ( theTime > 99 ) { Serial.print(" dash"); } else { Serial.print("  dot "); }
          Serial.print(" thisspace="); Serial.print(endspace-startspace);
          Serial.print(" space="); Serial.print(space);
          Serial.print(" dash="); Serial.print(dash); Serial.print(" dot="); Serial.println(dot);
          averageTime = (averageTime+theTime)/2;
          startspace =  millis();
        }
        signalStart = 0;
      }
    }
  }
  else
  {
    skipSamples --;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("MorseReceiverMicrophone");

  // skip spurious
  dot = 400;
  dash = 0;
  averageTime = 0;
  int i;
  for (i=0; i < 10; i++) {
    getSpeed();
    yield();
  }

  startspace =  millis();
  signalStart = 0;
  signalEnd = 0;

  Serial.print("averageTime="); Serial.println(averageTime);
  dot = 160;
  dash = 40;
  space = 1000;
  Serial.println("Calibrating (60000 samples)");
  Serial.println(millis());
  for (i=0; i < 3000; i++) {
    getSpeed();
    yield();
  }
  Serial.println(millis());
  // define range
  dotmin = dot-15;
  dotmax = dot+15;
  dashmin = dash-30;
  dashmax = dash+30;
  space = space-1;

  Serial.println("====================================================================");
  Serial.print("dash="); Serial.print(dash);
  Serial.print(" dashmax="); Serial.print(dashmax);
  Serial.print(" dashmin="); Serial.println(dashmin);
  
  Serial.print("dot="); Serial.print(dot);
  Serial.print(" dotmax="); Serial.print(dotmax);
  Serial.print(" dotmin="); Serial.println(dotmin);
  
  Serial.print("space="); Serial.print(space);
  
  Serial.print(" averageTime="); Serial.println(averageTime);
  Serial.println("====================================================================");
}


char decodeMorse(String lettervalue) {
  
  //Serial.println(lettervalue);
  if (lettervalue == "....") { return 'H'; }
  if (lettervalue == "...-") { return 'V'; }
  if (lettervalue == "...")  { return 'S'; }
  if (lettervalue == "..-.") { return 'F'; }
  if (lettervalue == "..-")  { return 'U'; }
  if (lettervalue == "..")   { return 'I'; }
  if (lettervalue == ".-..") { return 'L'; }
  if (lettervalue == ".-.")  { return 'R'; }
  if (lettervalue == ".--.") { return 'P'; }
  if (lettervalue == ".---") { return 'J'; }
  if (lettervalue == ".--")  { return 'W'; }
  if (lettervalue == ".-")   { return 'A'; }
  if (lettervalue == ".")    { return 'E'; }

  if (lettervalue == "-...") { return 'B'; }
  if (lettervalue == "-..-") { return 'X'; }
  if (lettervalue == "-..")  { return 'D'; }
  if (lettervalue == "-.-.") { return 'C'; }
  if (lettervalue == "-.--") { return 'Y'; }
  if (lettervalue == "-.-")  { return 'K'; }
  if (lettervalue == "-.")   { return 'N'; }
  if (lettervalue == "--..") { return 'Z'; }
  if (lettervalue == "--.-") { return 'Q'; }
  if (lettervalue == "--.")  { return 'G'; }
  if (lettervalue == "---")  { return 'O'; }
  if (lettervalue == "--")   { return 'M'; }
  if (lettervalue == "-")    { return 'T'; }

  if (lettervalue == ".----")  { return '1'; }
  if (lettervalue == "..---")  { return '2'; }
  if (lettervalue == "...--")  { return '3'; }
  if (lettervalue == "....-")  { return '4'; }
  if (lettervalue == ".....")  { return '5'; }
  if (lettervalue == "-....")  { return '6'; }
  if (lettervalue == "--...")  { return '7'; }
  if (lettervalue == "---..")  { return '8'; }
  if (lettervalue == "----.")  { return '9'; }
  if (lettervalue == "-----")  { return '0'; }

  if (lettervalue == ".-.-.-")  { return '.'; }
  if (lettervalue == "--..--")  { return ','; }
  if (lettervalue == "..--..")  { return '?'; }
  if (lettervalue == ".----.")  { return '\''; }
  if (lettervalue == "-.-.--")  { return '!'; }
  if (lettervalue == "-..-.")   { return '\\'; }
  if (lettervalue == "-.--.")   { return '('; }
  if (lettervalue == "-.--.-")  { return ')'; }
  if (lettervalue == ".-...")   { return '&'; }
  if (lettervalue == "---...")  { return ':'; }
  if (lettervalue == "-.-.-.")  { return ';'; }
  if (lettervalue == "-...-")   { return '='; }
  if (lettervalue == ".-.-.")   { return '+'; }
  if (lettervalue == "-....-")  { return '-'; }
  if (lettervalue == "..--.-")  { return '_'; }
  if (lettervalue == ".-..-.")  { return '"'; }
  if (lettervalue == "...-..-") { return '$'; }
  if (lettervalue == ".--.-.")  { return '@'; }

  return '*';
}

void loop(){
  sum = 0;
  for (int i=0;i<samples;i++){
    sum += analogRead(soundPin);
  }
  sum = sum / samples;

  if (( millis() - signalEnd ) > space*6 ) {
    if ( lettervalue.length() > 0) {
      Serial.print( millis() - signalEnd);
      Serial.print(" ");
      Serial.print(lettervalue);
      Serial.print(" ");
      Serial.print(decodeMorse(lettervalue));
      lettervalue="";
    }
  }

  if ( sum < 900 ) {
    if (signalStart == 0 ) {
      signalStart = millis();
      if ((signalStart-signalEnd) >= dash*3) {
//        Serial.print( millis() - signalEnd);
        Serial.print(" ");
      }
    }
  }
  else
  {
    if ( signalStart > 0 ){
      signalEnd = millis();
      
//      Serial.print(" signalEnd=");Serial.print(signalEnd);
      theTime = signalEnd - signalStart;

      if ((theTime >= dashmin) && (theTime <= dashmax)) {
//        Serial.print(theTime);Serial.print(" ");Serial.println(" - "); 
        lettervalue=lettervalue+'-';
      }else{
        if (( theTime >= dotmin) && (theTime <= dotmax)) {
//          Serial.print(theTime);Serial.print("=");Serial.println(" . ");
          lettervalue=lettervalue+'.';
        }
      }
      //Serial.println();
      signalStart = 0;
    }
  }
}
