// IMU for Sparkfun 9DOF Sensor Stick

#include "mbed.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "HMC5883L.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
ADXL345 accel(p9,p10);
ITG3200 gyro(p9, p10);
HMC5883L compass(p9, p10);
Serial pc(USBTX, USBRX);

#define pi       3.14159265

#define N        3
#define N_LWMA   10

double* calib();
double* RK4( double, double[N], double[N] );
double* func( double[N], double[N] );
double* LWMA( double[N] );
double* EKF_predict( double[N], double[N] );
double* EKF_correct( double[N], double[N], double[N] );

//             0       1       2
//        [ accXn-1 accXn-2   ...   ] 0
// zBuf = [ accYn-1 accYn-2   ...   ] 1
//        [ accZn-1 accZn-2   ...   ] 2
double z_buf[N][N_LWMA]    =    {0};                // For LWMA

double P[N][N]    =    {{1,0,0},{0,1,0},{0,0,1}};    // For EKF

int main(){ 
    
    pc.baud(115200);
    
    short int bit_acc[N]   =   {0};    // Buffer of the accelerometer
    int get_mag[N]   =   {0};    // Buffer of the compass
    
    // Calibration routine
    double calib_acc[N]         =   {0};
    double calib_gyro[N]        =   {0};
    
    
    // Getting data
    double acc[N]       =   {0};
    double gyro_deg[N]  =   {0};
    double gyro_rad[N]  =   {0};
    int    mag[N]       =   {0};
    
    
    for( int i=0;i<N_LWMA;i++ ){    z_buf[2][i]    =    1;    }
    
    double* p_calib;
    
    
    
    // ***  Setting up sensors ***    
    accel.set_range(0,true);
    accel.set_measurement_mode();
    gyro.setLpBandwidth(LPFBW_42HZ);
    compass.setDefault();
    wait(0.1);                        // Wait some time for all sensors (Need at least 5ms)
    
    // *** Calibration routine ***
    p_calib = calib();
    for( int i=0;i<3;i++ ){     calib_acc[i]    =   *p_calib;   p_calib = p_calib+1;    }
    for( int i=3;i<6;i++ ){     calib_gyro[i-3] =   *p_calib;   p_calib = p_calib+1;    }

    //pc.printf("Starting IMU unit\n\r");
    //pc.printf("   Time     AccX     AccY     AccZ    GyroX    GyroY    GyroZ   MagX   MagY   MagZ\n\r");
    while(1){
        
        // Updating accelerometer and compass
        accel.get_data_x_raw(bit_acc[0]); accel.get_data_y_raw(bit_acc[1]); accel.get_data_z_raw(bit_acc[2]);
        
        compass.readData(get_mag);
        
        // Transfering units (Acc[g], Gyro[deg/s], Compass[Ga])
        acc[0]      =  (double)((int16_t)bit_acc[0]-calib_acc[0])*0.004;
        acc[1]      =  (double)((int16_t)bit_acc[1]-calib_acc[1])*0.004;
        acc[2]      =  (double)((int16_t)bit_acc[2]-calib_acc[2])*0.004+1;
        
        gyro_deg[0] = (gyro.getGyroX()-calib_gyro[0])/14.375;
        gyro_deg[1] = -(gyro.getGyroY()-calib_gyro[1])/14.375;          // Modify the differencial of the sensor axis 
        gyro_deg[2] = -(gyro.getGyroZ()-calib_gyro[2])/14.375;
                
        for( int i=0;i<N;i++ ){     mag[i] = (int16_t)get_mag[i];   }
        for( int i=0;i<N;i++ ){     if( acc[i]<-2 ){    acc[i] = -2;    }  }
        for( int i=0;i<N;i++ ){     if( acc[i]>2 ){    acc[i] = 2;    }  }
                
        for( int i=0;i<N;i++ ){    gyro_rad[i] = gyro_deg[i]*pi/180;    }
       
        pc.printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
            gyro_rad[0],gyro_rad[1],gyro_rad[2],
            acc[0],acc[1],acc[2],
            (double)mag[0],(double)mag[1],(double)mag[2]
        );
       
       wait(1.0/512.0);
    }
}



double* calib(){

    int calib_loop  =   1000;

    short int bit_acc[N]   =   {0};    // Buffer of the accelerometer
    int get_mag[N]   =   {0};    // Buffer of the compass

    double calib_acc[N]         =   {0};
    double calib_gyro_buf[N]    =   {0};
    double calib_gyro[N]        =   {0};
    double compass_basis_buf[N] =   {0};
    double compass_basis_rad    =   0;
    double calib_result[7]      =   {0};

    double* p_calib_result      =   calib_result;

    //pc.printf("\n\r\n\rDon't touch... Calibrating now!!\n\r");
    led1    =   1;
    
    for( int i=0;i<calib_loop;i++ ){
        accel.get_data_x_raw(bit_acc[0]); accel.get_data_y_raw(bit_acc[1]); accel.get_data_z_raw(bit_acc[2]);
        compass.readData(get_mag);
        
        calib_gyro_buf[0]   =   gyro.getGyroX();
        calib_gyro_buf[1]   =   gyro.getGyroY();
        calib_gyro_buf[2]   =   gyro.getGyroZ();
        
        for( int j=0;j<N;j++ ){
            calib_acc[j]            +=  (int16_t)bit_acc[j];
            calib_gyro[j]           +=  calib_gyro_buf[j];
            compass_basis_buf[j]    +=  (int16_t)get_mag[j];
        }
        
        if( i>calib_loop*3/4 ){
             led4   =   1;
        }else if( i>calib_loop/2 ){
            led3    =   1;
        }else if( i>calib_loop/4 ){
            led2    =   1;
        }
        
    }
    
    for( int i=0;i<N;i++ ){
        calib_acc[i]            =   calib_acc[i]/calib_loop;
        calib_gyro[i]           =   calib_gyro[i]/calib_loop;
        compass_basis_buf[i]    =   compass_basis_buf[i]/calib_loop;
    }
    
    compass_basis_rad   =   compass_basis_buf[1]/compass_basis_buf[0];
    compass_basis_rad   =   atan(compass_basis_rad);
    led1 = 0;    led2 = 0;  led3 = 0;   led4    =   0;
    
    //pc.printf("  accX    accY    accZ   gyroX   gyroY   gyroZ    yaw_basis\n\r");
    //pc.printf("%6.1f, %6.1f, %6.1f, %6.1f, %6.1f, %6.1f, %6.1f\n\r",calib_acc[0],calib_acc[1],calib_acc[2],calib_gyro[0],calib_gyro[1],calib_gyro[2],compass_basis_rad*180/pi);
    
    for( int i=0;i<3;i++ ){     calib_result[i] =   calib_acc[i];    }
    for( int i=3;i<6;i++ ){     calib_result[i] =   calib_gyro[i-3];    }    
    calib_result[6] =  compass_basis_rad;
    
    for( int i=0;i<3;i++ ){
        wait(0.5);
        led1 = 1;    led2 = 1;  led3 = 1;   led4    =   1;
        wait(0.5);
        led1 = 0;    led2 = 0;  led3 = 0;   led4    =   0;
    }

    return p_calib_result;

}


