#pragma once
#include <functional>

namespace TimerState { enum ID { Playing, Paused, Ended }; }
class Timer 
{
    float EXECUTION_TIME;
    float timeLeftOfExecution;
    TimerState::ID timerState;
    bool looping;
    std::vector<std::function<void(Timer* timer)>> onFinished; 

public:
    explicit Timer(float executionTime = 0.0f)
        : EXECUTION_TIME(executionTime),
          timeLeftOfExecution(executionTime),
          timerState(TimerState::Paused),
          looping(false)
    {}

    // Register callback
    void onTimerFinished(std::function<void(Timer*)> callback) noexcept {
        onFinished.push_back(std::move(callback));
    }

    void resume() { if (timerState == TimerState::Paused) timerState = TimerState::Playing; }
    void pause()    { if (timerState == TimerState::Playing) timerState = TimerState::Paused; }
    void end()      
    { 
        timerState = TimerState::Ended; 
        for (auto callback : onFinished)
        {
            if (callback)
                callback(this);
        }

    }

    void reset() {
        timeLeftOfExecution = EXECUTION_TIME;
        timerState = TimerState::Paused;
    }

    void setLooping(bool l) { looping = l; }
    bool getLooping() const { return looping; }

    float getRemainingTime() const { return timeLeftOfExecution; }
    float getInitialTime() const   { return EXECUTION_TIME; }
    TimerState::ID getState() const { return timerState; }

    void update(float dt)
    {
        if (timerState != TimerState::Playing) return;

        timeLeftOfExecution -= dt;
        if (timeLeftOfExecution <= 0.0f)
        {
            end();
            if (looping)
            {
                reset();
                resume();
            }
        }
    }
};


