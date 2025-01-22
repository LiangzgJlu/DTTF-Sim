#include <cmath>
#include <array>
#include "common.h"
std::array<std::array<double, 3>, 3> rotateX(double angle) {
    return {{
        {1, 0, 0},
        {0, cos(angle), -sin(angle)},
        {0, sin(angle), cos(angle)}
    }};
}

std::array<std::array<double, 3>, 3> rotateY(double angle) {
    return {{
        {cos(angle), 0, sin(angle)},
        {0, 1, 0},
        {-sin(angle), 0, cos(angle)}
    }};
}

std::array<std::array<double, 3>, 3> rotateZ(double angle) {
    return {{
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0, 0, 1}
    }};
}

// 矩阵乘法
std::array<std::array<double, 3>, 3> multiplyMatrices(const std::array<std::array<double, 3>, 3>& A, const std::array<std::array<double, 3>, 3>& B) {
    std::array<std::array<double, 3>, 3> result = {{{0}}};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

// 将欧拉角转换为旋转矩阵（YZX顺序）
std::array<std::array<double, 3>, 3> eulerToMatrixYZX(double pitch, double yaw, double roll) {
    auto R_y = rotateY(pitch);
    auto R_z = rotateZ(yaw);
    auto R_x = rotateX(roll);
    
    auto result = multiplyMatrices(R_y, R_z);
    result = multiplyMatrices(result, R_x);
    return result;
}

// 欧拉角到旋转矩阵（ZXY顺序）
std::array<std::array<double, 3>, 3> eulerToMatrixZXY(double yaw, double roll, double pitch) {
    auto R_z = rotateZ(yaw);
    auto R_x = rotateX(roll);
    auto R_y = rotateY(pitch);
    
    auto result = multiplyMatrices(R_z, R_x);
    result = multiplyMatrices(result, R_y);
    return result;
}

// 提取旋转矩阵中的欧拉角（假设没有gimbal lock）
void matrixToEuler(const std::array<std::array<double, 3>, 3>& matrix, double& yaw, double& roll, double& pitch) {
    yaw = atan2(matrix[1][0], matrix[0][0]);
    roll = atan2(matrix[2][1], matrix[2][2]);
    pitch = atan2(-matrix[2][0], sqrt(matrix[0][0]*matrix[0][0] + matrix[1][0]*matrix[1][0]));
}

// 进行坐标系反转并转换为Apollo的欧拉角
void carlaToApolloEuler(double carlaYaw, double carlaPitch, double carlaRoll, double& apolloYaw, double& apolloPitch, double& apolloRoll) {
    // Carla欧拉角是YZX顺序，使用YZX顺序计算旋转矩阵
    auto carlaMatrix = eulerToMatrixYZX(carlaPitch, carlaYaw, carlaRoll);
    
    // 坐标系反转：Y和Z反转
    std::array<std::array<double, 3>, 3> flipYZ = {{
        {1, 0, 0},
        {0, -1, 0},
        {0, 0, -1}
    }};
    
    // 反转后的矩阵
    auto flippedMatrix = multiplyMatrices(flipYZ, carlaMatrix);
    
    // 转换为Apollo的ZXY顺序欧拉角
    matrixToEuler(flippedMatrix, apolloYaw, apolloRoll, apolloPitch);
}

Quaternion eulerToQuaternion(double yaw, double roll, double pitch) {
    // 计算每个轴的旋转四元数
    Quaternion q_z(cos(yaw / 2), 0, 0, sin(yaw / 2));  // 绕Z轴旋转
    Quaternion q_x(cos(roll / 2), sin(roll / 2), 0, 0); // 绕X轴旋转
    Quaternion q_y(cos(pitch / 2), 0, sin(pitch / 2), 0); // 绕Y轴旋转

    // 按照Z -> X -> Y的顺序进行四元数乘法
    Quaternion q = q_z * q_x;
    q = q * q_y;

    return q;
}

// 经纬度转 UTM
void LatLonToUTM(double lat, double lon, double& easting, double& northing, int& zone) {
    // 地球参数
    const double EQUATORIAL_RADIUS = 6378137.0;  // 赤道半径 (米)
    const double FLATTENING = 1.0 / 298.257223563;  // 扁率
    const double ECC2 = 2 * FLATTENING - FLATTENING * FLATTENING;  // 第一偏心率的平方
    const double K0 = 0.9996;  // 缩放因子
    // 计算区带 (Zone)
    zone = static_cast<int>((lon + 180.0) / 6.0) + 1;

    // 经度对应中央经线 (度)
    double lon_origin = (zone - 1) * 6.0 - 180.0 + 3.0;

    // 转换纬度经度为弧度
    double lat_rad = lat * M_PI / 180.0;
    double lon_rad = lon * M_PI / 180.0;
    double lon_origin_rad = lon_origin * M_PI / 180.0;

    // 计算辅助量
    double N = EQUATORIAL_RADIUS / sqrt(1 - ECC2 * sin(lat_rad) * sin(lat_rad));
    double T = tan(lat_rad) * tan(lat_rad);
    double C = ECC2 * cos(lat_rad) * cos(lat_rad);
    double A = cos(lat_rad) * (lon_rad - lon_origin_rad);

    // 中央子午线投影参数
    double M = EQUATORIAL_RADIUS * ((1 - ECC2 / 4 - 3 * ECC2 * ECC2 / 64 - 5 * ECC2 * ECC2 * ECC2 / 256) * lat_rad
               - (3 * ECC2 / 8 + 3 * ECC2 * ECC2 / 32 + 45 * ECC2 * ECC2 * ECC2 / 1024) * sin(2 * lat_rad)
               + (15 * ECC2 * ECC2 / 256 + 45 * ECC2 * ECC2 * ECC2 / 1024) * sin(4 * lat_rad)
               - (35 * ECC2 * ECC2 * ECC2 / 3072) * sin(6 * lat_rad));

    // 计算 UTM 坐标
    easting = K0 * N * (A + (1 - T + C) * A * A * A / 6.0 + (5 - 18 * T + T * T + 72 * C - 58 * ECC2) * A * A * A * A * A / 120.0) + 500000.0;
    northing = K0 * (M + N * tan(lat_rad) * (A * A / 2.0 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24.0
                  + (61 - 58 * T + T * T + 600 * C - 330 * ECC2) * A * A * A * A * A * A / 720.0));

    // 如果纬度为南半球，修正 Northing 值
    if (lat < 0) {
        northing += 10000000.0;  // 南半球使用 10,000,000
    }
}