# odrive_driver
    头文件是odrive.h和odrive.cpp。Odrive_init会自动对Odrive的电机和测编码器进行校准，进入斜坡速度控制模式。
todo:
    目前速度控制的ramprate默认是150且无法修改，之后可以加入ramprate的设置功能。