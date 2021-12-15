"""
    * @par Copyright (C): bit_MME_team4_thanos
    * @file        thanos.py
    * @version      V1.0
    * @details
    * @par History
    * @author: 吕添、张琪悦、巩俐安
"""

from __future__ import division#精准除法
import cv2 
import numpy as np
import Adafruit_PCA9685
import threading #使能多线程任务
import  RPi.GPIO as GPIO
import time
import serial  
from time import sleep 
from PCA9685 import PCA9685 #导入驱动， 
 
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)
#初始化PCA9685和舵机
#servo_pwm = Adafruit_PCA9685.PCA9685()  

#初始化PCA参数
pca=PCA9685(0x40)#设置模块的地址，默认0x40
pca.setsq(50)#设置频率
pca.init()#初始化pca9685
pca.allinit()#把16个通道初始化


# 树莓派小车引脚定义
PWMA = 18
#AIN1   =  22
#AIN2   =  27

PWMB = 23
#BIN1   = 25
#BIN2  =  24

BtnPin  = 19
Gpin    = 5
Rpin    = 6

TXP = 14
RXP = 15

def setup():
   GPIO.setwarnings(False) 
   GPIO.setmode(GPIO.BCM)
   GPIO.setup(Gpin, GPIO.OUT)     # 设置绿色Led引脚模式输出
   GPIO.setup(Rpin, GPIO.OUT)     # 设置红色Led引脚模式输出
   GPIO.setup(BtnPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # 设置输入BtnPin模式，拉高至高电平(3.3V) 
   GPIO.setup(AIN2,GPIO.OUT)
   GPIO.setup(AIN1,GPIO.OUT)
   GPIO.setup(PWMA,GPIO.OUT)

   GPIO.setup(BIN1,GPIO.OUT)
   GPIO.setup(BIN2,GPIO.OUT)
   GPIO.setup(PWMB,GPIO.OUT)

#L_Motor= GPIO.PWM(PWMA,100)
#L_Motor.start(0)

#R_Motor = GPIO.PWM(PWMB,100)
#R_Motor.start(0)


# Configure min and max servo pulse lengths
#servo_min = 150  # Min pulse length out of 4096
#servo_max = 600  # Max pulse length out of 4096
# Helper function to make setting a servo pulse width simpler.
#def set_servo_pulse(channel, pulse):
 #   pulse_length = 1000000    # 1,000,000 us per second
  #  pulse_length //= 60       # 60 Hz
   # print('{0}us per period'.format(pulse_length))
    #pulse_length //= 4096     # 12 bits of resolution
    #print('{0}us per bit'.format(pulse_length))
    #pulse *= 1000
    #pulse //= pulse_length
    #pwm.set_pwm(channel, 0, pulse)

#def set_servo_angle(channel,angle):
 #   angle=4096*((angle*11)+500)/20000
  #  pwm.set_pwm(channel,0,int(angle))

# 频率设置为50hz，适用于舵机系统。
#pwm.set_pwm_freq(50)
#set_servo_angle(5,90)  #底座舵机
#set_servo_angle(4,135)  # 顶部舵机

#time.sleep(0.5)

#舵机云台的每个自由度需要4个变量
#pid_thisError_x=500       #当前误差值
#pid_lastError_x=100       #上一次误差值
#pid_thisError_y=500
#pid_lastError_y=100

#pid_x=0
#pid_y=0

# 舵机的转动角度
#pid_Y_P = 280
#pid_X_P = 300           #转动角度
#pid_flag=0


# 机器人舵机旋转
#def Robot_servo(X_P,Y_P):
 #   servo_pwm.set_pwm(5,0,650-pid_X_P)
 #   servo_pwm.set_pwm(4,0,650-pid_Y_P)


def nothing(x):
    pass
    

        
def keysacn():
    val = GPIO.input(BtnPin)
    while GPIO.input(BtnPin) == False:
        val = GPIO.input(BtnPin)
    while GPIO.input(BtnPin) == True:
        time.sleep(0.01)
        val = GPIO.input(BtnPin)
        if val == True:
            GPIO.output(Rpin,1)
            while GPIO.input(BtnPin) == False:
                GPIO.output(Rpin,0)
        else:
            GPIO.output(Rpin,0)

def getRespose(self):
    pass
    #print "获取到响应1"


GPIO.add_event_detect(RXP, GPIO.RISING)  ##注册一个检测电压由低变高事件
GPIO.add_event_callback(RXP, getRespose)  ##检测到事件触发回调getRespose 函数

def recv(serial):    
    data  
    while True:    
        data =serial.read(30)    
        if data == '':    
            continue  
        else:  
            break  
        sleep(0.02)   
    return data    
                
def main_thanos():
    setup()
    keysacn()
   # L_Motor= GPIO.PWM(PWMA,100)
  #  L_Motor.start(0)
  #  R_Motor = GPIO.PWM(PWMB,100)
  #  R_Motor.start(0)
    setangle(n,angle,su)
   while True:
	   data =recv(ser)
        if GPIO.event_detected(touchPin):
        print "检测到事件"
    try:
        while True:
            SR_2 = GPIO.input(SensorRight)
            SL_2 = GPIO.input(SensorLeft)
            if SL_2 == True and SR_2 == True:      
                t_up(50,0)
            elif SL_2 == True and SR_2 ==False:
                t_left(50,0)
            elif SL_2==False and SR_2 ==True:
                t_right(50,0)
            else:
                t_stop(0.3)
                t_down(50,0.4)
                t_left(50,0.5)
    except KeyboardInterrupt:   # 当按下Ctrl+C时，将执行子程序destroy()。
        GPIO.cleanup()

main_thanos()
