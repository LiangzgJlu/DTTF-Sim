#pragma once

#include "cyber/cyber.h"

namespace apollo {
namespace dttf {
namespace drivers{

using apollo::cyber::Node;

class DriverBase{
public :
    DriverBase() = default;
    virtual ~DriverBase() = default;
    virtual bool Init(const std::shared_ptr<Node>& node) = 0;
};

}
}
}