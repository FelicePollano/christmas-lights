
typedef struct Mode{
    int(*init)(uint8_t*,int); //init function called when mode enter
    int(*run)(uint8_t* ,int); //run function
    void(*clean)(uint8_t* ,int); //cleanup function
    void(*tweak)(char* ,char*); //tweak parameter
}Mode_t;

typedef struct Context{
    int led_GPIO;
    uint8_t *led_data;
    int led_count;
    Mode_t *mode;
    Mode_t *modes;
    int modes_count;
} Context_t;

void led_driver(void *context);
void dio(void *context);

