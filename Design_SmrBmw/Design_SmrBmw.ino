#include <TimerOne.h>

#pragma region ======================================= Input =======================================
//Pin ldr 1 of header 1
#define head1Ldr1 A0
//Pin ldr 2 of header 1
#define head1Ldr2 A1
//Pin ldr 3 of header 1
#define head1Ldr3 A2
//Pin ldr 1 of header 2
#define head2Ldr1 A3
//Pin ldr 2 of header 2
#define head2Ldr2 A4
//Pin ldr 3 of header 2
#define head2Ldr3 A5
//Pin led status ready of header 1
#define ledReady1 2
//Pin led status running of header 1
#define ledRunning1 3
//Pin led status fail of header 1
#define ledFail1 4
//Pin led status pass of header 1
#define ledPass1 5
//Pin led status ready of header 2
#define ledReady2 7
//Pin led status running of header 2
#define ledRunning2 8
//Pin led status fail of header 2
#define ledFail2 9
//Pin led status pass of header 2
#define ledPass2 13
//Pin input limit switch for check put unit header 1
#define limitSwitch1 6
//Pin input limit switch for check put unit header 2
#define limitSwitch2 11
//Pin select on power to unit header 1
#define relayModule1 12
//Pin select on power to unit header 2
#define relayModule2 10
/// @brief For data input serial port
String inputString;
/// @brief Timeout of input rx serial port
int rxTimeOut;
/// @brief Flag read input complete
bool stringComplete;
/// @brief Timer 100 millisec for blink led status
int Timer100mS;
#pragma endregion

#pragma region ======================================= DisPlay =======================================
/// @brief Display all command
void Help(){
  Serial.println(F("****************************************************************"));
  Serial.println(F("* 1. GetReady                                                  *"));
  Serial.println(F("*  Check arduino ready                                         *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  OK                                                          *"));
  Serial.println(F("* 2. GetLimitSwitch                                            *"));
  Serial.println(F("*  Get status limit switch for check put unit                  *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  StatusHead1,StatusHead2                                     *"));
  Serial.println(F("* 3. LedStatus,[Head],[Status]                                 *"));
  Serial.println(F("*  Set led on follow status                                    *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  Head - is Header of machine [1, 2]                          *"));
  Serial.println(F("*  Status - is Status header [Ready, Running, Fail, Pass]      *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  OK                                                          *"));
  Serial.println(F("*  [Status]                                                    *"));
  Serial.println(F("* 4. ReadLdr,[Head],[LedNo]                                    *"));
  Serial.println(F("*  Read value ldr                                              *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  Head - is Header of machine [1, 2]                          *"));
  Serial.println(F("*  LedNo - is number of led of header [1, 2, 3]                *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  Value ldr from analogRead                                   *"));
  Serial.println(F("* 5. Connect,[Head]                                            *"));
  Serial.println(F("*  Select on power to unit                                     *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  Head - is Header of machine [1, 2]                          *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  OK                                                          *"));
  Serial.println(F("* 6. DisConnect,[Head]                                         *"));
  Serial.println(F("*  Off power unit                                              *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  Head - is Header of machine [1, 2]                          *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  OK                                                          *"));
  Serial.println(F("* 7. Help                                                      *"));
  Serial.println(F("*  Show all command                                            *"));
  Serial.println(F("* Return:                                                      *"));
  Serial.println(F("*  [All Command]                                               *"));
  Serial.println(F("****************************************************************"));
}
/// @brief Display Welcom start arduino
void SetSerial(){
  Serial.begin(9600);
  Serial.println(F("//================ Welcom to Smart Tester ===============//"));
}
#pragma endregion

#pragma region ======================================= Cal =======================================
/// @brief Event input serial port
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
/// @brief Off power unit
/// @param data is Data select head from cmd
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
/// @brief On power to unit
/// @param data is Data select head from cmd
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
/// @brief Read value ldr
/// @param data is Data select header and numbre of ldr
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
/// @brief Set status led
/// @param data is Data select header and status
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
/// @brief Get status put unit from limit switch
void GetLimitSwitch(){
  bool head1 = digitalRead(limitSwitch1);
  bool head2 = digitalRead(limitSwitch2);
  String result = String(head1) + "," + String(head2); 
  Serial.println(result);
}
/// @brief Setup timer timeout read serial port
void SetTimer(){
  Timer1.initialize(100000);
  Timer1.attachInterrupt( timerIsr );
}
/// @brief Setup pinout of arduino
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
/// @brief Function split string
/// @param data is All data
/// @param separator is Char split
/// @param index is Index of string split at select
/// @return String split follow index
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
/// @brief Event tric of timer
void timerIsr() {
  rxTimeOut += 1;
  Timer100mS += 1;
}
#pragma endregion

#pragma region ======================================= Main =======================================
/// @brief Void setup of arduino
void setup() {
  SetPinMode();
  SetSerial();
  SetTimer();
  inputString.reserve(200);
}
/// @brief Void loop of arduino
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
#pragma endregion
