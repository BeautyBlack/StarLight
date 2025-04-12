#StarLight Station

StarLight Station是牙科CT影像后处理工作站。为牙科医生提供CT影像浏览、报告编辑以及影像后处理功能。

1. 基础功能模块主要有以下:
  - 病人列表
  - 影像浏览
  - 报告编辑
  - 系统设置
  
2. 后处理模块主要有以下:
  - MPR/CPR
  - VR
    - 牙影像三维成像
    - 神经管提取
	- 食管提取
	- 牙种植体配准
	
	
## Getting Started

### Prerequisites
  - [QT](https://www.qt.io/) 界面库
  - Visual Studio 2017 编译工具
  
### Directories
  * ```bin``` Release编译模式下的构建目录
  * ```binD``` Debug编译模式下的构建目录
  * ```config``` 配置文件
  * ```Doc``` 说明文档
  * ```src``` StarLight工程代码
 
 
## Build
  1. 使用Visual Studio 2017 打开工程
  2. 选择对应的Debug/Release选项
  3. 编译主工程 StarLight

## Run
  1. 编译完成后，在bin【Release模式】/binD【Debug模式】文件夹下运行StarLightStation.exe
  
## Projects
  * ```Foundation.Common``` 【Lib】基础功能库，比较简单通用的小型功能集合库
  * ```Foundations.Interfaces``` 【Lib】接口定义，包含数据定义以及枚举型定义
  * ```Foundation.DataModule``` 【Lib】数据定义，包含数据定义以及枚举型定义
  * ```Foundation.Log``` 【Lib】日志模块
  * ```Include```  头文件目录
  * ```Lib```  Release编译模式下的lib文件目录
  * ```LibD```  Debug编译模式下的lib文件目录
  * ```Service.Mpr``` 【Dll】MPR 服务库
  * ```Service.VR``` 【Dll】VR 服务库
  * ```StarLightStation``` 【Exe】StarLight Station界面主工程库
  * ```SystemServer/StarLightServer``` 【Exe】DICOM 服务，系统级服务
  * ```Utilities.Configuation``` 【Dll】配置信息模块
  * ```Utilities.Database``` 【Dll】数据库模块
  * ```Utilities.DicomClient``` 【Dll】DICOM客户端
  * ```Utilities.DicomServer``` 【Dll】Dicom服务端
  * ```Utilities.ImageModule``` 【Dll】Dicom影像解析模块
  * ```Utilities.ImageViewer``` 【Dll】图像处理模块
  * ```StarLight.sln``` 主工程solution文件
  
## Database
  使用文件系统作为数据库，检查信息以Json格式保存在本地磁盘，以文件夹的层级目录作为分割日期以及检查
  手段。
