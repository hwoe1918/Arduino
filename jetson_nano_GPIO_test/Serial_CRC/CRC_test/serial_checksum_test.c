#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <termios.h>                  
#include <fcntl.h>                   
#include <errno.h> 
#include <unistd.h> 

union
{
    float data;
    char  bytedata[4];
} float_data;

union
{
    short data ;
    char  bytedata[2];
} int_data, checksum, checksum2;


static int uart_fd;
unsigned char protocal_receive[20] = {0,};

char cnt_rcv = 0;
short check = 0;
short datasize = 0;

int init_serial_port(void)
{
  int serial_port = open("/dev/ttyUSB0", O_RDWR);
  if(serial_port < 0)
  {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
      exit(1);
  }
  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;
  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }
  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
  tty.c_cc[VTIME] = 100;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;
  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);
  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return -1;
  }
  else
  {
      return serial_port;
  }
}


void *readserial_thread(void *pt)
{

    int num_bytes = -1;
    unsigned char insert_buf;
    while(1)
    {
     	while( (num_bytes = read(uart_fd, &insert_buf, 1)   ) > 0 )	
       {
		    datasize++;
			protocal_receive[cnt_rcv] = insert_buf; 
			
			if(protocal_receive[cnt_rcv] == '*')
			{
				if(protocal_receive[((cnt_rcv-8)+20)%20] == '#' && ((protocal_receive[((cnt_rcv-7)+20)%20] == 'I') || (protocal_receive[((cnt_rcv - 7) + 20) % 20] == 'F')))
				{
					for(int i = 3; i < 8; i++)
					{
						check += protocal_receive[((cnt_rcv - i)+20)%20];
					}
					checksum2.data = check;
					check = 0;
							
					if(checksum2.bytedata[0] == protocal_receive[((cnt_rcv-1)+20)%20] && checksum2.bytedata[1] == protocal_receive[((cnt_rcv-2)+20)%20])
					{
						if(protocal_receive[((cnt_rcv - 7) + 20) % 20 ] == 'I')
						{
							int_data.bytedata[1] = protocal_receive[((cnt_rcv - 6) + 20) % 20];
							int_data.bytedata[0] = protocal_receive[((cnt_rcv - 5) + 20) % 20];
							printf("Receive Data : %d\n", int_data.data);
							printf("Packet Size : %d\n", datasize);
						}
						
						else
						{
							float_data.bytedata[3] = protocal_receive[((cnt_rcv - 6) + 20) % 20];
							float_data.bytedata[2] = protocal_receive[((cnt_rcv - 5) + 20) % 20];
							float_data.bytedata[1] = protocal_receive[((cnt_rcv - 4) + 20) % 20];
							float_data.bytedata[0] = protocal_receive[((cnt_rcv - 3) + 20) % 20];
							printf("Receive Data : %f\n", float_data.data);
							printf("Packet Size : %d\n", datasize);
							
						}
						
						clear_array();
						cnt_rcv = 0;
					}
					
					else
					{	
						printf("CRC check Error!!!\n");
						printf("CRC : %x %x\n\n", checksum2.bytedata[1], checksum2.bytedata[0]);
						clear_array();
						cnt_rcv = 0;
					}
				}

				else
				{
					clear_array();
					cnt_rcv = 0;
				}
			}
			cnt_rcv++;
			cnt_rcv %= 20;
		}
		
		printf(". . . . . \n\n");
		if(datasize > 0)
			datasize = 0;
    }
}


void clear_array(void)
{
	for(int i = 0; i < 20; i++)
	{
		protocal_receive[i] = 0;
	}
	
}


int main(void)
{
  uart_fd = init_serial_port();
 
  pthread_t id_1;
  int ret1=pthread_create(&id_1,NULL,readserial_thread,NULL);

  while(1)
  {
	  
  }
      
  close(uart_fd);
  return 0;
}
