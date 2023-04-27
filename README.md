# 将 xv6 移植到 nemu

## 环境准备
分别按照 nemu 和 xv6 的手册来准备环境：
nemu: https://nju-projectn.github.io/ics-pa-gitbook/ics2020/index.html

xv6: https://pdos.csail.mit.edu/6.828/2018/labguide.html

## 运行
1. clone 代码
2. ./test.sh

Note: 

1. 在 nemu 中对于按键事件，通过SDL库的 SDL_PollEvent 获取，而这需要聚焦的窗口，所以测试的时候需要在焦点在SDL创建的窗口中。测试结果如下：
![image](https://user-images.githubusercontent.com/65274647/234832580-6963d2e6-bf6b-4363-8fe8-c1c5ca912679.png)
