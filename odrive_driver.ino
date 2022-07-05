#include <ODriveArduino.h>
#include <HardwareSerial.h>
#include "odrive.h"
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
HardwareSerial& odrive_serial = Serial2;
Stream& serial_(odrive_serial);
myOdrive odrive1(odrive_serial, 0);
myOdrive odrive2(odrive_serial, 1);
myOdrive *odrive_list[]={&odrive1, &odrive2};
float vel_bal=90;
void waiting_for_pc()
{
    while(1)
    {
        Serial.print("arduino");
        char c='0';
        while(Serial.available())
        {
            String str=Serial.readStringUntil('\n');
            int test=str.indexOf("test");
            int receive=str.indexOf("receive");
            if(receive!=-1)
            {
                c='r';
                break;
            }
           
        }
        if(c=='r')
        {
            break;
        }
    }
    return;
}
void setup() {
  odrive_serial.begin(115200);
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
  Serial.println("ODriveArduino");
  for(int i=0; i<2; i++)
  {
      odrive_list[i]->reboot();
  }
  for(int i=0; i<2; i++)
  {
      odrive_list[i]->Odrive_init();
  }
  for(int i=0; i<2; i++)
  {
      odrive_list[i]->set_vel(1);
  }
  waiting_for_pc();
  for(int i=0; i<2; i++)
  {
      odrive_list[i]->set_vel(vel_bal);
  }
}

void loop() {
    while(1)
    {
        if (Serial.available())
        {
            
            String str=Serial.readStringUntil('\n');
            if(str.length()<2)
            {
                continue;
            }
            char c1 = str[0];
            str=str.substring(1);
            if(c1!='v')
            {
                continue;
            }
            float vel=str.toFloat();
            if(vel>100)
            {
                vel=100;
            }
            if(vel<-100)
            {
                vel=-100;
            }
            float vels[2];
            vels[0]=vel_bal+vel/2;
            vels[1]=2*vel_bal-vels[0];
            for(int i=0;i<2;i++)
            {
                if(vels[i]<0)
                {
                    vels[i]=-vels[i];
                }
                odrive_list[i]->set_vel(vels[i]);
            }
        }
  
    }

}
/*
ODriveArduino odrive(odrive_serial);
bool vel_ctrl_read()
{
  Serial.println("press q to quit");
  while(!Serial.available());
  String str=Serial.readStringUntil('~');
  char c = str[0];
  if(c=='q')
  {
    return false;
  }
  int vel=0;
  bool nag=false;
  for(int i=0;i<str.length();i++)
  {
    if((str[i]<='9')&&(str[i]>='0'))
    {
      vel*=10;
      vel+=(str[i]-'0');
    }
    else if(str[i]=='-')
    {
        nag=!nag;
    }
  }
  if(nag)
  {
      vel=-vel;
  }
  //odrive_serial<<"w axis0.controller.input_vel"<<" "<<vel<<"\n";
  odrive_serial<<"v 0 "<<" "<<vel<<"\n";
  return true;
}
void vel_ctrl_mode()
{
  Serial.println("velocity control mode:");
  int ctr_mode=CONTROL_MODE_VELOCITY_CONTROL;
  odrive_serial<<"w axis0.controller.config.control_mode"<<" "<<ctr_mode<<"\n";
  odrive_serial<<"w axis0.controller.config.vel_ramp_rate"<<" "<<150<<"\n";
  odrive_serial<<"w axis0.controller.config.input_mode"<<" "<<INPUT_MODE_VEL_RAMP<<"\n";
  while(vel_ctrl_read());
}
void odrive_reboot()
{
  Serial.println("rebooting");
  odrive_serial<<"sr\n";
  odrive_serial<<"sr\n";
  odrive_serial<<"sr\n"; 
  return; 
}
void odrive_init()
{
  Serial.println("rebooting");
  odrive_serial<<"sr\n";
  odrive_serial<<"sr\n";
  odrive_serial<<"sr\n";
  delay(8000);
  Serial.println("start calibration");
  int requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  while(!odrive.run_state(0, requested_state, true, 20));
  Serial <<  "done\n";
  Serial.println("starting closeloop control!");
  requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL;
  odrive.run_state(0, requested_state, true) ;
  Serial <<  "done\n";
}
void setup() {
  odrive_serial.begin(115200);
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
  Serial.println("ODriveArduino");
  odrive_init();
}

void loop() {
  if (Serial.available())
  {
    String str=Serial.readStringUntil('~');
    char c = str[0];
    // Run calibration sequence
    if (c == 'v') 
    {
      vel_ctrl_mode();
    }
    if (c == 'r') 
    {
      odrive_reboot();
    }

    // Read bus voltage
    if (c == 'b') {
      odrive_serial << "r vbus_voltage\n";
      Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
    }

    // print motor positions in a 10s loop
    if (c == 'p') {
      static const unsigned long duration = 10000;
      unsigned long start = millis();
      while(millis() - start < duration) {
        for (int motor = 0; motor < 1; ++motor) {
          Serial << odrive.GetPosition(motor) << '\t';
        }
        Serial << '\n';
      }
    }
  }
}
*/
