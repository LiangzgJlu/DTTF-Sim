//
// Created by liangzg on 25-1-7.
//

#pragma once

#include <cstdint>
#include "message.h"


struct Quaternion {
    double w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0) {}

    Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    // 四元数乘法
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }
};

// apollo 欧拉角转四元数
Quaternion eulerToQuaternion(double yaw, double roll, double pitch);


void carlaToApolloEuler(double carlaYaw, double carlaPitch, double carlaRoll, double& apolloYaw, double& apolloPitch, double& apolloRoll);

void LatLonToUTM(double lat, double lon, double& easting, double& northing, int& zone);