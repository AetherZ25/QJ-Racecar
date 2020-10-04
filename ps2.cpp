#include <PS2X_lib.h>  //for v1.6
#include "ps2.h"
#include "iodefine.h"
#include "motor.h"
#include <Arduino.h>

extern void moveForward();

PS2X ps2x; // create PS2 Controller Class

int error ;
byte type ;
byte vibrate;

Serial.begin(57600);//设置通讯，将波特率设定为57600

 delay(300);  //增加了延迟以便在配置无线ps2模块之前给它一些启动时间
 
 error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);//设置引脚和设置检查错误
 
 if(error == 0){
    Serial.print("Found Controller, configured successful ");//检查正常输出正常标志
    Serial.print("pressures = ");//正常情况下输出按下去的按键
    if (pressures)
	  Serial.println("true ");//遥控器有压感时输出
    else
	  Serial.println("false");//无感应时输出
    Serial.print("rumble = ");
	if (rumble)//遥控器与接收器是否链接
  Serial.println("true)");
  else
	  Serial.println("false");
  Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  
  else if(error == 1)//无法连接
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if(error == 2)//遥控器命令无效
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   else if(error == 3)//遥控器不匹配
    Serial.println("Controller refusing to enter Pressures mode, may not support it. "); 
    
    type = ps2x.readType();
    switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void runps2() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */
  if(error == 1) //skip loop if no controller found
    return;

  if(type == 2){ //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller
    
     if(ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if(ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if(ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if(ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");
      
    if(ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");
    if(ps2x.Button(UP_STRUM))//will be TRUE as long as button is pressed
      Serial.println("forward");
    if(ps2x.Button(DOWN_STRUM))
      Serial.println("backup；
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");
    if(ps2x.Button(ORANGE_FRET)) {     // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
  }
     else { //DualShock Controller
     ps2x.read_gamepad(false, vibrate); //读取控制器并设置遥控器反馈为震动
     
    if(ps2x.Button(PSB_START))         //当有“开始”按键按住时（这里没有用到这个按键）
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))//当有“选择”按键按住时（这里没有用到这个按键）
      Serial.println("Select is being held");

    if(ps2x.Button(PSB_PAD_UP)) {      //当有“向上”按键按住时
      Serial.print("Forward ");
      moveForward();//调用电机向前的程序
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);//以十进制形式输出“向上”按键的ASCII编码值，为了方便后期检查是否通讯错误
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){  //当有“向右”按键按住时
      Serial.print("Right1  ");
      moveTurnRight();//调用电机向右的程序
      Serial.print("Right2  ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT)){//当有“向左”按键按住时
      Serial.print("LEFT");
      moveTurnLeft();           //调用电机向左的程序
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN)){//当有“向下”按键按住时
      Serial.print("Backup ");
      moveBackward(); //调用电机向后的程序
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }
    if(ps2x.ButtonReleased(PSB_PAD_DOWN)) //当向上键松开时
    {
       moveStop();//调用电机停下的程序使小车停下
    }
     if(ps2x.ButtonReleased(PSB_PAD_LEFT)) //当向左键松开时
    {
       moveStop();//调用电机停下的程序使小车停下
    } if(ps2x.ButtonReleased(PSB_PAD_RIGHT)) //当向右键松开时
    {
       moveStop();//调用电机停下的程序使小车停下
    } if(ps2x.ButtonReleased(PSB_PAD_UP)) //当向下键松开时
    {
       moveStop();//调用电机停下的程序使小车停下
    }
      
    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");
    }
    
    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
    if(ps2x.NewButtonState(PSB_CROSS)) 
    {//will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
       moveStop();
    }
    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
      Serial.println("Square just released");

    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC);
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  }
  delay(50);
}
