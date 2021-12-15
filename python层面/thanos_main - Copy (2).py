#!/usr/bin/env python2
# -*- coding: utf-8 -*-



import cv2
import numpy as np
import threading
import RPi.GPIO as GPIO
import time
import serial
from time import sleep
from PCA9685 import PCA9685




global ser
global data_2send
global SR_2, SL_2, SH_2
global interval
global test
test = 0
interval = 0
face_flag = 0
juli=0
count_cycle=0
# k1=0;k2=0
# serial init
# ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=0.5)
# if ser.isOpen == False:
#    ser.open()


# AT names:
# forward=0x08
# turn_left=0x04
# stop=0x05

# PCA_init
pca = PCA9685(0x40)
pca.setsq(50)
pca.init()
pca.allinit()

# pin define
BtnPin = 19
Gpin = 5
Rpin = 6

TXP = 14
RXP = 15

SensorRight = 16
# SensorLow  = 12
# SensorHigh = 13
SensorFront = 19

TRIG_f = 24
ECHO_f = 23


EPP_L = 26
EPP_R = 27

# 从网络摄像头获取输入
camera = cv2.VideoCapture(0)

def setup():
    global ser
    global SR_2, SL_2, SH_2, SR_1
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(Gpin, GPIO.OUT)  # led_green
    GPIO.setup(Rpin, GPIO.OUT)  # led_red
    GPIO.setup(BtnPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # buttom_pin setup
    GPIO.setup(SensorRight, GPIO.IN)
    GPIO.setup(SensorFront, GPIO.IN)
    #   GPIO.setup(SensorLow,GPIO.IN)
    #   GPIO.setup(SensorHigh,GPIO.IN)

    SR_2 = GPIO.input(SensorRight)
    SR_1 = GPIO.input(SensorFront)
    #   SL_2 = GPIO.input(SensorLow)
    #   SH_2 = GPIO.input(SensorHigh)

    GPIO.setup(TRIG_f, GPIO.OUT)
    GPIO.setup(ECHO_f, GPIO.IN)


    GPIO.setup(EPP_L, GPIO.OUT)
    GPIO.setup(EPP_R, GPIO.OUT)

    ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.5)


def nothing():
    pass



def keysacn():
    val = GPIO.input(BtnPin)
    while GPIO.input(BtnPin) == False:
        val = GPIO.input(BtnPin)
    while GPIO.input(BtnPin) == True:
        time.sleep(0.01)
        val = GPIO.input(BtnPin)
        if val == True:
            GPIO.output(Rpin, 1)
            while GPIO.input(BtnPin) == False:
                GPIO.output(Rpin, 0)
        else:
            GPIO.output(Rpin, 0)


# adjustment when first stage is done
def first_stage():
    global face_flag
    ser.write([0x05])
    #                avoid.join()
    #                get_interval.join()

    pca.setangle(1, 0, 50)
    pca.setangle(2, 90, 50)
    ser.write([0x06])
    sleep(1)  # change to visual condition
    ser.write([0x05])


#    face_flag=1

# infrared block
def infra_red():
    #	global SR_2
    global ser
    global test

    #	try:
    while test != 9:
        if GPIO.input(SensorRight) == False:
            ser.write([0x04]);test = 5;print("not adjusted\n")
        else:
            ser.write([0x08]);test = 9;pca.setangle(2, 0, 50);time.sleep(0.2);print("adjusted\n")
        sleep(0.01)


#	except Exception as e:
#			print(e)
avoid = threading.Thread(target=infra_red)


# ultrasonic block
def distance_f():
    GPIO.output(TRIG_f, 0)
    time.sleep(0.000002)

    GPIO.output(TRIG_f, 1)
    time.sleep(0.00001)
    GPIO.output(TRIG_f, 0)

    while GPIO.input(ECHO_f) == 0:
        a = 0
    time1 = time.time()
    while GPIO.input(ECHO_f) == 1:
        a = 1
    time2 = time.time()

    during = time2 - time1;
    k1 = during * 340 / 2 * 100
    return during * 340 / 2 * 100





def get_distance():
    global interval;
    global face_flag
    global ser
    while face_flag != 9:
        interval = distance_f()
        if interval + 11 < 20 and GPIO.input(SensorFront) == False: ser.write([0x08])
        if interval + 11 >= 20 and GPIO.input(SensorFront) == False: ser.write([0x05]);face_flag = 9;first_stage()
        else : ser.write([0x08])
#        print(interval)
        time.sleep(0.1)

    return interval


get_interval = threading.Thread(target=get_distance)

# 设定红色阈值，HSV空间

redLower = np.array([140, 43, 46])

redUpper = np.array([180, 255, 255])

# 创建一个xianshifangfa

def ceju_xianshi():
    global juli
    global count_cycle
    while count_cycle<128 :

        # 读取帧

        (ret, frame) = camera.read()

        frame = cv2.flip(frame, 1, dst=None)  # 水平翻转镜像

        # 判断是否成功打开摄像头

        if not ret:
            print('No Camera')

            break

        # 转到HSV空间

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # 根据阈值构建掩膜

        mask = cv2.inRange(hsv, redLower, redUpper)

        # 腐蚀操作

        mask = cv2.erode(mask, None, iterations=2)

        # 膨胀操作，其实先腐蚀再膨胀的效果是开运算，去除噪点

        mask = cv2.dilate(mask, None, iterations=2)

        # 轮廓检测

        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

        # 如果存在轮廓

        if len(cnts) > 0:
            # 找到面积最大的轮廓

            c = max(cnts, key=cv2.contourArea)

            # 确定面积最大的轮廓的矩形

            x, y, w, h = cv2.boundingRect(c)

            # 计算目标距离

            juli = (1029.006 / w) * 2.54
            juli1 = str(juli)
            count_cycle=count_cycle+1
        else :
            ser.write([0x04])
            time.sleep(0.05)
            count_cycle=count_cycle+1


        # 设定一个范围
        x_Lower = 150
        x_Upper = 180
        x_p = x + w / 2
        # 判断X方向范围来判断机器人的运动
        if x_p > x_Lower and x_p < x_Upper:
            ser.write([0x08])
        elif x_p < x_Lower:
            ser.write([0x04])
        elif x_p > x_Upper:
            ser.write([0x06])

        else:
            ser.write([0x04])

            # 输出距离

        print("juli", juli1)

        print("shijikuandu", w)

        # 显示矩形框

        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.putText(frame, juli1, (30, 300), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)  # shipin_zhong,xinshi_juli

    cv2.imshow('Frame', frame)

    cv2.waitKey(1)

    # 摄像头释放
vision_distance=threading.Thread(target=ceju_xianshi)

# big_arm (channel,angle,speed)
def lower_bar():
    pca.setangle(0, 90, 50)


def higher_bar():
    pca.setangle(0, 0, 50)


def main():
    global ser
    global face_flag
    global juli
    global count_cycle

    setup()
    print("started")
    higher_bar()
    pca.setangle(1, 0, 50)
    pca.setangle(2, 90, 50)
#    keysacn()
    while GPIO.input(SensorRight) == True:
        nothing()
    ser.write([0x44])
    print("keyed")
#
    lower_bar()
    sleep(1)
    ser.write([0x98])
    sleep(0.5)
    higher_bar()
    time.sleep(0.5)
    pca.setangle(1, 90, 50)
    pca.setangle(2, 0, 50)
    #avoid.start()
    get_interval.start()
    #avoid.join()
    #print("thread1done\n")
    get_interval.join()
    print("thread2done\n")
    time.sleep(0.5)
    ser.write([0x02])
    time.sleep(0.8)
    ser.write([0x05])

    pca.setangle(1, 90, 50)
    pca.setangle(2, 0, 50)
    ser.write([0x66])#stop angle adjusting
    time.sleep(1)

    vision_distance.start()

    try:
        while True:
            if juli<10 and count_cycle<5:
                ser.write([0x04])
            if count_cycle>=5 :
                ser.write([0x05])
                time.sleep(0.2)
                ser.write([0x76])
                time.sleep(0.5)
                while GPIO.input(SensorFront) == True :
                    ser.write([0x08])
                ser.write([0x05])
                GPIO.cleanup()






            # face the wall
            if face_flag == 9:
                #                fisrt_stage()
                #                ser.write([0x05])
                #                avoid.join()
                #                get_interval.join()
                #                pca.setangle(1,90,60)
                #                ser.write([0x08])
                #                sleep(3) #change to visual condition
                #                ser.write(0x05)
                #                face_flag=1
                print("done\n")
                count_cycle=count_cycle+1
                print(count_cycle)
            sleep(3)




    except KeyboardInterrupt:  # ctrl+c
        GPIO.cleanup()


# parent programme
if __name__ == '__main__':  # Program start from here
    main()
#    global ser
##    global face_flag
#
#    setup()
#
#
#    #keysacn()
#
#
#
#    lower_bar()
#    sleep(5)
#    higher_bar()
#
#    avoid.start()
#    get_interval.start()
#    avoid.join()
#    get_interval.join()
#
#
#
#    try:
#        while True:
#           # print(face_flag)
#
#
#            #unfold the arms
#            if GPIO.input(SensorRight) == True and face_flag!=9:
#                pca.setangle(1,90,60)
#            #face the wall
#            if face_flag==9:
#                fisrt_stage()
##                ser.write([0x05])
###                avoid.join()
###                get_interval.join()
##                pca.setangle(1,90,60)
##                ser.write([0x08])
##                sleep(3) #change to visual condition
##                ser.write(0x05)
##                face_flag=1
#                print("done\n")
#            sleep(3)
#
#
#
#
#    except KeyboardInterrupt:   # ctrl+c
#        GPIO.cleanup()

# main_thanos()
# -*- coding: utf-8 -*-

