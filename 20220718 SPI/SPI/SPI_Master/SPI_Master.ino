//Master
 
#include <SPI.h>
void setup (void)
{
  SPI.begin ();
  digitalWrite(SS, HIGH); // 슬레이브 선택 해제
  SPI.setClockDivider(SPI_CLOCK_DIV16); // 통신 속도 설정 2/4/8/16/32/64/128
  Serial.begin(9600);
}
 
void loop (void) {
  digitalWrite(SS,LOW); // 슬레이브 선택
  int A = SPI.transfer(0); // 1바이트 데이터(0)를 보내고 A 수신
delay(5);
  digitalWrite(SS, HIGH); // 슬레이브 선택 해제
  Serial.println(A);
}
