# -*- coding: utf-8 -*-
"""
Created on Mon Dec 14 14:21:11 2020

@author: lenovo
"""

#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 21:33:15 2020

@author: pi
"""

import cv2

 
import numpy as np
 
#设定蓝色阈值，HSV空间
 
blueLower = np.array([100, 80, 46])
 
blueUpper = np.array([124, 255, 255])
 
#打开摄像头
 
camera = cv2.VideoCapture(0)
 
#创建一个xianshifangfa
 
def ceju_xianshi():
 
    while True:
 
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
 
        mask = cv2.inRange(hsv, blueLower, blueUpper)
 
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
 
            #计算目标距离
 
            juli = (1029.006/w)*2.54
            juli1 = str(juli)
            
 
            # 输出距离
 
            print("juli",juli1)
 
            print("shijikuandu",w)
 
            # 显示矩形框
 
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.putText(frame, juli1, (30, 300), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)#shipin_zhong,xinshi_juli
 
        cv2.imshow('Frame', frame)
        
        
        cv2.waitKey(1)
 
        # 摄像头释放
 
if __name__ == '__main__':
 
    ceju_xianshi()
