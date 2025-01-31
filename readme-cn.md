# DTTF-Sim

DTTF-Sim 是一款为自动驾驶开发的基于数字孪生的交通流模拟器，专为模拟高速公路场景而设计。该工具旨在为自动驾驶系统提供动态、逼真的交通流环境，用于测试和验证自动驾驶算法在高速公路场景中的性能。

目前，DTTF-Sim 基于开源的 Carla 仿真平台 (v0.9.15) 构建，提供了多个功能，包括对真实交通流数据的驱动和高度可配置的传感器系统。

[![视频示例](https://img.youtube.com/vi/7ycsj_db4H0/0.jpg)](https://www.youtube.com/watch?v=7ycsj_db4H0)
## 1.系统环境

DTTF-Sim 当前支持在 Ubuntu 22.04 系统上运行。

> **注意**：v0.1.0 版本提供了可执行文件，源码将在后续版本中开放。

## 2. 部署

### 2.1 Carla
DTTF-Sim 基于 Carla 仿真平台构建，支持使用 Carla (v0.9.15) 提供的高质量仿真环境。Carla 可以从 [Carla 官网](https://carla.org/) 下载，或直接通过 [下载链接](https://tiny.carla.org/carla-0-9-15-linux) 获取。安装 Carla 的详细步骤请参考 [Carla 官方文档](https://carla.readthedocs.io/en/0.9.15/)。

### 2.2 下载DTTF-Sim
DTTF-Sim 可执行程序和相关文件存储在 [Google Drive](https://drive.google.com/file/d/15clB6-KZXRp6fTIvMYVb7k7vaiZ-Ib_I/view?usp=sharing) 中，您可以从该链接免费下载。


下载后，DTTF-Sim 的文件结构如下：

```shell
├── bin        # 二进制可执行文件
├── config     # 配置文件
├── data       # 真实交通流特征驱动文件
├── lib        # 依赖库
└── log        # 日志目录
start.sh       # DTTF-Sim 启动脚本
```

### 2.3 启动
启动 DTTF-Sim 前，请确保 Carla 环境已经启动。然后，使用以下命令运行 start.sh 启动脚本：
```shell
sh /path/to/DTTF-Sim/start.sh
```
脚本将自动加载 Carla 环境并初始化 DTTF-Sim 仿真环境。

### 2.4 自动驾驶系统接入
DTTF-Sim 提供与自动驾驶系统（如 Apollo）进行接入的接口。通过 UDP 或 UNIX 域流通信协议，用户可以将自动驾驶系统的数据发送至 DTTF-Sim，生成仿真车辆。

自动驾驶车辆的配置和创建指令采用 Protobuf 协议，支持多种传感器配置，包括相机、激光雷达、毫米波雷达、GNSS 等。

DTTF-Sim 使用TCP方式在地址 0.0.0.0:50051 上监听并接受自动驾驶车辆的配置和创建指令。


传感器配置协议在文件 `proto/autonomous_vehicle_creation_command.proto` 中定义。

传感器通信协议在文件 `proto/message.h` 中定义。


## 3. 示例

### 3.1  HighD 高速交通流场景孪生 

[![视频示例](https://img.youtube.com/vi/zPRp-Gs7UI4/0.jpg)](https://www.youtube.com/watch?v=zPRp-Gs7UI4)

DTTF-Sim 提供了基于 HighD 数据集的高速公路交通流场景。在 v0.1.0 版本中，已经集成了 HighD 数据集，并为其提供了特定的交通流驱动文件。v0.1.0版提供了适配Carla 0.9.15 Ubuntu环境的资源文件，资源文件保存在[Google Drive](https://drive.google.com/file/d/1DShAA5DvjNesSulPco-kVAJsSspIjIDv/view?usp=sharing)中。资源文件的导入方式请参照[教程](https://carla.readthedocs.io/en/0.9.15/tuto_M_add_map_package/)中的6，7进行导入。资源文件名为/Game/map_package/Maps/HighD-3-2/HighD-3-2。HighD-3-2的真实交通流特征驱动文件保存在data目录中。导入完成后，运行 start.sh 脚本即可加载真实的高速公路场景，并开始仿真。



### 3.2 接入Apollo自动驾驶系统

对于希望将 DTTF-Sim 与 Apollo 自动驾驶系统进行集成的用户，DTTF-Sim 提供了统一的接入接口。为了简化与 Apollo 的集成过程，我们提供了一个与 Apollo 消息传递系统兼容的数据桥——apollo_dttf，该组件能将 DTTF-Sim 的仿真数据与 Apollo 的各类传感器系统进行对接。

apollo_dttf 的目录结构如下：
```shell
├── apollo_dttf_component.cc            # apollo_dttf 组件
├── apollo_dttf_component.h
├── BUILD
├── common                  
│   ├── BUILD
│   ├── common.cc
│   ├── common.h
│   └── message.h                       # 传感器消息协议
├── conf
│   └── apollo_dttf.pb.txt              # 测试配置文件
├── cyberfile.xml
├── dag
│   └── apollo_dttf.dag
├── drivers                             # 传感器驱动文件，负责将DTTF-Sim数据与Apollo数据进行对齐
│   ├── BUILD
│   ├── camera_component.cc             # 相机传感器
│   ├── camera_component.h          
│   ├── chassis_component.cc            # 底盘传感器
│   ├── chassis_component.h
│   ├── driver_base.h 
│   ├── gnss_component.cc               # GNSS传感器
│   ├── gnss_component.h
│   ├── ground_true_component.cc        # 真值目标传感器
│   ├── ground_true_component.h
│   ├── imu_component.cc                # IMU传感器
│   ├── imu_component.h
│   ├── lidar_component.cc              # 激光雷达传感器
│   ├── lidar_component.h
│   ├── localization_component.cc       # 定位传感器
│   ├── localization_component.h
│   ├── radar_component.cc              # 毫米波雷达传感器
│   └── radar_component.h
├── launch
│   └── apollo_dttf.launch              # apollo_dttf启动文件
├── map                                 # highd-3地图文件
│   └── highd-3
│       ├── base_map.xml                # Apollo 高精度地图文件
│       ├── routing_map.bin             
│       ├── routing_map.txt
│       ├── sim_map.bin
│       └── sim_map.txt
├── proto                               # 自动驾驶车辆配置协议
│   ├── BUILD
│   └── dttf_rpc.proto
└── util                                # Unix域
    ├── BUILD
    ├── tcp_server.cc
    ├── tcp_server.h
    ├── tcp_client.cc
    ├── tcp_client.h
    ├── unix_socket_client.cc
    ├── unix_socket_client.h
    ├── unix_socket_server.cc
    └── unix_socket_server.h
```
您可以通过将 apollo_dttf 集成到 Apollo 项目中，来实现与 DTTF-Sim 仿真环境的实时数据交换。

### 3.2 编译
数据桥apollo_dttf使用的Apollo版本为10.0。用户可参照[官方文档](https://apollo.baidu.com/docs/apollo/10.x/index.html)进行安装编译Apollo。

使用一下指令进行apollo_dttf编译
```shell
buildtool build -p modules/apollo_dttf
```

### 3.3 运行
使用cyber_launch启动数据桥`apollo_dttf`

```shell
cyber_launch start modules/apollo_dttf/launch/apollo_dttf.launch
```

# 贡献
我们欢迎社区对 DTTF-Sim 进行贡献！如果您发现任何问题或有改进的建议，请提交 Issue 或 Pull Request。
