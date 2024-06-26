#ifndef pid_h
#define pid_h

#include <functional>
#include <cmath>
#include "global_defines.h"

template <typename ValueType, typename outType, outType MaxOut, outType MinOut, int32_t maxErrorSum, int32_t minErrorSum, uint32_t samplingMs = 20>
class pid{
public:
    /*
        Kp - proportional Gain
        Ti - integrator constant
        Td - derivative constant
        Ts - sampling time in ms
    */

    constexpr pid(float Kp, float Ti, float Td) noexcept:
        m_Kp{ Kp }, m_Ki{ (Kp / Ti) * (samplingMs * 0.001f) }, m_Kd{ (Kp * Td)/(samplingMs * 0.001f + 0.1f*Td) },
        m_errorSum{ 0 }, m_previousError{ 0 } {}
    
    void reinit(float Kp, float Ti, float Td){
        m_Kp = Kp;
        m_Ki = (Kp / Ti) * (samplingMs * 0.001f);
        m_Kd = (Kp * Td)/((samplingMs * 0.001f) + 0.1f*Td);
        m_errorSum = m_previousError = 0;
    }

    outType getOutput(ValueType actual, ValueType desired){
        auto error = desired - actual;
        float P = m_Kp * error;
        
        m_errorSum += static_cast<int32_t>(error);

        if(m_errorSum > maxErrorSum){
            m_errorSum = maxErrorSum;
        }
        else if(m_errorSum < minErrorSum){
            m_errorSum = minErrorSum; 
        }

        float I = m_Ki * m_errorSum;
        float D = m_Kd * (error - m_previousError);

        auto output = static_cast<outType>(P + I + D);

        if(output > MaxOut){
            return MaxOut;
        }
        else if(output < MinOut){
            return MinOut;
        }
        else{
            return output;
        }
    }

private:
    float m_Kp;
    float m_Ki;
    float m_Kd;

    ValueType m_errorSum;
    ValueType m_previousError;
};

#endif