# station
一个记录气象的装置
</br>
## 实现的数据
温度，湿度，风速风向，光照强度，气压，空气质量等</br>
## 项目简介
本项目是在物联网平台上呵树莓派上的硬件方面，具体是将测试周围气象的各种数据，并将数据传入服务器，可转换成面向客户形式。</br>
## 内容介绍
- 项目开发板：wemos d1，树莓派。
- 项目数据示例：保存在data.txt中。
- 项目包含供wemos烧写的.ino C语言程序和树莓派编写的.py python文件
- 项目传导数据方式：
- 1.树莓派文档直接写入
- 2.wemos esp8266 模块通过无线网传入服务器。</br></br>
## 树莓派：
树莓派上python代码获取数据</br>
get_data.py可将wemos或arduino串口上的数据提取,并通过文档的形式显示。</br>
关于如何抓取arduino上的串口数据见我的csdn博客：</br>
</a>https://blog.csdn.net/chengshunhang/article/details/80644915</a></br></br>
## wemos d1:
esp8266传入数据</br>
数据传入格式：json。</br>
使用的协议：websocket协议</br>
物联网平台网站：</br>
实验举例：————.ino文件。</br>
