int led=13;

int crashRgbLedPin[3][3]={{22,24,26},{28,30,32},{34,36,38}};
int crashPin[3]={31,33,35};
int humanDetectPin[3]={7,39,41};
int ultrasonicTrigPin[3]={37,41,45};
int ultrasonicEchoPin[3]={39,43,47};
unsigned  long ultrasonicPreviousTime[3];
int ultrasonicRgbLedPin[3][3]={{40,42,44},{8,9,10},{7,6,5}};
int handDetectCount[3]={0,0,0};
bool handCountFalg[3]={1,1,1};
bool startFlag[3]={0,0,0};//the flag will set true when botton is put in the machine
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 initCrashRgbLedPin();   
 initCrashPin();
 initUltrasonic();
 initUltrasonicLedPin();
}
void loop() {
  for(int i=0;i<3;i++){
    if(crash(i)){
     crashRgbLed(i,1,0,0);
      handDetectCount[i]=0;
    }else{
      crashRgbLed(i,0,1,0);
      startFlag[i]=1;
    }
  }
  for(int i=0;i<3;i++){
    if(millis()-ultrasonicPreviousTime[i]>250){
       ultrasonicPreviousTime[i]= millis();
        long cm=ultrasonic(i);
        if( cm<6){
            if(handCountFalg[i]==1){
                handDetectCount[i]++;
                handCountFalg[i]=0;
            }
        }
        else{
            handCountFalg[i]=1;
        }
    }
    if(startFlag[i]&&crash(i)==0){
      if(handDetectCount[i]==3){
        ultrasonicRgbLed(i,1,1,0);
      }
      else if(handDetectCount[i]>3){
        ultrasonicRgbLed(i,1,0,0);
      }
      else{
        ultrasonicRgbLed(i,0,1,0);
      }
    }
    else{
      ultrasonicRgbLed(i,0,0,0);
    }
     Serial.print("handDetectCount:");
   Serial.println(handDetectCount[0]);
  //delay(200);
  }
}
void initCrashPin(){
  for(int i=0;i<3;i++)
    pinMode(crashPin[i],0);
}
bool crash(int index){
  return digitalRead(crashPin[index]);
}
void initCrashRgbLedPin(){
   for(int i=0;i<3;i++)
     for(int j=0;j<3;j++)
      pinMode(crashRgbLedPin[i][j],1);
}
void crashRgbLed(int index,bool r,bool g,bool b){
  digitalWrite(crashRgbLedPin[index][0],r);
  digitalWrite(crashRgbLedPin[index][1],g);
  digitalWrite(crashRgbLedPin[index][2],b);
}
void initUltrasonicLedPin(){
   for(int i=0;i<3;i++)
     for(int j=0;j<3;j++)
      pinMode(ultrasonicRgbLedPin[i][j],1);
}
void ultrasonicRgbLed(int index,bool r,bool g,bool b){
  digitalWrite(ultrasonicRgbLedPin[index][0],r);
  digitalWrite(ultrasonicRgbLedPin[index][1],g);
  digitalWrite(ultrasonicRgbLedPin[index][2],b);
}
void initUltrasonic(){
 for(int i=0;i<3;i++){
  pinMode(ultrasonicTrigPin[i],1);
  pinMode(ultrasonicEchoPin[i],0);
  ultrasonicPreviousTime[i]=millis();
 }
}
int ultrasonic(int index){
  digitalWrite(ultrasonicTrigPin[index], LOW);
  delayMicroseconds(5);
  digitalWrite(ultrasonicTrigPin[index], HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin[index], LOW);
  pinMode(ultrasonicEchoPin[index], INPUT);             // 讀取 echo 的電位
  long duration = pulseIn(ultrasonicEchoPin[index], HIGH);   // 收到高電位時的時間
  long cm = (duration/2) / 29.1;         // 將時間換算成距離 cm 或 inch
  return cm;
}
