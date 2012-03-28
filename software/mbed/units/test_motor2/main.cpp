#include "mbed.h"
#include "motor.h"

#define RPM_TICKS_PER_REVOLUTION    2
#define RPM_POLL_TIME               1000   //in ms

DigitalOut hb_light(LED1);
Serial pc(USBTX,USBRX);
Motor m(p23);
InterruptIn pulse(p17);

Ticker hb_ticker;
Ticker rpm_ticker;
unsigned int ticks = 0;

void hb() {
    hb_light = !hb_light;
}

void count() {
    ++ticks;
}

void print_rpm() {
    //Copy ticks and reset so that we can catch new interrupts
    //without effecting this time period calculation
    double t = (double)ticks;
    ticks = 0;
    
    //Use dimensional analysis to get rps
    //
    //  ticks           1         1000 ms      revs
    // --------- * ----------- * --------- =  ------
    //  poll ms     ticks/rev       1 s         s
    //
    // Which gives
    //
    //     ticks * 1000 ms
    // ------------------------ =  rps
    //   poll ms * ticks/rev
    double rps = (t*1000)/(RPM_POLL_TIME*RPM_TICKS_PER_REVOLUTION);
    
    //Use dimensional analysis again to get rpm
    double rpm = rps*60;
    
    pc.printf("Ticks: %0.2f        RPS: %0.2f        RPM: %0.2f\r\n",t,rps,rpm);
}

int main() {

    double rate = 0.1;

    pc.baud(115200);
    pc.printf("Initializing...\r\n");

    hb_ticker.attach(hb,1.0);
    rpm_ticker.attach_us(print_rpm,RPM_POLL_TIME*1000);
    
    pulse.rise(&count);
    
    pc.printf("Starting Motor at Idle...\r\n");
    m.idle();


    while(1) {
        //pc.scanf("%s",buf);
        char c = pc.getc();
        if(c == '!') {
            m.kill();
            pc.printf("killed motor\r\n");
        } else if(c == 'f') {
            m.full_throttle();
            pc.printf("full throttled motor\r\n");
        } else if(c == 'i') {
            m.idle();
            pc.printf("idled motor\r\n");
        } else if(c == '+') {
            m += rate;
            pc.printf("motor speed: %f\r\n",m.read());
        } else if(c == '-') {
            m -= rate;
            pc.printf("motor speed: %f\r\n",m.read());
        } else if(c == '1') {
            rate = 0.1;
            pc.printf("motor adjust rate: %f\r\n",rate*100);
        } else if(c == '2') {
            rate = 0.01;
            pc.printf("motor adjust rate: %f\r\n",rate*100);
        } else {
            pc.printf("invalid\r\n");
        }
    }
}
