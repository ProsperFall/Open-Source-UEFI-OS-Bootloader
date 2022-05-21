@[TOC](UEFI开发EDK2环境搭建（Ubuntu - Linux X86_64）)
如果有误，希望能在评论区指出。文章语法上的问题也可以。

# UEFI是什么？

很多同学都对 UEFI 有一个误解，“ UEFI就是 BIOS 和传统 BIOS 几乎无区别” ，“ UEFI 是系统引导方式”。
UEFI 全称 Unified Extensible Firmware Interface （统一可扩展固件接口），UEFI在功能上更接近于操作系统，甚至 NVMe 组织将 UEFI 和诸如 Windos 、 Linux 等操作系统划为一“类”。厂商将其称为 “BIOS” 是为了便于用户理解。

# 开发系统环境的选择——Linux
这里笔者推荐使用Linux，主要理由如下

1.微软Windows环境搭建较麻烦，如果是EDK不支持的VS版本需要手动配置很多环境变量，EDK 2（一个全面的UEFI开发环境，相较于其他EFI环境，最亮的特点就是支持ARM平台）更新适配Windows过慢；
2.Windows支持的GNU工具很多都是老掉牙的版本[待证实]；
3.Linux基础工具安装便捷；
4.Linux下EDK 2的问题更易于排除。

以Ubuntu-21.10为例: 
虚拟机寻址会受到限制，由虚拟机平台提供相应的优化，例如VMware只提供给我们256MB自由发挥。UEFI 标准规定的2GB以后的空间载入系统加载器，256MB远远不足我们使用来开发标准UEFI程序。所属 Windows 的 UEFI 程序并未按照UEFI规范编写，因为 Windows 支持仅有2GB的设备。
写这篇文章的原因：
1.市面上的教程都不是最新版本的，在实践过程中会难免有不同，甚至是不适用。（本文章作于2021年末 2022年初）
2.多数问题的排除与解决较难
3.国内相关文章是有，不过他们不是复制的就是抄书，基本上对于GCC11环境无助
4.写这篇文章的主要目的还是希望能帮助其他人输入build就能看见Done（EDK 2编译成功的标准）

最好是在实体机上进行测试[参考文章](https://blog.csdn.net/r77683962/article/details/118640380)

# 正文

## 安装工具

```powershell
sudo apt install git
sudo apt install build-essential
sudo apt install make
代码库工具

sudo apt install gcc
sudo apt install python
sudo apt install iasl
sudo apt install nasm
汇编器、编译器与工具

sudo apt install uuid-dev

sudo apt install xorg-dev
UI库
```

### 1.获取EDK 2代码

```powershell
git clone https://github.com/tianocore/edk2
```
更新Submodule

```powershell
cd edk2
git submodule update --init
```

拿着链接下载zip自行解压也可以
### 2.修改配置文件
文件~/edk2/Conf/target.txt
其他项看着改，下面是必须改的。根据要输出的目标文件进行修改。
TARGET_ARCH           = X64
TOOL_CHAIN_TAG        = GCC5

### 3.编译BaseTools
BaseTools的源码是没有Brotli模块的，需要自行下载保存到如下目录
~/edk2-master/Source/C/BrotliCompress/Brotli
[在谷歌的Git](https://github.com/google/brotli/blob/666c3280cc11dc433c303d79a83d4ffbdd12cc8d/LICENSE)
```powershell
git clone https://github.com/google/brotli
```
需要注意的是GCC11在使用https://github.com/google/brotli的代码时可能会出现类型不正确无法编译的等问题。如果你使用的平台安装的是GCC11，则应做处理（全部按照文章上方所提所做的话，Ubuntu安装的是GCC11）。
解决方案如下：
安装[brotli的另一个分支的代码](https://github.com/google/brotli/tree/f4153a09f87cbb9c826d8fc12c74642bb2d879ea)
[参考文章](https://github.com/BobCF/edk2/tree/brotli2/BaseTools/Source/C/BrotliCompress)

并修改Makefile和GNUmakefile
Makefile:
修改Common_Obj为：
COMMON_OBJ = \
  brotli\c\common\constants.obj \
  brotli\c\common\context.obj \
  brotli\c\common\dictionary.obj \
  brotli\c\common\platform.obj \
  brotli\c\common\shared_dictionary.obj \
  brotli\c\common\transform.obj
ENC_Obj中添加：
  brotli\c\enc\command.obj \
  brotli\c\enc\compound_dictionary.obj \
  brotli\c\enc\fast_log.obj \
![](https://img-blog.csdnimg.cn/bdf53d188818445aac087811ae5073dd.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUHJvc3Blcm91c0ZhbGw=,size_15,color_FFFFFF,t_70,g_se,x_16)
GNUmakefile:
在OBJECTS下添加：
  brotli/c/common/platform.o \
  brotli/c/common/shared_dictionary.o \
  brotli/c/common/constants.o \
  brotli/c/common/context.o \
  brotli/c/enc/command.o \
  brotli/c/enc/compound_dictionary.o \
  brotli/c/enc/fast_log.o \
 ![](https://img-blog.csdnimg.cn/ef3fa848889d4a319e2a556321eb01a1.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUHJvc3Blcm91c0ZhbGw=,size_10,color_FFFFFF,t_70,g_se,x_16)
```powershell
make -c BaseTools
```

### 4.编译模拟器
还要创建灵魂文件，不起作用，但没他EDK2又不工作
```powershell
~EDK2/
mkdir MdeModuelPkg/Library/BrotliCustomDecompressLib
mkdir MdeModuelPkg/Library/BrotliCustomDecompressLib/brotli
mkdir MdeModuelPkg/Library/BrotliCustomDecompressLib/brotli/c
mkdir MdeModuelPkg/Library/BrotliCustomDecompressLib/brotli/c/include
source edksetup.sh
```

检查GCC输出错误，我的GCC报错是找不到/usr/lib/gcc/X86_64-linux-gnu/10
（不同安装方式位置可能不同）
```powershell
ls /usr/lib/gcc/X86_64-linux-gnu
11
```

可以看到输出了11，我们安装的是gcc11，而需求gcc10的文件

```powershell
cd /usr/lib/gcc/X86_64-linux-gnu
sudo mkdir 10
sudo cp -r 11/* 10
```
返回EDK2目录

```powershell
cd edk2
重新进入edk2文件夹
source edksetup.sh
build
./Build/EmulatorX64/DEBUG_GCC5/X64/Host
启动GOP虚拟机
```
Done 是EDK 2编译成功的标志
缺少的包一般可以通过 https://github.com/tianocore/edk2 下方的表查到，除了EDK2提供的Brotli包，暂时还没有别的问题
![大功告成](https://img-blog.csdnimg.cn/0364f0c22cea4afcba927872c8c399ba.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXFfMzczMjkwMDM=,size_20,color_FFFFFF,t_70,g_se,x_16)
