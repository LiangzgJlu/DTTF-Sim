#pragma once
#define MAGIC_NO 35712
#define VERSION "0.1.0"

/**
 * \brief Message Header
 * 
 */
struct MessageHeader {
    // Message identification flag
    uint32_t magic;
    // Protocol version 
    char version[8];
    // Message length
    uint32_t msg_len;
    // Message type (redundant)
    uint32_t msg_type;
    // Simulation frame
    uint32_t sim_frame;
    // Simulation time
    double sim_time;
};

/**
 * \brief Accelerometer Message
 * 
 */
struct AccelerometerMessage
{
    double x;
    double y;
    double z;
};

/**
 * \brief Gyroscope Message
 * 
 */
struct GyroscopeMessage
{
    double x;
    double y;
    double z;
};

/**
 * \brief Location Message
 * 
 */
struct LocationMessage{
    double x;
    double y;
    double z;
};

/**
 * \brief Rotation Message
 * 
 */
struct RotationMessage{
    double roll;
    double yaw;
    double pitch;
};

/**
 * \brief Speed Message, m/s
 * 
 */
struct SpeedMessage{
    double x;
    double y;
    double z;
};

/**
 * \brief Acceleration Message, m/s^2
 * 
 */
struct AccelerationMessage{
    double x;
    double y;
    double z;
};

/**
 * \brief Angular Velocity Message, rad/s
 * 
 */
struct AngularVelocityMessage{
    double x;
    double y;
    double z;
};

/**
 * \brief Quaternion Message
 * 
 */
struct QuaternionMessage{
    double qx;
    double qy;
    double qz;
    double qw;
};

/**
 * \brief Localization Message
 * 
 */
struct LocalizationMessage{
    // Message header
    MessageHeader header;
    // Heading angle (degrees)
    double heading;
    // Location information (x: longitude, y: latitude, z: altitude)                       
    LocationMessage location;
    // Rotation information
    RotationMessage rotation;
    // Speed information
    SpeedMessage speed;
    // Acceleration information
    AccelerationMessage acceleration;
    // Angular velocity information
    AngularVelocityMessage angular_velocity;
    // Quaternion information
    QuaternionMessage quaternion;
};

/**
 * \brief GNSS Message
 * 
 */
struct GnssMessage{
    // Message header
    MessageHeader header;

    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
};

/**
 * \brief IMU Message
 * 
 */
struct ImuMessage{
    // Message header
    MessageHeader header;
    // Accelerometer message
    AccelerometerMessage accelerometer; // Measures linear acceleration. m/s^2
    // Gyroscope message
    GyroscopeMessage gyroscope;     // Measures angular velocity. rad/s
    // Heading angle
    float heading;         
};

/**
 * \brief Camera Message RGB8
 * 
 */
struct CameraMessage {
    // Message header
    MessageHeader header;
    // Image width
    uint32_t width;
    // Image height
    uint32_t height;
    // Image byte length
    uint32_t length;
    /* data */
    // RGB8 data is at the end of CameraMessage, with a byte length of `length`.
};

/**
 * \brief LiDAR Point Message
 * 
 */
struct LidarPointMessage{
    float x;
    float y;
    float z;
    float i;
};

/**
 * \brief LiDAR Message
 * 
 */
struct LidarMessage {
    // Message header
    MessageHeader header;
    // Horizontal field of view
    double horizontal_angle;
    // Number of channels
    int channels;
    // Number of LiDAR points
    int point_count;
    // LiDAR point byte size
    uint32_t point_size;
    /* data */
    // LiDAR point cloud data is at the end of LidarMessage, with a byte length of `point_count * point_size`.
};

/**
 * \brief Radar Point Message
 * 
 */
struct RadarPointMessage{
    // Elevation angle, unit: rad
    float altitude;
    // Azimuth angle, unit: rad
    float azimuth;
    // Distance, unit: m
    float depth;
    // Velocity towards Ego, unit: m/s
    float velocity;
};

/**
 * \brief Radar Message
 * 
 */
struct RadarMessage {
    // Message header
    MessageHeader header;
    // Number of radar points
    int point_count;
    // Radar point size
    uint32_t point_size;
    /* data */
    // Radar point cloud data is at the end of RadarMessage.
};

/**
 * \brief Vehicle Dimension Message
 * 
 */
struct DimensionMessage
{
    // Length, unit: m 
    double x;
    // Width, unit: m 
    double y;
    // Height, unit: m 
    double z;
};

// Object type information
enum ObjectType{
    // Unknown
    None = 0,
    // Car
    Car = 1,
    // Pedestrian
    Pedestrian = 2
};

/**
 * \brief Object Message
 * 
 */
struct ObjectMessage{
    // Object ID, continuous
    uint32_t id;
    // Object type
    ObjectType object_type;
    // Object location information (x: longitude, y: latitude, z: altitude)
    LocationMessage location;
    // Object speed information
    SpeedMessage speed;
    // Object rotation
    RotationMessage rotation;
    // Object acceleration
    AccelerationMessage acceleration;
    // Object dimensions
    DimensionMessage dimension;
    // Object heading
    float heading;
};

/**
 * \brief Ground Truth Object List Message
 * 
 */
struct GroundTrueObjectMessage{
    MessageHeader header;
    double range;
    uint32_t object_count;
    /* data */
    // Object list message is at the end of GroundTrueObjectMessage.
};

// Gear position status
enum GearPosition {
    // Neutral gear
    GEAR_NEUTRAL = 0,
    // Drive gear
    GEAR_DRIVE = 1,
    // Reverse gear
    GEAR_REVERSE = 2,
    // Parking gear
    GEAR_PARKING = 3,
    GEAR_LOW = 4,
    GEAR_INVALID = 5,
    GEAR_NONE = 6,
};

/**
 * \brief Chassis Message
 * 
 */
struct ChassisMessage{
    // Message header
    MessageHeader header;
    // Engine RPM
    double engine_rpm;
    // Speed
    double speed;
    // Odometer (meters)
    double odometer_m;
    // Fuel range (meters)
    double fuel_range_m;
    // Throttle percentage [0-100]
    double throttle_percentage;
    // Brake percentage [0-100]
    double brake_percentage;
    // Steering wheel angle [-100, 100] Clockwise: negative, Counter-clockwise: positive
    double steering_percentage;
    // Parking brake status
    int  parking_brake;
    int gear_location;
    // Throttle percentage command [0-100]
    double throttle_percentage_cmd;
    // Brake percentage command [0-100]
    double brake_percentage_cmd;
    // Steering wheel angle command [-100, 100] Clockwise: negative, Counter-clockwise: positive
    double steering_percentage_cmd;
};

/**
 * \brief Control Message
 * 
 */
struct ControlMessage {
    MessageHeader header;

    // Throttle, [0-1]
    double throttle;
    // Brake, [0-1]
    double brake;
    // Steering angle, [-1-1], Left: negative; Right: positive
    double steer;
    // Reverse gear, 0: forward; 1: reverse 
    int reverse;
};

/**
 * \brief Autonomous Vehicle Configuration Message
 * 
 */
struct AutonomousVehicleConfigMessage{
    // Message header
    MessageHeader header;
    /* data */
    // Configuration message is at the end of AutonomousVehicleConfigMessage.
};