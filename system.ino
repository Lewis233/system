/*
 * 机械小车操作系统
 * 完成时间：2017/3/30
 * 作者：Lewis233
 */
#include <Servo.h>

#define H_SERVO 3
#define P_SERVO 5
#define T_SERVO 6
#define MOTOR_FRONT 10//直流电机输入
#define MOTOR_BACK 11//直流电机输入（反向）
#define EN 7//步进电机使能
#define CW 8//步进电机转动方向
#define CLK 9//步进电机脉冲信号
#define RT_MOTOR 100//motor_run()函数单次运行时间
#define RT_H 100//h_run()函数单次运行时间(毫秒)
#define RT_T 100//t_run()函数单次运行时间
#define RT_STEP 100//stepper_run()函数单次运行的时间
#define A_T_L 70//向左转的角度
#define A_T_R 110//向右转的角度
#define DELTA 1//云台每次转过的角度
#define CUBE 20//机械手抓正方体的角度
#define CONE 15//机械手抓圆锥
#define PUT 25//机械手放下工件

char pos = 90;//记录云台位置

Servo h_servo;//抓手处的舵机hand_servo
Servo p_servo;//平台舵机platform_servo
Servo t_servo;//转向舵机turn_servo

void setup() {
  Serial.begin(9600);//以9600的波特率初始化串口
  while(!Serial){
    ;//等待连接
  }

  h_servo.attach(H_SERVO);//绑定
  p_servo.attach(P_SERVO);
  t_servo.attach(T_SERVO);
  pinMode(MOTOR_FRONT, OUTPUT);
  pinMode(MOTOR_BACK, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(CW, OUTPUT);
  pinMode(CLK, OUTPUT);
}

void loop() {
  while(Serial.available() > 0){//如果串口打开了
    char c = Serial.read();
    switch(c){//根据串口读取的数值的不同做出不同动作
      case 'w': motor_run(true);break;//如果按下w键，小车向前走
      case 's': motor_run(false);break;//如果按下s键，小车向后走
      case 'a': t_run(A_T_L);break;//按下a键，向左转
      case 'd': t_run(A_T_R);break;//按下d键，向右转
      case 'i': stepper_run(true);break;//按下i键，吊臂向上
      case 'k': stepper_run(false);break;//按下k键，吊臂向下
      case 'j': p_servo.write(pos += DELTA);break;//按下j键，云台向左转
      case 'l': p_servo.write(pos += DELTA);break;//按下l键，云台向右转
      case '1': h_servo.write(CUBE);break;//抓正方体
      case '2': h_servo.write(CONE);break;//抓圆锥
      case '0': h_servo.write(PUT);break;//放下
      default : break;
    }
  }
}

void motor_run(bool dir){//控制直流电机，true正转，false反转
  if(dir){
    digitalWrite(MOTOR_FRONT, HIGH);
    delay(RT_MOTOR);//持续一段时间
    digitalWrite(MOTOR_FRONT, LOW);//运动周期结束
  }
  else{
    digitalWrite(MOTOR_BACK, HIGH);
    delay(RT_MOTOR);
    digitalWrite(MOTOR_BACK, LOW);//运动周期结束
  }
}

void t_run(char angle){
  t_servo.write(angle);
  delay(RT_T);
  t_servo.write((A_T_L + A_T_R)/2);
}

void stepper_run(bool dir){//控制步进电机，true向上，false向下
  digitalWrite(EN, LOW);
  if(dir){
    digitalWrite(CW, LOW);
    digitalWrite(CLK, HIGH);
    delay(RT_STEP/2);
    digitalWrite(CLK, LOW);
    delay(RT_STEP/2);
  }
  else{
    digitalWrite(CW, HIGH);
    digitalWrite(CLK, HIGH);
    delay(RT_STEP / 2);
    digitalWrite(CLK, LOW);
    delay(RT_STEP / 2);
  }
}


