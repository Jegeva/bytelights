#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

#define PWM_STEP 0x101

#define S1_R 0
#define S1_G 1
#define S1_B 2

#define S2_R 3
#define S2_G 4
#define S2_B 5

#define S3_R 6
#define S3_G 7
#define S3_B 8

#define S4_R 9
#define S4_G 10
#define S4_B 11

#define S5_R 12
#define S5_G 13
#define S5_B 14




uint8_t strip_ccm[15];
uint8_t strip_rgb[15];
float   strip_delta[15];
float   strip_float[15];

/*---------------------------------------------------
  X11
  ---------------------------------------------------*/
#ifdef X11_SHOW

Display *d;
Window w,root;
int s;
XVisualInfo* visinfo;

Colormap cmap;
XColor xcolour;

#define WIN_SZ 600

int createwin(){
  XEvent e;
  const char *msg = "Hello, World!";
  int s;
  d = XOpenDisplay(NULL);
  if (d == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, WIN_SZ, WIN_SZ, 1, BlackPixel(d, s), WhitePixel(d, s));



     XSelectInput(d, w, ExposureMask | KeyPressMask);
  root = RootWindow(d, s);
  s = DefaultScreen(d);
  visinfo = (XVisualInfo *) malloc( sizeof(XVisualInfo) );
  if (!XMatchVisualInfo( d, s, 24, TrueColor, visinfo )) {
      printf("Couldn't get 24-bit TrueColor visual!\n");
      exit(1);
  }
  XMapWindow(d,w);

  cmap = XCreateColormap( d, root, visinfo->visual, AllocNone );;

  XFlush(d);



  xcolour.flags = DoRed | DoGreen | DoBlue;
  usleep(1000);




}

#define SQ_SZ (WIN_SZ/3)

int back_set = 0;


void x11show()
{
    if(!back_set){
        back_set=1;
        xcolour.red   = 0 ;
        xcolour.green = 0 ;
        xcolour.blue  = 0 ;
        XAllocColor(d, cmap, &xcolour);
        XSetBackground(d, DefaultGC(d, s), xcolour.pixel);
        XFillRectangle(d, w, DefaultGC(d, s), 0,   0 , WIN_SZ-1, WIN_SZ-1);
        XFlush(d);
    }

    // I guess XParseColor will work here
    xcolour.red   = (strip_rgb[0]<< 8) ;
    xcolour.green = (strip_rgb[1]<< 8) ;
    xcolour.blue  = (strip_rgb[2]<< 8) ;

    XAllocColor(d, cmap, &xcolour);
    XSetForeground(d, DefaultGC(d, s), xcolour.pixel);
    XFillRectangle(d, w, DefaultGC(d, s), SQ_SZ,   SQ_SZ , SQ_SZ, SQ_SZ);

    xcolour.red   = (strip_rgb[3]<< 8) ;
    xcolour.green = (strip_rgb[4]<< 8) ;
    xcolour.blue  = (strip_rgb[5]<< 8) ;

    XAllocColor(d, cmap, &xcolour);
    XSetForeground(d, DefaultGC(d, s), xcolour.pixel);
    XFillRectangle(d, w, DefaultGC(d, s), 0, SQ_SZ, SQ_SZ, SQ_SZ);

    xcolour.red   = (strip_rgb[6]<< 8) ;
    xcolour.green = (strip_rgb[7]<< 8) ;
    xcolour.blue  = (strip_rgb[8]<< 8) ;

    XAllocColor(d, cmap, &xcolour);
    XSetForeground(d, DefaultGC(d, s), xcolour.pixel);
    XFillRectangle(d, w, DefaultGC(d, s), SQ_SZ, 0, SQ_SZ, SQ_SZ);

    xcolour.red   = (strip_rgb[9]<< 8) ;
    xcolour.green = (strip_rgb[10]<< 8) ;
    xcolour.blue  = (strip_rgb[11]<< 8) ;

    XAllocColor(d, cmap, &xcolour);
    XSetForeground(d, DefaultGC(d, s), xcolour.pixel);
    XFillRectangle(d, w, DefaultGC(d, s), 2*SQ_SZ, SQ_SZ, SQ_SZ, SQ_SZ);

    xcolour.red   = (strip_rgb[12]<< 8) ;
    xcolour.green = (strip_rgb[13]<< 8) ;
    xcolour.blue  = (strip_rgb[14]<< 8) ;

    XAllocColor(d, cmap, &xcolour);
    XSetForeground(d, DefaultGC(d, s), xcolour.pixel);
    XFillRectangle(d, w, DefaultGC(d, s), SQ_SZ, 2*SQ_SZ, SQ_SZ, SQ_SZ);
}

/*---------------------------------------------------
!  X11
  ---------------------------------------------------*/

#endif


int strip_rgb_to_ccm(){
  int i;
  for(i=0;i<15;i++){
    strip_ccm[i] = strip_rgb[i] * PWM_STEP;
  }
}

void apply_rbg_to_pwm(){
  strip_rgb_to_ccm();
  //
  // TODO
  // update the CCM registers
  //
#ifdef X11_SHOW
  x11show();
#endif
}





void fade_to(uint8_t to[], uint16_t steps ,uint8_t init){
  uint8_t start;
  uint8_t target;
  int i;
  float f;
  if(init){
    for(i=0;i<15;i++){
      strip_delta[i] = ((float)(to[i] - strip_rgb[i])) / ((float)steps);
      strip_float[i] = strip_rgb[i];
    }    
  }else{
  for(i=0;i<15;i++){
    strip_float[i] += strip_delta[i];
    f =  strip_float[i] ;
    //printf("%f ",f);
    if(f < 0){
      strip_rgb[i] = 0;
    }else{
      strip_rgb[i] = floorf(f);
    }
  }
  apply_rbg_to_pwm();
  }
}

uint8_t wheel_r[360]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
uint8_t wheel_g[360]={0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t wheel_b[360]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4
};


int do_wheel(int i)
{
     /*
      strip_rgb[S1_R] = wheel_r[i%360];
      strip_rgb[S1_G] = wheel_g[i%360];
      strip_rgb[S1_B] = wheel_b[i%360];
      */
      strip_rgb[S2_R] = wheel_r[(i+90)%360];
      strip_rgb[S2_G] = wheel_g[(i+90)%360];
      strip_rgb[S2_B] = wheel_b[(i+90)%360];

      strip_rgb[S3_R] = wheel_r[(i+180)%360];
      strip_rgb[S3_G] = wheel_g[(i+180)%360];
      strip_rgb[S3_B] = wheel_b[(i+180)%360];

      strip_rgb[S4_R] = wheel_r[(i+270)%360];
      strip_rgb[S4_G] = wheel_g[(i+270)%360];
      strip_rgb[S4_B] = wheel_b[(i+270)%360];

      strip_rgb[S5_R] = wheel_r[(i+0)%360];
      strip_rgb[S5_G] = wheel_g[(i+0)%360];
      strip_rgb[S5_B] = wheel_b[(i+0)%360];
      apply_rbg_to_pwm();

}


void pulse_white(int steps)
{
    int i;


}


int main(){
  

  Visual *visual;
#ifdef X11_SHOW
  createwin();
#endif

  int i,j;
  int steps = 100;
  // init
  uint8_t strip_to[15];
  uint8_t strip_bckup[15];

  for(i=0;i<15;i++){
    strip_ccm[i] = 0;
    strip_rgb[i] = 0;
    strip_to[i] =  0;   
  }

  i=0;

  strip_to[S1_R] = 255;
  strip_to[S1_G] = 255;
  strip_to[S1_B] = 255;

  strip_to[S2_R] = wheel_r[(i+90)%360];
  strip_to[S2_G] = wheel_g[(i+90)%360];
  strip_to[S2_B] = wheel_b[(i+90)%360];

  strip_to[S3_R] = wheel_r[(i+180)%360];
  strip_to[S3_G] = wheel_g[(i+180)%360];
  strip_to[S3_B] = wheel_b[(i+180)%360];

  strip_to[S4_R] = wheel_r[(i+270)%360];
  strip_to[S4_G] = wheel_g[(i+270)%360];
  strip_to[S4_B] = wheel_b[(i+270)%360];

  strip_to[S5_R] = wheel_r[(i+0)%360];
  strip_to[S5_G] = wheel_g[(i+0)%360];
  strip_to[S5_B] = wheel_b[(i+0)%360];

  // FADE _ TEST
  putchar('\n');
  //

  fade_to(strip_to, steps ,1);
  for(i=0;i<steps;i++){
    fade_to(strip_to, steps ,0);

    usleep(10000);
  }

  for(i=0;i<360;i++){
      do_wheel(i);

      usleep(10000);
  }

  for(i=0;i<15;i++){
      strip_bckup[i] = strip_rgb[i];
      strip_to[i] = 128;
  }
  fade_to(strip_to, steps ,1);
  for(i=0;i<steps;i++){
      fade_to(strip_to, steps ,0);

      usleep(10000);
  }

  for(j=0;j<20;j++){

      steps = 10;
      for(i=0;i<15;i++){
          strip_to[i] = 255;
      }
      fade_to(strip_to, steps ,1);
      for(i=0;i<steps;i++){
          fade_to(strip_to, steps ,0);
          usleep(10000);
      }
      steps = 50;
      fade_to(strip_bckup, steps ,1);
      for(i=0;i<steps;i++){
          fade_to(strip_to, steps ,0);

          usleep(10000);
      }
  }

#ifdef X11_SHOW
  XCloseDisplay(d);
#endif

  return 0;
}
