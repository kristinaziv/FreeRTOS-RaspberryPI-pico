#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 10
#define LCD_D5 11
#define LCD_D6 12
#define LCD_D7 13

void lcd_write_nibble(unsigned char nibble);
void lcd_print(const char* str);
void lcd_clear();
void lcd_set_cursor(unsigned char col, unsigned char row);
void lcd_pulse_enable();
void lcd_write_command(unsigned char cmd);
void lcd_write_data(unsigned char  data);
void lcd_write_nibble(unsigned char nibble);



typedef struct {
    char text[16];
    uint8_t row;
    uint8_t col;
} LCDTaskParams;

void lcd_clear() {
    lcd_write_command(0x01); 
    vTaskDelay(pdMS_TO_TICKS(2)); 
} 
void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    lcd_write_command(0x80 + col);
    printf("test");
}


void lcd_print(const char* str) {
    while (*str) {
        lcd_write_data(*str++);
    }
}


void lcd_pulse_enable() {
    gpio_put(LCD_EN, 1);
    vTaskDelay(pdMS_TO_TICKS(600));
    gpio_put(LCD_EN, 0);
    vTaskDelay(pdMS_TO_TICKS(600));
}



void lcd_send(unsigned char value, uint8_t mode) {
    gpio_put(LCD_RS, mode);
    vTaskDelay(pdMS_TO_TICKS(100));
    lcd_write_nibble(value >> 4);
    lcd_write_nibble(value);
}

void lcd_write_command(unsigned char cmd) {
    lcd_send(cmd, 0);
}

void lcd_write_data(unsigned char data) {
    lcd_send(data, 1);
}

void lcd_write_nibble(unsigned char nibble) {
    gpio_put(LCD_D4, (nibble >> 0) & 0x01);
    lcd_pulse_enable();
    gpio_put(LCD_D5, (nibble >> 1) & 0x01);
    lcd_pulse_enable();

    gpio_put(LCD_D6, (nibble >> 2) & 0x01);
    lcd_pulse_enable();

    gpio_put(LCD_D7, (nibble >> 3) & 0x01);

    lcd_pulse_enable();
}



void lcd_init() {

    gpio_init(LCD_RS);
    gpio_init(LCD_EN);
    gpio_init(LCD_D4);
    gpio_init(LCD_D5);
    gpio_init(LCD_D6);
    gpio_init(LCD_D7);

    gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_set_dir(LCD_EN, GPIO_OUT);
    gpio_set_dir(LCD_D4, GPIO_OUT);
    gpio_set_dir(LCD_D5, GPIO_OUT);
    gpio_set_dir(LCD_D6, GPIO_OUT);
    gpio_set_dir(LCD_D7, GPIO_OUT);


    lcd_write_nibble(0x03);
    lcd_write_nibble(0x03);
    lcd_write_nibble(0x03);
    lcd_write_nibble(0x02);

   

    lcd_write_command(0x06); 
    lcd_write_command(0x28); 
    lcd_write_command(0x0C); 



    lcd_clear();
}

