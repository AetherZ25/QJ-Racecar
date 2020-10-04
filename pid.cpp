#include "encoder.h"
#include "pid.h"
#include "motor.h"
#include <Arduino.h>
#include <math.h>

float Kp_l,Kp_r;
float Ki_l,Ki_r;
float Kd_l,Kd_r;
float right_encoder_cnts_pre;
float right_encoder_cnts_last;
long diff_right_cnts= right_encoder_cnts_pre-right_encoder_cnts_last;
float left_encoder_cnts_pre;
float left_encoder_cnts_last;
long diff_left_cnts= left_encoder_cnts_pre-left_encoder_cnts_last;
float speed_target_left;
float speed_target_right;
volatile float v_left;
volatile float v_right;
 float v_left_rpm,v_right_rpm;
 float pwm_left,pwm_right;

float diff_left_distance;
float diff_right_distance;

extern volatile long left_encoder_cnts;
extern volatile long right_encoder_cnts;

float speed_error_last_l;//全局
float speed_error_last_r;

long t_last=0;
/*         测左轮速度                   */
float speedMeasure()
{
  //left_encoder_cnts_last=0;
	
	long t_pre = millis();//记录下测速起点
	long left_encoder_cnts_pre = left_encoder_cnts;
  long right_encoder_cnts_pre = right_encoder_cnts;
	long diff_left_time = t_pre-t_last;
 long diff_right_time = t_pre-t_last;
  diff_left_cnts= left_encoder_cnts_pre-left_encoder_cnts_last;
  diff_right_cnts= right_encoder_cnts_pre-right_encoder_cnts_last;
	diff_left_distance = diff_left_cnts*(PI*D/N_PPR_PER_CIRCLE);//m
 diff_right_distance = diff_right_cnts*(PI*D/N_PPR_PER_CIRCLE);
	v_left = 1000*diff_left_distance/diff_left_time;//m/s
 v_right = 1000*diff_right_distance/diff_right_time;//m/s
  if (isnan(v_left)) v_left = 0;
  
  if (isnan(v_right)) v_right = 0;

  
	v_left_rpm = v_left*60/PI/D;


  
  v_right_rpm = v_right*60/PI/D;


 
	left_encoder_cnts_last = left_encoder_cnts_pre;
 right_encoder_cnts_last = right_encoder_cnts_pre;
/*  Serial.print(v_left_rpm);
 Serial.print('\t');
  Serial.print(v_right_rpm);
  Serial.print('\t');
    Serial.print(v_left);
 Serial.print('\t');
 Serial.print(v_right);
 Serial.print('\t');
  /*Serial.print(v_left);
  Serial.print('\t');
   Serial.println(v_right);*/
  delay(50);
  t_last = t_pre;
	
	return v_left, v_right;
}
/*       测右轮速度                  
float speedMeasureright()
{
  long tight_encoder_cnts_last=0;
  
  long t_pre = millis();//记录下测速起点
  long right_encoder_cnts_pre = left_encoder_cnts;
  long diff_right_time = t_pre-t_pre;
  float diff_right_distance = diff_right_cnts*(PI*D/N_PPR_PER_CIRCLE);//m
  v_right = diff_right_distance/diff_right_time;//m/s
  float v_right_rpm = v_right*60/PI/D;

  long t_last = t_pre;
  right_encoder_cnts_last = right_encoder_cnts_pre;
  
  return v_right;
}
 */


void setupPid()
{
	Kp_l = 10;
	Ki_l = 1000;
	Kd_l = 6;
	
	Kp_r = 10;
	Ki_r = 1000;
	Kd_r = 6;
	
	speed_target_left = 0.66;
	speed_target_right = 0.66;
	
	pwm_left = 120;
	pwm_right = 120;
	
}


void pidSpeed()
{
	/*    左轮PID测速    */
	
	float speed_left = v_left;//当前速度
	float speed_error_l = v_left-speed_target_left;
	float propration_left = speed_error_l*Kp_l;
	float integral_left = (speed_error_l + speed_error_last_l)*Ki_l;
	float diffiential_left = (speed_error_l - speed_error_last_l)*Kd_l;
	pwm_left = propration_left+integral_left+diffiential_left;
	if(pwm_left>=PWM_MAX)	pwm_left=PWM_MAX;
	if(pwm_left<=-PWM_MAX)	pwm_left=PWM_MAX;

	speed_error_last_l = speed_error_l;

  /*    右轮PID测速    */
  float speed_right = v_right;//当前速度
  float speed_error_r = v_right-speed_target_right;
  float propration_right = speed_error_r*Kp_r;
  float integral_right = (speed_error_r + speed_error_last_r)*Ki_r;
  float diffiential_right = (speed_error_r - speed_error_last_r)*Kd_r;
  pwm_right = propration_right+integral_right+diffiential_right;
  if(pwm_right>=PWM_MAX) pwm_right=PWM_MAX;
  if(pwm_right<=PWM_MIN) pwm_right=PWM_MIN;
 
  speed_error_last_r = speed_error_r;
    Serial.print(v_left);
  Serial.print('\t');

  Serial.print(v_right);
  Serial.print('\t');
    Serial.print(v_left_rpm);
  Serial.print('\t');
    Serial.print(v_right_rpm);
  Serial.print('\t');
  
 Serial.print(speed_error_l);//左轮的误差
 Serial.print('\t');
  Serial.println(pwm_left);
   
  /*
  Serial.print("The left error is:"); 
  Serial.print('\t');
  Serial.print(pwm_left);
  Serial.print("The left error is:"); 
  Serial.print('\t');
  Serial.println(pwm_right);
  */

  
}
