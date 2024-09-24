
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
} Context_t;

void led_driver(void *context);
void dio(void *context);
void ir_receiver(void *c);

RGB_t hi_cpicker(); //random color picker, high intensity only
RGB_t rnd_cpicker(); //totally random color picker
