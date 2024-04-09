#ifndef global_h
#define global_h

#define WIFI_DEBUG 1
#define MQTT_DEBUG 1

#include "Arduino.h"
#include <array>
#include <functional>

namespace constants{
    constexpr uint32_t subscribedTopics = 2;
    constexpr uint32_t jobStackDepth = 10;

    // cant make constexpr because String is not constexpr
    const std::array<String, subscribedTopics> topicsArray{
        { "robot/debug/input", "robot/request" }
    };

namespace types{
    struct job_t{
        std::function<void(const String&)> cback;
        String payload;
    };

namespace mapping{
    enum topic_mapping : uint32_t{
        debugInfo = 0,
        request
    };
}
}
}
#endif