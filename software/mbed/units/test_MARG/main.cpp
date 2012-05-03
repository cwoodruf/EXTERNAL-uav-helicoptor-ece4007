#include "mbed.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "HMC5883L.h"
#include "ahrs2.h"

#define RATE 0.01

DigitalOut hb_light(LED1);
DigitalOut l2(LED2);
DigitalOut l3(LED3);
DigitalOut l4(LED4);
Serial pc(USBTX,USBRX);

Ticker hb_ticker;
Ticker algorithm;

ADXL345 accel(p9,p10);
ITG3200 gyro(p9,p10);
HMC5883L magn(p9,p10);
//AHRS ahrs(RATE,Quaternion(1,Vector3(0,0,0)),1);
AHRS ahrs(RATE,0.001);

double cal_a[3] = {0};
double cal_g[3] = {0};
double cal_m[3] = {0};

void hb() {
    hb_light = !hb_light;
}

void alg() {
    short int ax,ay,az;
    short int gx,gy,gz;
    short int mx,my,mz;
    accel.get_data_x_raw(ax); accel.get_data_y_raw(ay); accel.get_data_z_raw(az);
    gyro.get_x_out(gx); gyro.get_y_out(gy); gyro.get_z_out(gz);
    magn.get_data_x_raw(mx); magn.get_data_y_raw(my); magn.get_data_z_raw(mz);
    
    Vector3 a(
        (double)(ax-cal_a[0])*0.004,
        (double)(ay-cal_a[1])*0.004,
        (double)(az-cal_a[2])*0.004
    );
    a[0] = (a[0] > 2.0) ? 2.0 : (a[0] < -2.0 ? -2.0 : a[0]);
    a[1] = (a[1] > 2.0) ? 2.0 : (a[1] < -2.0 ? -2.0 : a[1]);
    a[2] = (a[2] > 2.0) ? 2.0 : (a[2] < -2.0 ? -2.0 : a[2]);
    
    Vector3 g(
        ((gx-cal_g[0])/14.375)*M_PI/180,
        ((gy-cal_g[1])/14.375)*M_PI/180,
        ((gz-cal_g[2])/14.375)*M_PI/180
    );
    
    Vector3 m(mx,my,mz);
    
    ahrs.update(g,a);
    Vector3 angles = ahrs.get_angles();
    
    pc.printf("%f,%f,%f\r\n",angles[0],angles[1],angles[2]);
}

void calibrate() {

    short int ax,ay,az;
    short int gx,gy,gz;
    short int mx,my,mz;
    double a[3] = {0.0};
    double g[3] = {0.0};
    double m[3] = {0.0};
    
    for(int i=0;i<1000;++i) {
        accel.get_data_x_raw(ax); accel.get_data_y_raw(ay); accel.get_data_z_raw(az);
        gyro.get_x_out(gx); gyro.get_y_out(gy); gyro.get_z_out(gz);
        magn.get_data_x_raw(mx); magn.get_data_y_raw(my); magn.get_data_z_raw(mz);
    
        a[0] += (double)ax; a[1] += (double)ay; a[2] += (double)az;
        g[0] += (double)gx; g[1] += (double)gy; g[2] += (double)gz;
        m[0] += (double)mx; m[1] += (double)my; m[2] += (double)mz;
       
        if(i == 750) {
            l4 = 1;
        } else if(i == 500) {
            l3 = 1;
        } else if(i == 250) {
            l2 = 1; 
        } else  if(i == 0) {
            hb_light = 1;
        }
    }
    
    cal_a[0] = a[0] / 1000; cal_a[1] = a[1] / 1000;  cal_a[2] = a[2] / 1000;
    cal_g[0] = g[0] / 1000; cal_g[1] = g[1] / 1000;  cal_g[2] = g[2] / 1000;
    cal_m[0] = m[0] / 1000; cal_m[1] = m[1] / 1000;  cal_m[2] = m[2] / 1000;
    
    for(int i=0;i<3;++i) {
        hb_light = 1; l2 = 1; l3 = 1; l4 = 1;
        wait(0.5);
        hb_light = 0; l2 = 0; l3 = 0; l4 = 0;
        wait(0.5);
    }
}

void setup() {
    //Communication Interfaces Setup
    pc.baud(115200);
    
    //Accelerometer Setup
    accel.set_range(0,false);
    accel.set_measurement_mode();
    
    //Gyro Setup
    
    //Magnetometer Setup
    magn.set_config_a(HMC5883L_SAMPLES_AVG_1,HMC5883L_OUTPUT_RATE_15_0,HMC5883L_MEASURE_MODE_NORM);
    magn.set_config_b(HMC5883L_GAIN_1090);
    magn.set_measurement_mode(HMC5883L_MODE_CONTINUOUS_MEASUREMENT);
    
    //Calibrate
    calibrate();
    
    //Interrupts Setup
    hb_ticker.attach(hb,1.0);
    algorithm.attach(alg,RATE);
}

int main() {

    setup();
    
    while(1) {
        wait(0.2);
    }
}
