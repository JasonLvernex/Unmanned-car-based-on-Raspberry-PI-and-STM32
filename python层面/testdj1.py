# -*- coding: UTF-8 -*-
from PCA9685 import PCA9685 #导入驱动，
import time
pwm=PCA9685(0x40)#设置模块的地址，默认0x40
pwm.setsq(50)#设置频率
pwm.init()#初始化pca9685
pwm.allinit()#把16个通道初始化

jiaodu=0
ledoff=0
#pwm.setduoji(0,jiaodu)

while 1:
    a=raw_input("input:")
    a=int(a)
    pwm.setpwm(0,0,a)
    #pwm.setpwm(15,0,a)
#    pwm.setduoji(0,a)
    