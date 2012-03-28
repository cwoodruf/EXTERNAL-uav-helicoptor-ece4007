#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

//Period in ms
#define MOTOR_PERIOD               21
#define MOTOR_DEFAULT_PIN          21
#define MOTOR_KILL              0.021
#define MOTOR_IDLE              0.001
#define MOTOR_FULL              0.002

class Motor {

    private:
        PwmOut out;
        double percent;
        double pulsewidth;
    
        void scale(double raw) {
            percent = (raw >= 1.0) ? 1.0 : ((raw <= 0.0) ? 0.0 : raw);
            pulsewidth = (percent*0.001 + 0.001);
        }
    
    public:
        //Default Constructor
        //Motor() : out(MOTOR_DEFAULT_PIN) {
        //    out.period_ms(MOTOR_PERIOD);
        //    out.pulsewidth(0.0);
        //}
    
        //Constructor using a pin - must be a PWM pin
        Motor(PinName pin) : out(pin) {
            out.period_ms(MOTOR_PERIOD);
            out.pulsewidth(0.0);
        }
        
        //Kill the motor
        void kill() {
            percent = -0.1;
            pulsewidth = MOTOR_KILL;
            out.pulsewidth(pulsewidth);
        }
        
        //Put the motor in idle
        void idle() {
            percent = 0.0;
            pulsewidth = MOTOR_IDLE;
            out.pulsewidth(pulsewidth);
        }
        
        //Put the motor into full throttle
        void full_throttle() {
            percent = 1.0;
            pulsewidth = MOTOR_FULL;
            out.pulsewidth(pulsewidth);
        }
        
        //Change the motor speed - input is a percentage of power from 0.0 to 1.0
        void write(double p=0.0) {
            if(p < 0.0) {
                this->kill();
                return;
            }
            scale(p);
            out.pulsewidth(pulsewidth);
        }
        
        //Change the motor speed - input is a percentage of power from 0 to 100
        void write(int p=0) {
            if(p < 0) {
                this->kill();
                return;
            }
            double dp = ((double)p)/100;
            scale(dp);
            out.pulsewidth(pulsewidth);
        }
        
        //Returns the current power percentage - output range is 0.0 to 1.0, negative is off
        double read() {
            return percent;
        }
        
        //Equal operator for Motor class
        Motor& operator=(const Motor &rhs) {
            percent = rhs.percent;
            pulsewidth = rhs.pulsewidth;
            this->kill();
            return *this;
        }
        
        //Shorthand for write(double)
        Motor& operator=(const double &rhs) {
            this->write(rhs);
            return *this;
        }
        
        //Shorthand for write(int)
        Motor& operator=(const int &rhs) {
            this->write(rhs);
            return *this;
        }
        
        //Increment the motor speed by 10%
        Motor& operator++() {
            write(percent+0.1);
            return *this;
        }
        
        //Intentially same as ++Motor
        Motor& operator++(int) {
            write(percent+0.1);
            return *this;
        }
        
        //Decrement the motor speed by 10%
        Motor& operator--() {
            double tmp = percent - 0.1;
            write(((tmp < 0.0) ? 0.0: tmp));
            return *this;
        }
        
        //Intentially same as ++Motor
        Motor& operator--(int) {
            double tmp = percent - 0.1;
            write(((tmp < 0.0) ? 0.0: tmp));
            return *this;
        }
        
        //Increase the motor power
        Motor& operator+=(const double &rhs) {
            this->write(percent+rhs);
            return *this;
        }
        
        //Increase the motor power
        Motor& operator+=(const int &rhs) {
            this->write(percent+((double)rhs)/100);
            return *this;
        }
        
        //Decrease the motor power
        Motor& operator-=(const double &rhs) {
            this->write(percent-rhs);
            return *this;
        }
        
        //Decrease the motor power
        Motor& operator-=(const int &rhs) {
            this->write(percent-((double)rhs)/100);
            return *this;
        }
        
        //Shorthand for kill
        void operator !() {
            this->kill();
        }
};

#endif