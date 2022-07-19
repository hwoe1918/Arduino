//#define _BV(A) 1<<A
//#define MSTR 4
//#define SPE 6
//#define SPIE 7
//위의 소스는 굳이 쓸 필요 없이 헤더파일에 있는 내용입니다
 
#include <SPI.h>
 
void setup (void)
{
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT); // 마스터에서 클럭을 쏴주는거니 당연히 Input
  pinMode(SS, INPUT);

  SPCR |= _BV(SPE);
  SPCR &= ~_BV(MSTR);
  SPCR |= _BV(SPIE);
 
  SPI.setClockDivider(SPI_CLOCK_DIV16); // 마스터와 같은 속도로 설정
 
  Serial.begin(9600);
}
 
int B = 0;
 
ISR (SPI_STC_vect) // SPI Interrupt / 데이터 수신시 자동 호출
{
  byte Confirm = SPDR; // SPDR이라는 8비트 레지스터에 데이터를 수신
  if(Confirm == 0)
    Serial.println("1234");
  SPDR = B; // 송신 또한 SPDR을 통해 송신
}
 
  void loop (void)
  {
     B++;
     delay(1000);
  }
