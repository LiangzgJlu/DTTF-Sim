# DTTF-Sim

DTTF-Sim is a digital twin-based traffic flow simulator developed for autonomous driving, specifically designed for simulating highway scenarios. The tool aims to provide a dynamic and realistic traffic flow environment for testing and validating the performance of autonomous driving algorithms in highway scenarios.

Currently, DTTF-Sim is built on the open-source Carla simulation platform (v0.9.15) and offers several features, including real traffic flow data-driven simulation and a highly configurable sensor system.

[![Video Example](https://img.youtube.com/vi/7ycsj_db4H0/0.jpg)](https://www.youtube.com/watch?v=7ycsj_db4H0)

## 1. System Environment

DTTF-Sim currently supports running on Ubuntu 22.04.

> **Note**: The v0.1.0 version provides an executable file, and the source code will be released in subsequent versions.

## 2. Deployment

### 2.1 Carla
DTTF-Sim is built on the Carla simulation platform and supports the high-quality simulation environment provided by Carla (v0.9.15). Carla can be downloaded from the [Carla official website](https://carla.org/) or directly via the [download link](https://tiny.carla.org/carla-0-9-15-linux). For detailed installation steps, please refer to the [Carla official documentation](https://carla.readthedocs.io/en/0.9.15/).

### 2.2 Download DTTF-Sim
The DTTF-Sim executable and related files are stored on [Google Drive](https://drive.google.com/file/d/15clB6-KZXRp6fTIvMYVb7k7vaiZ-Ib_I/view?usp=sharing), and you can download them for free from this link.

After downloading, the file structure of DTTF-Sim is as follows:

```shell
├── bin        # Binary executable files
├── config     # Configuration files
├── data       # Real traffic flow feature driving files
├── lib        # Dependency libraries
└── log        # Log directory
start.sh       # DTTF-Sim startup script
```

### 2.3 Startup
Before starting DTTF-Sim, ensure that the Carla environment is already running. Then, use the following command to run the start.sh script:
```shell
sh /path/to/DTTF-Sim/start.sh
```
The script will automatically load the Carla environment and initialize the DTTF-Sim simulation environment.

### 2.4 Autonomous Driving System Integration
DTTF-Sim provides interfaces for integrating with autonomous driving systems (such as Apollo). Through UDP or UNIX domain stream communication protocols, users can send data from the autonomous driving system to DTTF-Sim to generate simulated vehicles.

The configuration and creation commands for autonomous driving vehicles use the Protobuf protocol, supporting various sensor configurations, including cameras, LiDAR, millimeter-wave radar, GNSS, etc.

DTTF-Sim listens and accepts configuration and creation commands for autonomous driving vehicles via TCP at address 0.0.0.0:50051.

The sensor configuration protocol is defined in the file `proto/autonomous_vehicle_creation_command.proto`.

The sensor communication protocol is defined in the file `proto/message.h`.

## 3. Examples

### 3.1 HighD Highway Traffic Flow Scenario Twin

[![Video Example](https://img.youtube.com/vi/zPRp-Gs7UI4/0.jpg)](https://www.youtube.com/watch?v=zPRp-Gs7UI4)

DTTF-Sim provides a highway traffic flow scenario based on the HighD dataset. In the v0.1.0 version, the HighD dataset has been integrated, and specific traffic flow driving files have been provided for it. The v0.1.0 version offers resource files adapted for the Carla 0.9.15 Ubuntu environment, which are stored on [Google Drive](https://drive.google.com/file/d/1DShAA5DvjNesSulPco-kVAJsSspIjIDv/view?usp=sharing). The import method for the resource files can be found in the [tutorial](https://carla.readthedocs.io/en/0.9.15/tuto_M_add_map_package/) under steps 6 and 7. The resource file is named /Game/map_package/Maps/HighD-3-2/HighD-3-2. The real traffic flow feature driving files for HighD-3-2 are stored in the data directory. After importing, run the start.sh script to load the real highway scenario and start the simulation.

### 3.2 Integration with Apollo Autonomous Driving System

For users who wish to integrate DTTF-Sim with the Apollo autonomous driving system, DTTF-Sim provides a unified integration interface. To simplify the integration process with Apollo, we offer a data bridge compatible with Apollo's messaging system—apollo_dttf, which can connect DTTF-Sim's simulation data with Apollo's various sensor systems.

The directory structure of apollo_dttf is as follows:
```shell
├── apollo_dttf_component.cc            # apollo_dttf component
├── apollo_dttf_component.h
├── BUILD
├── common                  
│   ├── BUILD
│   ├── common.cc
│   ├── common.h
│   └── message.h                       # Sensor message protocol
├── conf
│   └── apollo_dttf.pb.txt              # Test configuration file
├── cyberfile.xml
├── dag
│   └── apollo_dttf.dag
├── drivers                             # Sensor driver files, responsible for aligning DTTF-Sim data with Apollo data
│   ├── BUILD
│   ├── camera_component.cc             # Camera sensor
│   ├── camera_component.h          
│   ├── chassis_component.cc            # Chassis sensor
│   ├── chassis_component.h
│   ├── driver_base.h 
│   ├── gnss_component.cc               # GNSS sensor
│   ├── gnss_component.h
│   ├── ground_true_component.cc        # Ground truth target sensor
│   ├── ground_true_component.h
│   ├── imu_component.cc                # IMU sensor
│   ├── imu_component.h
│   ├── lidar_component.cc              # LiDAR sensor
│   ├── lidar_component.h
│   ├── localization_component.cc       # Localization sensor
│   ├── localization_component.h
│   ├── radar_component.cc              # Millimeter-wave radar sensor
│   └── radar_component.h
├── launch
│   └── apollo_dttf.launch              # apollo_dttf launch file
├── map                                 # highd-3 map file
│   └── highd-3
│       ├── base_map.xml                # Apollo high-definition map file
│       ├── routing_map.bin             
│       ├── routing_map.txt
│       ├── sim_map.bin
│       └── sim_map.txt
├── proto                               # Autonomous vehicle configuration protocol
│   ├── BUILD
│   └── dttf_rpc.proto
└── util                                # Unix domain
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
You can integrate apollo_dttf into the Apollo project to achieve real-time data exchange with the DTTF-Sim simulation environment.

### 3.2 Compilation
The data bridge apollo_dttf uses Apollo version 10.0. Users can refer to the [official documentation](https://apollo.baidu.com/docs/apollo/10.x/index.html) to install and compile Apollo.

Use the following command to compile apollo_dttf:
```shell
buildtool build -p modules/apollo_dttf
```

### 3.3 Running
Use cyber_launch to start the data bridge `apollo_dttf`:

```shell
cyber_launch start modules/apollo_dttf/launch/apollo_dttf.launch
```

# Contribution
We welcome community contributions to DTTF-Sim! If you find any issues or have suggestions for improvements, please submit an Issue or Pull Request.