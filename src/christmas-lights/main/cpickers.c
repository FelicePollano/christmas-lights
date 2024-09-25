#include <stdio.h>
#include "cl.h"
#include "esp_random.h"

RGB_t hi_cpicker(){ //random color picker, high intensity only
     uint32_t r=esp_random();
     uint32_t r2=esp_random();
     RGB_t color;
     color.G= r%2==0?(r2%2==0?255:128):0;//G
     r=esp_random();
     r2=esp_random();
     color.R = r%2==0?(r2%2==0?255:128):0;//R
     r=esp_random();
     r2=esp_random();
     color.B = r%2==0?(r2%2==0?255:128):0; //B
     return color;
}

RGB_t rnd_cpicker(){ //totally random color picker
   
    RGB_t color;
    color.G= (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    color.R = (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    color.B = (uint8_t)(esp_random()/(float)UINT32_MAX*255);
    return color;
}

RGB_t by_vals(uint8_t R,uint8_t G,uint8_t B){
    RGB_t c={.R=R,.G=G,.B=B};
    return c;
}
RGB_t c1_cpicker() { return by_vals(255,0,0); }
RGB_t c2_cpicker() { return by_vals(0,255,0); }
RGB_t c3_cpicker() { return by_vals(0,0,255); }
RGB_t c4_cpicker() { return by_vals(0,255,255); }
RGB_t c5_cpicker() { return by_vals(255,0,255); }
RGB_t c6_cpicker() { return by_vals(255,255,0); }
RGB_t c7_cpicker() { return by_vals(255,255,255); }
RGB_t c8_cpicker() { return by_vals(128,255,255); }
RGB_t c9_cpicker() { return by_vals(255,128,255); }
RGB_t c10_cpicker() { return by_vals(255,255,128); }
RGB_t c11_cpicker() { return by_vals(255,128,0); }
RGB_t c12_cpicker() { return by_vals(128,0,0); }
RGB_t c13_cpicker() { return by_vals(128,255,0); }
RGB_t c14_cpicker() { return by_vals(128,255,128); }
RGB_t c15_cpicker() { return by_vals(128,128,255); }