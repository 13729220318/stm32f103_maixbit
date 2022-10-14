import sensor,image,lcd,time
import time
import KPU as kpu
from fpioa_manager import fm
from Maix import GPIO
from machine import UART
from board import board_info

io_led_red = 13
fm.register(io_led_red, fm.fpioa.GPIO0)
led_r=GPIO(GPIO.GPIO0, GPIO.OUT)

#board_info.PIN1=10
#board_info.PIN2=9
fm.register(10, fm.fpioa.UART1_TX, force=True)
fm.register(9, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 115200, 8, 0, 0, timeout=1000, read_buf_len=4096)


lcd.init(type=1, freq=15000000, color=lcd.BLACK, invert = 0, lcd_type = 0)
#lcd.init(type=2)
#lcd.rotation(2)
lcd.clear(lcd.WHITE)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.set_vflip(1) #flip camera; maix go use sensor.set_hmirror(0)
sensor.set_hmirror(1)
sensor.set_vflip(1)
sensor.run(1)
img = image.Image(size=(320, 240))
clock = time.clock()
classes = ['aeroplane', 'bicycle', 'bird', 'boat', 'bottle', 'bus', 'car', 'cat', 'chair', 'cow', 'diningtable', 'dog', 'horse', 'motorbike', 'person', 'pottedplant', 'sheep', 'sofa', 'train', 'tvmonitor']
task = kpu.load(0x800000)
anchor = (1.08, 1.19, 3.42, 4.41, 6.63, 11.38, 9.42, 5.11, 16.62, 10.52)
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
counter = 0
while(True):
    clock.tick()
    img = sensor.snapshot()
    code = kpu.run_yolo2(task, img)


    if code:
        for i in code:
            a=img.draw_rectangle(i.rect())
            a = lcd.display(img)
            for i in code:
                lcd.draw_string(i.x(), i.y(), classes[i.classid()], lcd.RED, lcd.WHITE)
                lcd.draw_string(i.x(), i.y()+12, '%f'%i.value(), lcd.RED, lcd.WHITE)
                print('%f'%i.value())
                print( classes[i.classid()])



                if classes[i.classid()]==classes[14] and i.value()>0.6:#人
                    led_r.value(0) #只要有一次概率大于0.6
                    uart_A.write('1') #向stm32发送数字1
                elif classes[i.classid()]==classes[5] or classes[i.classid()]==classes[6] and i.value()>0.6:#车
                    led_r.value(0) #只要有一次概率大于0.6
                    uart_A.write('2')
                elif classes[i.classid()]==classes[7] or classes[i.classid()]==classes[11] and i.value()>0.6:#动物
                    led_r.value(0) #只要有一次概率大于0.6
                    uart_A.write('3')
                elif classes[i.classid()]==classes[8] or classes[i.classid()]==classes[18]and i.value()>0.6:#椅子
                    led_r.value(0) #只要有一次概率大于0.6
                    uart_A.write('4')
                elif classes[i.classid()]==classes[19] and i.value()>0.6:#火车
                    led_r.value(0) #只要有一次概率大于0.6
                    uart_A.write('5')



    else:
        a = lcd.display(img)
        led_r.value(1)
a = kpu.deinit(task)
