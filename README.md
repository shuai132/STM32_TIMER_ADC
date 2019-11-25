# Scope

MCU: STM32F103C8Tx

## 介绍

用最少的硬件成本，做一个实用的虚拟示波器。

这是硬件部分，基于STM32最小系统，不使用任何片外外设。
可以方便地移植到其他硬件（只需要ADC、定时器、串口即可）。
采样率和采样深度等参数取决于芯片性能。

## Features

* 使用Timer触发ADC采样配合DMA读取
* 采样率可调 最高采样率1Msps
* 采样深度可调 默认最大2048点
* 支持FFT频谱分析(上位机实现)
* TODO: 多种触发方式
* TODO: USB CDC模拟串口
* TODO: Arduino工程

注: 
* 未设计前端运放电路，只用于采集0~3.3V信号。
* 需要C++11支持，推荐使用arm-none-eabi编译器和cmake构建，已在Window、macOS、Ubuntu下测试通过。
* 为了方便软件设计和数据解析，直接打包结构体作为消息单元。注意要和上位机保持一致的字节序（arm、x86、arduino均为小端模式）。

## 移植

设计上把可移植性作为重要目标，可以很方便地移植到其他单片机、Arduino、DSP。

只需要硬件提供几个具体实现的函数即可，具体可参考[App/Scope.h](App/Scope.h)中public方法的注释。

### 移植步骤

* 初始化scope
```cpp
    auto& scope = Scope::getInstance();
    scope.updateVolMax(3300);
    scope.setMcuImpl(
            {
                    .sendData = [](uint8_t* data, size_t size) {
                        // 用串口发送数据
                    },
                    .startSample = []{
                        // 开始采样
                    },
                    .stopSample = []{
                        // 停止采样
                    },
                    .setSampleFs = [](uint32_t fs) {
                        // 设置采样率 返回实际的采样率
                    },
            });
```
* 当收到串口数据时
```cpp
    Scope::getInstance().onRead(data, size);
```
* 当一次ADC转换完成
```cpp
    Scope::getInstance().add(volmV);
```

## DevelopTools

* STM32CubeMX
* arm-none-eabi-gcc
* CLion(CMake)
* OpenOCD
* ST-LINK

## Install Requirements(for macOS)

* config OpenOCD and ST-LINK
```bash
brew install openocd
brew install stlink
brew cask install gcc-arm-embedded
```

## Usage
* [blog](https://blog.csdn.net/u012456479/article/details/101543344)

## Links

* [arm-none-eabi-gcc](https://developer.arm.com/tools-and-software/open-source-software)
