#include "motor.h"
#include "iodefine.h"
#include <Arduino.h>

#define PWM_A 120
#define PWM_B 120

void setupMotor()
{int i;
for (i=22;i<=25;i++)
  pinMode(i,OUTPUT);//利用一个简单的办法把22~25号引脚变成输出模式
  Serial.begin(115200);//初始化串口通信，并将波特率设置为115200。高波特率有利于快速传输
}

//实现车体前进，配置两个电机转速相同，开启ENA，ENB，配置IN1,IN2开关状态
//digitalwrite()    analogwrite()--0~255
void moveForward()//前进部分
{  analogWrite(MOTOR_L_ENA,PWM_A);//左轮的PWM调速

   analogWrite(MOTOR_R_ENB,PWM_B);//右轮的PWM调速

  digitalWrite(MOTOR_L_IN2,HIGH);//左轮前进电机驱动写入高电平，电机正转

  digitalWrite(MOTOR_L_IN1,LOW);//左轮后退电机驱动写入低电平，电机不反转

  digitalWrite(MOTOR_R_IN4,HIGH);//右轮前进电机驱动写入高电平，电机正转

  digitalWrite(MOTOR_R_IN3,LOW);//右轮后退电机驱动写入低电平，电机不反转
}

//实现电机后退，配置逻辑和void moveForward()相反
void moveBackward()
{
   analogWrite(MOTOR_L_ENA,PWM_A);
 analogWrite(MOTOR_R_ENB,PWM_B);

  digitalWrite(MOTOR_L_IN2,LOW);

  digitalWrite(MOTOR_L_IN1,HIGH);

  digitalWrite(MOTOR_R_IN4,LOW);

  digitalWrite(MOTOR_R_IN3,HIGH);



}

//右轮速度>左轮速度，可以直接使左轮停止，也可以配置左右轮速度差不同，表现为PWM输入值不同
void moveTurnLeft()//左转部分
{
analogWrite(MOTOR_L_ENA,0);//左轮的PWM调速，PWM为可以设定的占空比，在这里为了转向最快设置为零
 analogWrite(MOTOR_R_ENB,PWM_B);//右轮的PWM调速，PWM为可以设定的占空比

  digitalWrite(MOTOR_L_IN2,HIGH);//左轮 前进电机驱动写入高电平，电机正转

  digitalWrite(MOTOR_L_IN1,LOW);//左轮 后退电机驱动写入低电平，电机不反转

  digitalWrite(MOTOR_R_IN4,HIGH);//右轮 前进电机驱动写入高电平，电机正转

  digitalWrite(MOTOR_R_IN3,LOW);//右轮 后退电机驱动写入低电平，电机不反转


}

//与左转逻辑相反
void moveTurnRight()
{
analogWrite(MOTOR_R_ENB,0);
 analogWrite(MOTOR_L_ENA,PWM_B);

  digitalWrite(MOTOR_L_IN2,HIGH);
  digitalWrite(MOTOR_L_IN1,LOW);
  digitalWrite(MOTOR_R_IN4,HIGH);
  digitalWrite(MOTOR_R_IN3,LOW);

}

//同时设置ENA和ENB为0，也可以设置IN1，IN2或IN3，IN4同电平
void moveStop()
{
analogWrite(MOTOR_L_ENA,0);
 analogWrite(MOTOR_R_ENB,0);

  digitalWrite(MOTOR_L_IN1,LOW);

  digitalWrite(MOTOR_L_IN2,LOW);

  digitalWrite(MOTOR_R_IN3,LOW);

  digitalWrite(MOTOR_R_IN4,LOW);

}

void upPCCtl()
{int a;
    a=Serial.read();
    Serial.println(a);
    if (a=='w')
    
        moveForward();
    
    if (a=='s')
    
        moveBackward();
    
    if (a=='a')
    
     moveTurnLeft();
    
    if (a=='d')
    
   moveTurnRight();
 
    if (a=='x')
    
       moveStop();
    
}
