#pragma once
#define MAGIC_NO 35712
#define VERSION "0.1.0"

struct MessageHeader {
    uint32_t magic;
    char version[8];
    uint32_t msg_len;
    uint32_t msg_type;
    uint32_t sim_frame;
    double sim_time;
};

struct AccelerometerMessage
{
    double x;
    double y;
    double z;
};
struct GyroscopeMessage
{
    double x;
    double y;
    double z;
};

struct LocationMessage{
    double x;
    double y;
    double z;
};

struct RotationMessage{
    double roll;
    double yaw;
    double pitch;
};

struct SpeedMessage{
    double x;
    double y;
    double z;
};

struct AccelerationMessage{
    double x;
    double y;
    double z;
};

struct AngularVelocityMessage{
    double x;
    double y;
    double z;
};

struct QuaternionMessage{
    double qx;
    double qy;
    double qz;
    double qw;
};

struct LocalizationMessage{
    MessageHeader header;
    double heading;
    LocationMessage location;
    RotationMessage rotation;
    SpeedMessage speed;
    AccelerationMessage acceleration;
    AngularVelocityMessage angular_velocity;
    QuaternionMessage quaternion;
};

struct GnssMessage{
    MessageHeader header;

    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;


};

struct ImuMessage{
    MessageHeader header;
    AccelerometerMessage accelerometer; // Measures linear acceleration. m/s^2
    GyroscopeMessage gyroscope;     // Measures angular velocity. rad/s
    float heading;         // Orientation in radians. North is 0 radians.
};

struct CameraMessage {
    MessageHeader header;
    uint32_t width;
    uint32_t height;
    uint32_t length;
    /* data */
};
struct LidarPointMessage{
    float x;
    float y;
    float z;
    float i;
};

struct LidarMessage {
    MessageHeader header;
    double horizontal_angle;
    int channels;
    int point_count;
    uint32_t point_size;
    /* data */
};
struct RadarPointMessage{
    float altitude;
    float azimuth;
    float depth;
    float velocity;
};

struct RadarMessage {
    MessageHeader header;
    int point_count;
    uint32_t point_size;
    /* data */
};


struct DimensionMessage
{
    double x;
    double y;
    double z;
};

enum ObjectType{
    None = 0,
    Car = 1
};

struct ObjectMessage{
    int object_type;
    LocationMessage relative_location;
    SpeedMessage speed;
    RotationMessage rotation;
    AccelerationMessage acceleration;
    DimensionMessage dimension;
    float heading;
};

struct GroundTrueObjectMessage{
    MessageHeader header;
    double range;
    uint32_t object_count;
    /* data */
};

enum GearPosition {
    GEAR_NEUTRAL = 0,
    GEAR_DRIVE = 1,
    GEAR_REVERSE = 2,
    GEAR_PARKING = 3,
    GEAR_LOW = 4,
    GEAR_INVALID = 5,
    GEAR_NONE = 6,
};

struct ChassisMessage{
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

struct ControlMessage {
    MessageHeader header;

    double throttle;
    double brake;
    double steer;
    int reverse;
};

struct AutonomousVehicleConfigMessage{
    MessageHeader header;
};


