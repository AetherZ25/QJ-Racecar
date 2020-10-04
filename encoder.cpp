#include "encoder.h"
#include "iodefine.h"
#include "motor.h"
#include <Arduino.h>

//volatile--异变的
volatile long left_encoder_cnts;
volatile long right_encoder_cnts;

oid setupEncoders()       //IO和中断设置
{
	//setup Left encoder
	pinMode(Left_Encoder_PinA,INPUT);//改左编码器引脚A模式为输入
	pinMode(Left_Encoder_PinB,INPUT);//改左编码器引脚B模式为输入
	attachInterrupt(0,updateLeftEncoder,RISING);//调节第三个参数，更改触发模式，使得电机可以同时测速和测向
	
	//setup Right encoder
	pinMode(Right_Encoder_PinA,INPUT);//改右编码器引脚A模式为输入
	pinMode(Right_Encoder_PinB,INPUT);//改右编码器引脚B模式为输入
	attachInterrupt(3,updateRightEncoder,RISING);//分配中断引脚，中断初始化 在这里是上升沿触发中断20号IO口（中断通道3号对应着20号IO），中断函数是updateRightEncoder也就是完成了右侧轮子的编码后（先左后右）

	
}

void updateLeftEncoder()
{
	/*
	if(int_occur):
		update left_encoder_cnts//+ or -
	*/
    //int current_left_B;
    if(digitalRead(Left_Encoder_PinB)==LOW)//读取左轮的编码器B引脚也就是编码数的数字信号（编码器已经将模拟信号转换成数字信号） 如果比设定值更低 输出的是低电平
       
        left_encoder_cnts++;//左轮要快一点
    else if(digitalRead(Left_Encoder_PinB)==HIGH)//读取右轮的编码器B引脚也就是编码数的数字信号（编码器已经将模拟信号转换成数字信号） 如果比设定值更高 输出的是高电平
       
        left_encoder_cnts--;//左轮要慢一点
	
}

//要对右编码器计数值进行更新
void updateRightEncoder()
{
	/*
	if(int_occur):
		update right_encoder_cnts//+ or -
	*/
    if(digitalRead(Right_Encoder_PinB)==HIGH)//读取右轮的编码器B引脚也就是编码数的数字信号（编码器已经将模拟信号转换成数字信号） 如果比设定值更低 输出的是低电平
        right_encoder_cnts++;//右轮应当再快一点
    else
       right_encoder_cnts--;//右轮应当再慢一点
}

void printEncoders()        //串口显示和打印
{
	Serial.print(left_encoder_cnts);
	Serial.print('\t');
	Serial.println(right_encoder_cnts);

}
