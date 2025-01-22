# DTTF-Sim

DTTF-Sim是一个为高速自动驾驶开发的基于数字孪生的交通流模拟器. DTTF-Sim旨在为自动驾驶系统提供高速场景下的动态交通流环境。DTTF-Sim基于Carla(0.9.15)构建. 


<video src="https://www.youtube.com/watch?v=7ycsj_db4H0" controls="controls" width="500" height="300"></video>

## 1.系统环境

目前， DTTF-Sim支持在Ubuntu 22.04系统上运行。v0.1.0版本仅提供可执行文件

## 2. 部署

### 2.1 Carla
DTTF-Sim基于Carla虚拟引擎构建，我们建议用户下载[0.9.15发行版](https://tiny.carla.org/carla-0-9-15-linux), Carla的详细安装方法请参考官网教程。

### 2.2 下载DTTF-Sim
DTTF-Sim运行程序保存在[Google Drive](https://drive.google.com/file/d/15clB6-KZXRp6fTIvMYVb7k7vaiZ-Ib_I/view?usp=sharing)中.

DTTF-Sim软件目录如下所示：
```shell
├── bin        # 二进制可执行文件
├── config     # 配置文件
├── data       # 真实交通流特征驱动文件
├── lib        # 依赖库
└── log        # 日志目录
start.sh       # DTTF-Sim启动脚本
```

### 2.3 启动
我们推荐通过执行`start.sh`启动DTTF-Sim. 在启动DTTF-Sim之前，请确保Carla环境已启动。
```shell
sh /path/to/DTTF-Sim/start.sh
```
### 2.4 自动驾驶系统接入

自动驾驶车辆的配置协议使用Protobuf进行定义。自动驾驶系统可以在任意使用使用该配置协议进行创建自动驾驶车辆。DTTF-Sim使用UDP方式在地址0.0.0.0:50051上监听自动驾驶车辆配置信息。
```protobuf
syntax = "proto2";
package dttf.rpc;


enum CommunicationType{                 // 消息通信类型，考虑传感器通信效率问题，目前传感器数据、控制信息仅支持UNIX域流方式通信。
  None = 0;                
  TCP = 1;                              // TCP
  UNIX = 2;                             // UNIX 域
  UDP = 3;                              // UDP 
}


message Communication{                  // 通信配置        
  required CommunicationType type = 1;
  required string url = 2;              // 套接字通信地址 
}

message Location{                       // 位置信息
  required double x = 1;                         
  required double y = 2;                        
  required double z = 3;                         
}

message Rotation{                       // 姿态信息
  required double roll = 1;
  required double yaw = 2;
  required double pitch = 3;
}

message CameraConfig{                            // 相机传感器配置
  required string name = 1;                      // 相机传感器名
  required Location location = 2;                // 相机相对于车辆中心的偏移
  required Rotation rotation = 3;                // 相机姿态
  required uint32 fov = 4;                       // 相机FOV
  required uint32 width = 5;                     // 像宽
  required uint32 height = 6;                    // 像高 
  required double lens_flare_intensity = 7;      // 镜头光晕强度
  required Communication communication = 8;      // 相机传感器数据通信方式

}

message LidarConfig{                             // 激光雷达传感器配置
  required string name = 1;                      // 激光雷达传感器名
  required Location location = 2;                // 激光雷达相对于车辆中心的安装位置
  required Rotation rotation = 3;                // 激光雷达的姿态
  required uint32 channels = 4;                  // 激光雷达的通道数
  required uint32 range = 5;                     // 激光雷达的探测半径
  required uint32 rotation_frequency = 6;        // 激光雷达的旋转频率
  required uint32 horizontal_fov = 7;            // 激光雷达的水平视野范围
  required int32 upper_fov = 8;                  // 激光雷达的垂直-上极限视野
  required int32 lower_fov = 9;                  // 激光雷达的垂直-下极限视野
  required Communication communication = 11;     // 激光雷达传感器数据通信方式
}

message GnssConfig{                              // GNSS传感器配置
  required string name = 1;                      // GNSS传感器名
  required Location location = 2;                // GNSS设备相对于车辆中心的安装位置
  required Rotation rotation = 3;                // GNSS设备的姿态
  required Communication communication = 11;     // GNSS传感器数据通信方式
}

message ImuConfig{                               // IMU传感器配置
  required string name = 1;                      // IMU传感器名
  required Location location = 2;                // IMU设备相对于车辆中心的安装位置
  required Rotation rotation = 3;                // IMU设备姿态
  required Communication communication = 11;     // IMU传感器数据通信方式
}

message RadarConfig{                             // Radar传感器配置
  required string name = 1;                      // Radar传感器名
  required Location location = 2;                // Radar传感器相对于车辆中心安装位置
  required Rotation rotation = 3;                // Radar传感器姿态
  required uint32 horizontal_fov = 4;            // 毫米波雷达水平视野范围
  required uint32 range = 5;                     // 毫米波雷达探测距离
  required uint32 vertical_fov = 6;              // 毫米波雷达垂直视野范围
  required Communication communication = 11;     // 毫米波雷达数据通信方式
}

message GroundTrueConfig{                        // 真值传感器配置
  required string name = 1;                      // 真值传感器名
  required uint32 range = 2;                     // 真值传感器探测范围
  required Communication communication = 11;     // 真值传感器通信方式
}

message LocalizationConfig{                      // 定位传感器配置 (GNSS+IMU)
  required string name = 1;                      // 定位传感器名
  required Location location = 2;                // 定位传感器相对于车辆中心的安装位置
  required Rotation rotation = 3;                // 定位传感器姿态
  required Communication communication = 11;     // 定位传感器数据通信方式
}

message ChassisConfig{                           // 底盘配置
  required string name = 1;                      // 底盘名
  required Communication communication = 11;     // 底盘数据通信方式
}

message ControlConfig{                           // 车辆控制信息配置
  required string name = 1;                      // 控制信息名
  required Communication communication = 11;     // 车辆控制信息通信方式
}

message AutonomousVehicleConfigCommand{          // 自动驾驶车辆配置命令
  required string name = 1;                      // 自动驾驶车辆名
  required string type = 2;                      // 自动驾驶车辆类型，参照Carla支持的车辆类型(如: vehicle.tesla.model3)
  required Location location = 3;                // 初始时车辆位置信息(x: 经度， y: 纬度， z: 高程)
  required Rotation rotation = 4;                // 初始时车辆姿态信息
  repeated CameraConfig cameras = 6;             // 相机传感器配置
  repeated LidarConfig lidars = 7;               // 激光雷达传感器配置
  repeated RadarConfig radars = 8;               // 毫米波雷达传感器配置
  repeated GnssConfig gnsses = 9;                // GNSS传感器配置
  repeated ImuConfig imus = 10;                  // IMU传感器配置
  repeated GroundTrueConfig ground_true = 11;    // 真值传感器配置
  repeated LocalizationConfig localization = 12; // 定位传感器配置
  required ChassisConfig chassis = 13;           // 底盘配置
  required ControlConfig control = 14;           // 控制配置
}
```


## 3. 样例

### 3.1 孪生 HighD 高速交通流场景 

目前我们使用HighD高质量交通数据集构建了一个DTTF-Sim的使用样例。v0.1.0版提供了适配Carla 0.9.15 Ubuntu环境的资源文件，资源文件的下载地址保存在[Google Drive](https://drive.google.com/file/d/1DShAA5DvjNesSulPco-kVAJsSspIjIDv/view?usp=sharing)中。资源文件的导入方式请参照[教程](https://carla.readthedocs.io/en/0.9.15/tuto_M_add_map_package/)中的6，7进行导入。资源文件名为/Game/map_package/Maps/HighD-3-2/HighD-3-2。HighD-3-2的真实交通流特征驱动文件保存在data目录中。在资源文件导入完成后，执行start.sh启动脚本即可在仿真环境中孪生真实的高速交通流场景。

### 3.2 接入Apollo自动驾驶系统

我们不建议测试用户使用Carla提供的API在DTTF-Sim环境中构建仿真车辆，否则DTTF-Sim中的背景车辆将不会识别到该仿真车辆以至于发生未知的碰撞事件。DTTF-Sim系统提供了统一的自动驾驶系统接入接口。该仓库提供了DTTF-Sim与Apollo消息传递的数据桥，apollo_dttf。

apollo_dttf的文件组织方式Apollo项目中的模块相同。其文件结构为：
```shell
./
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
│   ├── ground_true_component.cc        # 真值传感器
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
    ├── unix_socket_client.cc
    ├── unix_socket_client.h
    ├── unix_socket_server.cc
    └── unix_socket_server.h
```


