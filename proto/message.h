#pragma once
#define MAGIC_NO 35712
#define VERSION "0.1.0"

/**
 * \brief 消息头
 * 
 */
struct MessageHeader {
    // 消息识别标志
    uint32_t magic;
    // 协议版本 
    char version[8];
    // 消息长度
    uint32_t msg_len;
    // 消息类型 (冗余)
    uint32_t msg_type;
    // 仿真帧
    uint32_t sim_frame;
    // 仿真时间
    double sim_time;
};

/**
 * \brief 加速度计消息
 * 
 */
struct AccelerometerMessage
{
    double x;
    double y;
    double z;
};

/**
 * \brief 陀螺仪消息
 * 
 */
struct GyroscopeMessage
{
    double x;
    double y;
    double z;
};

/**
 * \brief 位置消息
 * 
 */
struct LocationMessage{
    double x;
    double y;
    double z;
};
/**
 * \brief 姿态消息
 * 
 */
struct RotationMessage{
    double roll;
    double yaw;
    double pitch;
};

/**
 * \brief 速度消息, m/s
 * 
 */
struct SpeedMessage{
    double x;
    double y;
    double z;
};
/**
 * \brief 加速度消息, m/s^2
 * 
 */
struct AccelerationMessage{
    double x;
    double y;
    double z;
};
/**
 * \brief 角速度消息, rad/s
 * 
 */
struct AngularVelocityMessage{
    double x;
    double y;
    double z;
};

/**
 * \brief 四元数消息
 * 
 */
struct QuaternionMessage{
    double qx;
    double qy;
    double qz;
    double qw;
};

/**
 * \brief 定位消息
 * 
 */
struct LocalizationMessage{
    // 消息头
    MessageHeader header;
    // 航向角(度)
    double heading;
    // 位置信息 (x: 经度， y: 纬度, z: 高程)                       
    LocationMessage location;
    // 姿态信息
    RotationMessage rotation;
    // 速度信息
    SpeedMessage speed;
    // 加速度信息
    AccelerationMessage acceleration;
    // 角速度信息
    AngularVelocityMessage angular_velocity;
    // 四元数信息
    QuaternionMessage quaternion;
};

/**
 * \brief Gnss消息
 * 
 */
struct GnssMessage{
    // 消息头
    MessageHeader header;

    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;


};

/**
 * \brief 惯性测量单元消息
 * 
 */
struct ImuMessage{
    // 消息头
    MessageHeader header;
    // 加速度计消息
    AccelerometerMessage accelerometer; // Measures linear acceleration. m/s^2
    // 陀螺仪消息
    GyroscopeMessage gyroscope;     // Measures angular velocity. rad/s
    // 航向角
    float heading;         
};

/**
 * \brief 图像消息 RGB8
 * 
 */
struct CameraMessage {
    // 消息头
    MessageHeader header;
    // 像宽
    uint32_t width;
    // 像高
    uint32_t height;
    // 图片字节长度
    uint32_t length;
    /* data */
    // RGB8数据在CameraMessage尾部，字节长度为length.
};

/**
 * \brief 激光雷达探测点消息
 * 
 */
struct LidarPointMessage{
    float x;
    float y;
    float z;
    float i;
};

/**
 * \brief 激光雷达消息
 * 
 */
struct LidarMessage {
    // 消息头
    MessageHeader header;
    // 水平视角
    double horizontal_angle;
    // 通道数量
    int channels;
    // 激光雷达探测点数量
    int point_count;
    // 激光雷达探测点字节大小
    uint32_t point_size;
    /* data */
    // 激光雷达点云的数据在LidarMessage尾部，字节长度为point_count * point_size
};

/**
 * \brief 毫米波雷达探测点消息
 * 
 */
struct RadarPointMessage{
    // 高度
    float altitude;
    // 
    float azimuth;
    // 
    float depth;
    // 
    float velocity;
};

/**
 * \brief 毫米波雷达消息
 * 
 */
struct RadarMessage {
    // 消息头
    MessageHeader header;
    // 毫米波雷达探测点数量
    int point_count;
    // 毫米波雷达探测点大小
    uint32_t point_size;
    /* data */
    // 毫米波雷达点云数据在RadarMessage尾部
};

/**
 * \brief 车辆尺寸信息
 * 
 */
struct DimensionMessage
{
    // 长, 单位 m 
    double x;
    // 宽, 单位 m 
    double y;
    // 高, 单位 m 
    double z;
};

// 目标物信息
enum ObjectType{
    // 未知
    None = 0,
    // 车辆
    Car = 1,
    // 行人
    Pedestrian = 2
};

/**
 * \brief 目标物消息
 * 
 */
struct ObjectMessage{
    // 目标物ID, 连续
    uint32_t id;
    // 目标物类型
    ObjectType object_type;
    // 目标物位置信息(x: 经度， y: 纬度， z:高程)
    LocationMessage location;
    // 目标物速度信息
    SpeedMessage speed;
    // 目标物姿态
    RotationMessage rotation;
    // 目标物加速度
    AccelerationMessage acceleration;
    // 目标物维度
    DimensionMessage dimension;
    // 目标物航向
    float heading;
};

/**
 * \brief 真值目标物列表消息
 * 
 */
struct GroundTrueObjectMessage{
    MessageHeader header;
    double range;
    uint32_t object_count;
    /* data */
    // 目标物列表消息在GroundTrueObjectMessage尾部
};

// 档位状态
enum GearPosition {
    // N 档
    GEAR_NEUTRAL = 0,
    // D 档
    GEAR_DRIVE = 1,
    // R 档
    GEAR_REVERSE = 2,
    // P 档
    GEAR_PARKING = 3,
    GEAR_LOW = 4,
    GEAR_INVALID = 5,
    GEAR_NONE = 6,
};

/**
 * \brief 底盘消息
 * 
 */
struct ChassisMessage{
    // 消息头
    MessageHeader header;
    // 引擎转速
    double engine_rpm;
    // 速度
    double speed;
    // 行驶历程
    double odometer_m;
    // 燃油续航里程
    double fuel_range_m;
    // 油门量 [0-100]
    double throttle_percentage;
    // 刹车量 [0-100]
    double brake_percentage;
    // 方向盘转角 [-100, 100] Clockwise: negative, CountClockwise: positive
    double steering_percentage;
    // 驻车状态
    int  parking_brake;
    int gear_location;
    // 油门量 [0-100]
    double throttle_percentage_cmd;
    // 刹车量 [0-100]
    double brake_percentage_cmd;
    // 方向盘转角 [-100, 100] Clockwise: negative, CountClockwise: positive
    double steering_percentage_cmd;
};

/**
 * \brief 控制消息
 * 
 */
struct ControlMessage {
    MessageHeader header;

    // 油门量, [0-1]
    double throttle;
    // 刹车量, [0-1]
    double brake;
    // 方向盘转角量, [-1-1]， 向左: 小于0; 向右: 大于0
    double steer;
    // 是否倒车，0: 前进; 1: 倒车 
    int reverse;
};