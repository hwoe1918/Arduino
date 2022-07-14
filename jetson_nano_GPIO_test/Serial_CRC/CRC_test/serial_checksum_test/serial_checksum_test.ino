#define MAX_RECEIVE_DATA 20
#define SIZE_PACKET  15

union
{
    short data ;
    char  bytedata[2];
} m_int;


union
{
    float data ;
    char  bytedata[4];
} m_float;


char send_buff[SIZE_PACKET+1] ={0,};
char check = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //send_packet();
  send_int_data_5();

}

void send_packet()
{
  send_int_data_2();
}

void clear_buff()
{
   for(int i=0;i<SIZE_PACKET+1;i++)
   {
     send_buff[i] = 0;
   }
}

void send_int_data_1()
{
   check++;
   send_buff[0] = '#';
   send_buff[1] = 'I';
   send_buff[2] = 0;
   send_buff[3] = 2;
   send_buff[4] = 0;
   send_buff[5] = 0;
   send_buff[6] = 0;
   send_buff[7] = 75;
   send_buff[8] = '*';
   //send_buff[9] = check;
   for(int i=0; i<9; i++)
   {
     Serial.print(send_buff[i]);
   }
   check++;
   clear_buff();
}

void send_int_data_2()
{
   check++;
   send_buff[0] = '#';
   send_buff[1] = '*';
   send_buff[2] = '#';
   send_buff[3] = 'I';
   send_buff[4] = 0;
   send_buff[5] = 2;
   send_buff[6] = 0;
   send_buff[7] = 0;
   send_buff[8] = 0;
   send_buff[9] = 75;
   send_buff[10] = '*';
   //send_buff[11] = check;
   for(int i=0;i<11;i++)
   {
     Serial.print(send_buff[i]);
   }

   clear_buff();
}

void send_int_data_3()
{
   check++;
   send_buff[0] = '#';
   send_buff[1] = '*';
   send_buff[2] = '#';
   send_buff[3] = 'I';
   send_buff[4] = 0;
   send_buff[5] = 2;
   send_buff[6] = 0;
   send_buff[7] = 0;
   send_buff[8] = 0;
   send_buff[9] = 78;
   send_buff[10] = '*';
   //send_buff[11] = check;
   for(int i=0;i<11;i++)
   {
     Serial.print(send_buff[i]);
   }
   clear_buff();
}

void send_int_data_4()
{
   check++;
   send_buff[0] = '#';
   send_buff[1] = '*';
   send_buff[2] = '#';
   send_buff[3] = 'I';
   send_buff[4] = 0;
   send_buff[5] = 5;
   send_buff[6] = 0;
   send_buff[7] = 0;
   send_buff[8] = 0;
   send_buff[9] = 78;
   send_buff[10] = '*';
   send_buff[11] = '*';
   //send_buff[12] = check;
   for(int i=0;i<12;i++)
   {
     Serial.print(send_buff[i]);
   }

   clear_buff();
}

void send_int_data_5()
{
   check++;
   send_buff[0] = '#';
   send_buff[1] = '*';
   send_buff[2] = '*';
   send_buff[3] = '*';
   send_buff[4] = '#';
   send_buff[5] = 'I';
   send_buff[6] = 0;
   send_buff[7] = 5;
   send_buff[8] = 0;
   send_buff[9] = 0;
   send_buff[10] = 0;
   send_buff[11] = 77;
   send_buff[12] = '*';
   send_buff[13] = '*';
   //send_buff[14] = check;
   for(int i=0;i<14;i++)
   {
     Serial.print(send_buff[i]);
   }

   clear_buff();
}


void loop() {
  // put your main code here, to run repeatedly:


}
