#include "../include/SensorManager.h"

namespace Sensor{

SensorManager::SensorManager(std::array<sensorPointer, numSensors>& s)
    : sensors{ std::move(s) } { }

void SensorManager::init(){
    for(auto& s : sensors){
        s->init();
    }
}

const void* SensorManager::getReadings(SensorMapping s){
    auto sensor_index = translate(s);

    return sensors[sensor_index]->getReadings();
}

void SensorManager::poolSensors(){
    for(auto& s : sensors){
        s->poolSensor();
    }
}

}