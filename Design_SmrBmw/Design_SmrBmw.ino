#include <TimerOne.h>

#define head1Ldr1 A0
#define head1Ldr2 A1
#define head1Ldr3 A2
#define head2Ldr1 A3
#define head2Ldr2 A4
#define head2Ldr3 A5

#define ledReady1 2
#define ledRunning1 3
#define ledFail1 4
#define ledPass1 5
#define ledReady2 7
#define ledRunning2 8
#define ledFail2 9
#define ledPass2 13

#define limitSwitch1 6
#define limitSwitch2 11

#define relayModule1 12
#define relayModule2 10

String inputString;
int rxTimeOut;
bool stringComplete;
int Timer100mS;

void setup() {
  SetPinMode();
  SetSerial();
  SetTimer();
  inputString.reserve(200);
}

void loop() {
  if (inputString != "") {
    if (rxTimeOut >= 5) { //Set Rx receive time out to 0.5 sec.
      inputString = "";
      Serial.println(F("Time out reach, Data received not include new line character"));
    }
  }

  if (stringComplete) {
    inputString.toLowerCase();
    inputString.replace(" ", "");


    if(inputString.substring(0, 4).equalsIgnoreCase("Help")){
      Help();
    }
    else if(inputString.substring(0, 8).equalsIgnoreCase("GetReady")){
      Serial.println("Ok");
    }
    else if(inputString.substring(0, 14).equalsIgnoreCase("GetLimitSwitch")){
      GetLimitSwitch();
    }
    else if(inputString.substring(0, 9).equalsIgnoreCase("LedStatus")){
      LedStatus(inputString);
    }
    else if(inputString.substring(0, 7).equalsIgnoreCase("ReadLdr")){
      ReadLdr(inputString);
    } 
    else if(inputString.substring(0, 7).equalsIgnoreCase("Connect")){
      Connect(inputString);
    } 
    else if(inputString.substring(0, 10).equalsIgnoreCase("DisConnect")){
      DisConnect(inputString);
    } 
    else{
      Serial.println(F("Cmd incorrect format"));
      Serial.println(inputString);
    }

    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  rxTimeOut = 0;
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void DisConnect(String data){
  String head = SeparateStringByCharacter(data,',',1);
  if(head == "1\n"){
    digitalWrite(relayModule1, HIGH);
  }
  else{
    digitalWrite(relayModule2, HIGH);
  }
  Serial.println("Ok");
}
void Connect(String data){
  String head = SeparateStringByCharacter(data,',',1);
  if(head == "1\n"){
    digitalWrite(relayModule1, LOW);
  }
  else{
    digitalWrite(relayModule2, LOW);
  }
  Serial.println("Ok");
}
void ReadLdr(String data){
  String head = SeparateStringByCharacter(data,',',1);
  String ledNo = SeparateStringByCharacter(data,',',2);
  int result;
  if(head == "1"){
    if(ledNo == "1\n"){
      result = analogRead(head1Ldr1);
    }else if("2\n"){
      result = analogRead(head1Ldr2);
    }else{
      result = analogRead(head1Ldr3);
    }
  }
  else{
    if(ledNo == "1\n"){
      result = analogRead(head2Ldr1);
    }else if("2\n"){
      result = analogRead(head2Ldr2);
    }else{
      result = analogRead(head2Ldr3);
    }
  }
  Serial.println(String(result));
}
void LedStatus(String data){
  String head = SeparateStringByCharacter(data,',',1);
  String status_ = SeparateStringByCharacter(data,',',2);
  if(head == "1"){
    digitalWrite(ledReady1, LOW);
    digitalWrite(ledRunning1, LOW);
    digitalWrite(ledFail1, LOW);
    digitalWrite(ledPass1, LOW);
    if(status_ == "ready\n"){
      digitalWrite(ledReady1, HIGH);
    }
    else if(status_ == "running\n"){
      digitalWrite(ledRunning1, HIGH);
    }
    else if(status_ == "fail\n"){
      digitalWrite(ledFail1, HIGH);
    }
    else{
      digitalWrite(ledPass1, HIGH);
    }
  }
  else{
    digitalWrite(ledReady2, LOW);
    digitalWrite(ledRunning2, LOW);
    digitalWrite(ledFail2, LOW);
    digitalWrite(ledPass2, LOW);
    if(status_ == "ready\n"){
      digitalWrite(ledReady2, HIGH);
    }
    else if(status_ == "running\n"){
      digitalWrite(ledRunning2, HIGH);
    }
    else if(status_ == "fail\n"){
      digitalWrite(ledFail2, HIGH);
    }
    else{
      digitalWrite(ledPass2, HIGH);
    }
  }
  Serial.println("Ok");
  Serial.println(status_);
}
void GetLimitSwitch(){
  bool head1 = digitalRead(limitSwitch1);
  bool head2 = digitalRead(limitSwitch2);
  String result = String(head1) + "," + String(head2); 
  Serial.println(result);
}
void Help(){
  Serial.println(F("//1. Help = Show avialable Cmd and Guid line"));
  Serial.println(F("//2. GetReady = Return Ok"));
  Serial.println(F("//3. GetLimitSwitch = StatusHead1,StatusHead2"));
  Serial.println(F("//4. LedStatus,[Head],[Status]"));
  Serial.println(F("//    - Head = 1 is head 1, 2 is head 2"));
  Serial.println(F("//    - Status = Ready, Running, Fail, Pass"));
  Serial.println(F("//5. ReadLdr,[Head]"));
  Serial.println(F("//    - Head = 1 is head 1, 2 is head 2"));
  Serial.println(F("//6. Connect,[Head]"));
  Serial.println(F("//    - Head = 1 is head 1, 2 is head 2"));
  Serial.println(F("//7. DisConnect,[Head]"));
  Serial.println(F("//    - Head = 1 is head 1, 2 is head 2"));
}
void SetTimer(){
  Timer1.initialize(100000);
  Timer1.attachInterrupt( timerIsr );
}
void SetSerial(){
  Serial.begin(9600);
  Serial.println(F("//================ Welcom to Smart Tester ===============//"));
}
void SetPinMode(){
  pinMode(head1Ldr1, INPUT);
  pinMode(head1Ldr2, INPUT);
  pinMode(head1Ldr3, INPUT);
  pinMode(head2Ldr1, INPUT);
  pinMode(head2Ldr2, INPUT);
  pinMode(head2Ldr3, INPUT);

  pinMode(ledReady1, OUTPUT);
  pinMode(ledRunning1, OUTPUT);
  pinMode(ledFail1, OUTPUT);
  pinMode(ledPass1, OUTPUT);
  pinMode(ledReady2, OUTPUT);
  pinMode(ledRunning2, OUTPUT);
  pinMode(ledFail2, OUTPUT);
  pinMode(ledPass2, OUTPUT);
  digitalWrite(ledPass2, LOW);
  digitalWrite(ledReady1, HIGH);
  digitalWrite(ledReady2, HIGH);

  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch2, INPUT);

  pinMode(relayModule1, OUTPUT);
  pinMode(relayModule2, OUTPUT);
  digitalWrite(relayModule1, HIGH);
  digitalWrite(relayModule2, HIGH);
}
String SeparateStringByCharacter(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void timerIsr() {
  rxTimeOut += 1;
  Timer100mS += 1;
}
