/************************************************************************************************************************  
 *   
 * Define
 * 
 ************************************************************************************************************************/
  #include <PWM.h>
  #include <math.h>

/************************************************************************************************************************  
 *   
 * Global Variables
 * 
 ************************************************************************************************************************/
  
  //'T' 'F'
  char debug_mode = 'F';
  
  int pwm_pin = 3;            // pin no : pwn output
  int pwm_init = 50;          // varialbe : pwm soft-start target value
  int pwm_ctr = 50;           // variable : pwm control value@loop
  int pwm_ctrl_amout = 1;     // varialbe : pwm control resolution value
  int32_t frequency = 30000;  // frequency (in Hz)
  int analogPin = 0;          // pin no : voltage input
  int val = 50;               // variable : voltage input
  int vth = 380; // = 20v              // variable : target voltage value => 0V@0, 5.37V@100V, 10.30V@200, 20.19V@375

  //pwm 오차
  int vhys = 5;

  //pid
  float Kp = 0.02;
  float Ki = 0.01;
  float Kd = 0.03;

  float error;
  float error_prev;
  float goal = 10;
  float P_control, I_control, D_control;
  float Time = 0.003;
  float PID_control;

    

/************************************************************************************************************************  
 *   
 * Functions
 * 
 ************************************************************************************************************************/


/************************************************************************************************************************  
 * PID Control                                                                                                          
 ************************************************************************************************************************/
  int pidcontrol(int vth_goal, int vth_curr)
  {
    error = vth_goal - vth_curr;
    P_control = Kp * error;
    I_control += Ki * error * Time;
    D_control = Kd * (error - error_prev) / Time;
    PID_control = P_control + I_control + D_control;
    PID_control = constrain(PID_control, 0, 40);
    PID_control = round(PID_control);
    
    error_prev = error;
    
    if(debug_mode == 'T')
    {
      Serial.print("PID_control : ");
      Serial.println(PID_control);
    }
      Serial.print("PID_control : ");
      Serial.println(PID_control);    
//  delay(500);
    return (int)PID_control;
  }

/************************************************************************************************************************  
 * PWM Control                                                                                                          
 ************************************************************************************************************************/
  int pwm_ctrl()
  {
    //read voltage input = pwm 0~1024 max 300
    val = analogRead(analogPin);

    if(abs(val - vth) > 6)
    {
      if(val < (vth+vhys)){
        pwm_ctr = pwm_ctr + pidcontrol((vth+vhys), val);
        if(pwm_ctr > (vth+vhys))
          pwm_ctr = (vth+vhys);
      }
      else if(val > (vth-vhys)){
        pwm_ctr = pwm_ctr - pidcontrol(val, (vth-vhys));
        if(pwm_ctr < 0)
          pwm_ctr = 0;
      }
    }
    else
    {
      delay(3000);
      if(vth == 250)
        vth = 100;
      else if(vth == 100)
        vth = 250;  
    }
    
    if(debug_mode == 'T')
    {
      Serial.print("val : ");
      Serial.println(val);
      Serial.print("vth : ");
      Serial.println(vth);
      Serial.print("pwm_ctr : ");
      Serial.println(pwm_ctr);
    }
    return pwm_ctr;  
  }

/************************************************************************************************************************  
 * setup
 ************************************************************************************************************************/
  void setup(){
    Serial.begin(9600);
    
    //pwm timer init, pwmWrite 사용하기 위함
    InitTimersSafe();
    
    //pwm frequency init, pin의 주파수(단위 hz)를 세팅하고 성공여부를 bool로 돌려줌
    bool success = SetPinFrequencySafe(3, frequency);

    //soft-start 0.5s동안 0 > 50 
    for(int w = 0; w <= pwm_init; w = w + pwm_ctrl_amout)
    {
      Serial.print(".");
      delay(10);
      pwmWrite(3, w);
    }
    Serial.print("start\n");
    delay(2000);

  }

/************************************************************************************************************************  
 * Loop                                                                                                                 
 ************************************************************************************************************************/
  void loop(){
    pwmWrite(3, pwm_ctrl()); 
  }
