#define TRIG 8
#define ECHO 9

#define SIZE 5

float cm_array[SIZE+1] = {0,};
float pre_avg = 0.0;
float cm = 0.0;
float duration = 0.0;

void setup()
{
Serial.begin(9600);
pinMode(TRIG, OUTPUT); // 센서 Trig 핀
pinMode(ECHO, INPUT); // 센서 Echo 핀
Serial.println("cm,Average");
}

void loop()
{
digitalWrite(TRIG, HIGH);
delayMicroseconds(200);
digitalWrite(TRIG, LOW);

duration = pulseIn(ECHO, HIGH);
cm = microsecondsToCentimeters(duration);

pre_avg = mov_avg_filter(pre_avg, cm);

Serial.print(cm);
Serial.print(",");
Serial.println(pre_avg);

delay(200); //1초에 5번 실행
}



float microsecondsToCentimeters(float microseconds)
{
return ((microseconds * 34)/1000)/2;
}

float mov_avg_filter(float pre_avg, float sample)
{
  float avg = 0.0;

  cm_array[SIZE] = sample;

  //재귀식 avg_x(k) = avg_x(k-1) + x(k)/n - x(k-n)/n
  avg = pre_avg + cm_array[SIZE] / SIZE - cm_array[0] / SIZE;

  for (int i = 1; i < SIZE+1; i++) 
  {
    cm_array[i - 1] = cm_array[i];
  }
  cm_array[SIZE] = 0.0;

//  Serial.print("SONAR : ");
//  for (int i = 0; i < SIZE; i++)
//  {
//    Serial.print(cm_array[i]);
//    Serial.print(" ");
//  }
//  Serial.print("\n");
  
  return avg;
}
