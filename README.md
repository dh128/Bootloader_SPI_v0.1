# Bootloader_SPI_v0.1
# 通过串口先将固件包传输保存至SPI Flash中，然后从SPI Flash中读取数据升级
# test push
0x1800		0x1801		0x1802		0x1803		0x1804
升级当前	当前版本	升级出厂	出厂版本	NB升级标志
开机判断：
升级？正常运行？

正常运行：
判断0xC400处是否连续为0xFF(10Bytes),不是则正常运行代码；
是则进入写入出厂固件状态。

写入出厂固件：
判断出厂固件版本号是否位0xFF,是则置位升级出厂固件标志位，复位；
不是0xFF说明存在出厂固件，则进入写入出厂固件代码，下载固件，完成后取消写入出厂固件标志位，复位。

升级：
判断升级出厂固件标志位，是则下载出厂固件代码，完成后清除升级出厂固件标志位，复位；
否则进入升级当前代码，下载最新代码至SPI Flash，然后更新MCU　Flash。


初始状态下设备：
设备进入Update程序
根据下发指令	0xFE----设备升级
				0xF1----写入出厂固件