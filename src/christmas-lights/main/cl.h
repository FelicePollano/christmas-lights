
typedef struct RGB{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_t;

typedef struct Mode{
    int(*init)(uint8_t*,int,RGB_t (*cpicker)()); //init function called when mode enter
    int(*run)(uint8_t* ,int,RGB_t (*cpicker)()); //run function
    void(*clean)(uint8_t* ,int); //cleanup function
}Mode_t;

typedef struct Context{
    int led_GPIO;
    uint8_t *led_data;
    int led_count;
    Mode_t *mode;
    Mode_t *modes;
    int modes_count;
    RGB_t (*cpicker)(); //color picker 
    RGB_t (**cpickers)(); //color pickers 
    int cpickers_count;
} Context_t;

void led_driver(void *context);
void dio(void *context);
void ir_receiver(void *c);

RGB_t hi_cpicker(); //random color picker, high intensity only
RGB_t rnd_cpicker(); //totally random color picker
RGB_t c1_cpicker(); // fixed color pickers
RGB_t c2_cpicker();
RGB_t c3_cpicker();
RGB_t c4_cpicker();
RGB_t c5_cpicker();
RGB_t c6_cpicker();
RGB_t c7_cpicker();
RGB_t c8_cpicker();
RGB_t c9_cpicker();
RGB_t c10_cpicker();
RGB_t c11_cpicker();
RGB_t c12_cpicker();
RGB_t c13_cpicker();
RGB_t c14_cpicker();
RGB_t c15_cpicker();