int echo = 3;
int trig = 2;
unsigned long duration;
float distance;

void setup() {

    Serial.begin(9600);                //   시리얼 셋팅
    pinMode(echo, INPUT);       //   초음파 input  설정
    pinMode(trig, OUTPUT);      //  초음파 out  설정
    while (!Serial) {
   }

   Serial.println("-------start-----------");
   Serial.println("CLEARDATA");
   Serial.println("LABEL,Time,distance");
}
void loop() {
  char i;
  duration = 0;
  for( i=0 ; i<10 ; i++)
  {
    // Trigger Signal output
    digitalWrite(trig, HIGH);   // trig pin에 High를 출력
    delay(10);
    digitalWrite(trig, LOW);  // trig pin에 Low를 출력
   
    // Echo Signal Input
    duration += pulseIn(echo, HIGH); //echoPin에서 펄스값을 받아온다
    delay(10);
  }
  duration = duration/10;

  Serial.print("DATA,TIME,");

  distance = ((float)(340*duration)/10000) / 2;
  Serial.print(distance);
  Serial.println("cm");
  delay(100); 
}
