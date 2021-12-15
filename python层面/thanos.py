"""
    * @par Copyright (C): bit_MME_team4_thanos
    * @file        thanos.py
    * @version      V1.0
    * @details
    * @par History
    * @author: ���������á�������
"""

from __future__ import division#��׼����
import cv2 
import numpy as np
import Adafruit_PCA9685
import threading #ʹ�ܶ��߳�����
import  RPi.GPIO as GPIO
import time
import serial  
from time import sleep 
from PCA9685 import PCA9685 #���������� 
 
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5)
#��ʼ��PCA9685�Ͷ��
#servo_pwm = Adafruit_PCA9685.PCA9685()  

#��ʼ��PCA����
pca=PCA9685(0x40)#����ģ��ĵ�ַ��Ĭ��0x40
pca.setsq(50)#����Ƶ��
pca.init()#��ʼ��pca9685
pca.allinit()#��16��ͨ����ʼ��


# ��ݮ��С�����Ŷ���
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
   GPIO.setup(Gpin, GPIO.OUT)     # ������ɫLed����ģʽ���
   GPIO.setup(Rpin, GPIO.OUT)     # ���ú�ɫLed����ģʽ���
   GPIO.setup(BtnPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # ��������BtnPinģʽ���������ߵ�ƽ(3.3V) 
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

# Ƶ������Ϊ50hz�������ڶ��ϵͳ��
#pwm.set_pwm_freq(50)
#set_servo_angle(5,90)  #�������
#set_servo_angle(4,135)  # �������

#time.sleep(0.5)

#�����̨��ÿ�����ɶ���Ҫ4������
#pid_thisError_x=500       #��ǰ���ֵ
#pid_lastError_x=100       #��һ�����ֵ
#pid_thisError_y=500
#pid_lastError_y=100

#pid_x=0
#pid_y=0

# �����ת���Ƕ�
#pid_Y_P = 280
#pid_X_P = 300           #ת���Ƕ�
#pid_flag=0


# �����˶����ת
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
    #print "��ȡ����Ӧ1"


GPIO.add_event_detect(RXP, GPIO.RISING)  ##ע��һ������ѹ�ɵͱ���¼�
GPIO.add_event_callback(RXP, getRespose)  ##��⵽�¼������ص�getRespose ����

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
        print "��⵽�¼�"
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
    except KeyboardInterrupt:   # ������Ctrl+Cʱ����ִ���ӳ���destroy()��
        GPIO.cleanup()

main_thanos()
