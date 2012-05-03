#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#define JOY_DEV "/dev/input/js0"
void* read_cont(void* in);

struct joystat
{
   int x1,y1,x2,y2,lt,rt;
   int buts[11];
};

struct joystat joy;
int stop=0;
int newdat=0;
pthread_mutex_t  read_mux;
pthread_mutex_t  data_mux;  
int joy_fd=0;
int main()
{

   pthread_t read_thread;
   pthread_mutex_init(&read_mux,NULL);
   pthread_mutex_init(&data_mux,NULL);
   pthread_create(&read_thread, NULL , read_cont, NULL);
   while(1)
         {
           //while(newdat!=1){}

           //printf("ax %d ,%d,%d,%d,%d,%d  but ,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n", joy.x1, joy.y1,joy.x2,joy.y2,joy.lt,joy.rt,joy.buts[0],joy.buts[1],joy.buts[2],joy.buts[3],joy.buts[4],joy.buts[5],joy.buts[6],joy.buts[7],joy.buts[8],joy.buts[9],joy.buts[10]);

           /*pthread_mutex_lock(&read_mux);
           newdat=0;  
           pthread_mutex_unlock(&read_mux); */
         }
   close( joy_fd );   /*too bad we never get here */
   return 0;
}


void* read_cont(void* in)
   {
     int  *axis=NULL, num_of_axis=0, num_of_buttons=0, k;
   char *button=NULL, name_of_joystick[80];
   struct js_event js;

   if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
   {
      printf( "Couldn't open joystick\n" );
      return NULL;
   }

   ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
   ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
   ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

   axis = (int *) calloc( num_of_axis, sizeof( int ) );
   button = (char *) calloc( num_of_buttons, sizeof( char ) );

   printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
      , name_of_joystick
      , num_of_axis
      , num_of_buttons );

   fcntl( joy_fd, F_SETFL, O_NONBLOCK );  /* use non-blocking mode */
   while( stop==0 )   /* infinite loop */
      {
         /* read the joystick state */
      read(joy_fd, &js, sizeof(struct js_event));
      
         /* see what to do with the event */
      switch (js.type & ~JS_EVENT_INIT)
      {
         case JS_EVENT_AXIS:
            axis   [ js.number ] = js.value;
            break;
         case JS_EVENT_BUTTON:
            button [ js.number ] = js.value;
            break;
      }

         /* print the results */
      /*printf( "X1: %6d  Y1: %6d  ", axis[0], axis[1] );
      
      if( num_of_axis > 2 )
         printf("Z: %6d  ", axis[2] );
         
      if( num_of_axis > 3 )
         printf("R: %6d  ", axis[3] );

      if( num_of_axis > 4 ) 
                        printf("R: %6d  ", axis[4] );

      if( num_of_axis > 5 )
                        printf("R: %6d  ", axis[5] );

      for(  k=0 ; k<num_of_buttons ; ++k )
         printf("B%d: %d  ", k, button[k] );    */           
     // pthread_mutex_lock(&data_mux); 
      joy.x1=axis[0];
      joy.y1=axis[1];
      joy.x2=axis[2];
      joy.y2=axis[3];
      joy.lt=axis[4];
      joy.rt=axis[5];
         
      for( k=0 ; k<num_of_buttons ; ++k )
            {
            joy.buts[k]=button[k];   
            }
                 printf("ax %d ,%d,%d,%d,%d,%d  but ,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n", joy.x1, joy.y1,joy.x2,joy.y2,joy.lt,joy.rt,joy.buts[0],joy.buts[1],joy.buts[2],joy.buts[3],joy.buts[4],joy.buts[5],joy.buts[6],joy.buts[7],joy.buts[8],joy.buts[9],joy.buts[10]);
      
      //pthread_mutex_unlock(&data_mux);
      //pthread_mutex_lock(&read_mux);
      //newdat=1;
      //pthread_mutex_unlock(&read_mux);                 
      fflush(stdout);


      }

   close( joy_fd );   /*too bad we never get here */
   }





