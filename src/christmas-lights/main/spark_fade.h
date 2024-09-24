int spark_fade_init(uint8_t *data,int count,RGB_t (*cpicker)()); //init function
int spark_fade_init_inv(uint8_t *data,int count,RGB_t (*cpicker)()); //alt init function
int spark_fade_run(uint8_t *data,int count,RGB_t (*cpicker)()); //run function
int spark_fade_run_inv(uint8_t *data,int count,RGB_t (*cpicker)()); // alt run function
void spark_fade_clean(uint8_t *data ,int count); //cleanup function
