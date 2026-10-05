#include <iostream>
#include <random>
#include "TrafficLight.h"



/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
    _gen.seed(std::random_device{}());
    _dist = std::uniform_int_distribution<>(4000, 6000);    
    _cycleDuration = std::chrono::milliseconds(_dist(_gen));
}

TrafficLight::~TrafficLight()
{
    // destructor
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true)
    {
        TrafficLightPhase phase = _messageQueue.receive();

        if (phase == TrafficLightPhase::green)
        {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    auto cycleThread = std::thread(&TrafficLight::cycleThroughPhases, this);
    threads.emplace_back(std::move(cycleThread));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    std::chrono::milliseconds timeSinceLastUpdate = std::chrono::milliseconds(0);
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (timeSinceLastUpdate < _cycleDuration)
        {
            timeSinceLastUpdate += std::chrono::milliseconds(1);
            continue;
        }

        timeSinceLastUpdate = std::chrono::milliseconds(0);        
        
        // toggle current phase of traffic light
        if (_currentPhase == TrafficLightPhase::red)
        {
            _currentPhase = TrafficLightPhase::green;
            _messageQueue.send(std::move(_currentPhase));
        }
        else
        {
            _currentPhase = TrafficLightPhase::red;
            _messageQueue.send(std::move(_currentPhase));
        }
    }
}

