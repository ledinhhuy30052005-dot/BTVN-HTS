#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

/* Cau truc chua chan LED va thoi gian doi trang thai */
typedef struct
{
    uint16_t pin;
    uint32_t time;
} LED_t;

/* Cau hinh 3 LED */
LED_t LED1 = {GPIO_Pin_0, 5000};  // PA0: 0.1 Hz  LED1.pin = GPIO_Pin_0 LED1.time = 5000
LED_t LED2 = {GPIO_Pin_1, 500};   // PA1: 1 Hz    LED2.pin = GPIO_Pin_1 LED2.time = 5000
LED_t LED3 = {GPIO_Pin_2, 50};    // PA2: 10 Hz   LED3.pin = GPIO_Pin_2 LED3.time = 5000

/* Cau hinh GPIO */
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* Ham dieu khien LED dung chung */
void LED_Task(void *parameter)//void *parameter la con tro tong quan chua co 1 dinh dang cu the 
{
    LED_t *led;//con tro led dung de luu dia chi cua mot bien kieu struct LED_t
    led = (LED_t *)parameter;//ép kieu parameter thành con tro den LED_t.
    // led = &LED1/&LED2/&LED3
    while (1)
    {
        GPIO_SetBits(GPIOA, led->pin);// led -> pin = (*led).pin 
        vTaskDelay(led->time);

        GPIO_ResetBits(GPIOA, led->pin);
        vTaskDelay(led->time);
    }
}

int main(void)
{
    GPIO_Config();

    /* Tao task nhay LED PA0 */
    xTaskCreate(LED_Task, "LED1", 128, &LED1, 1, NULL);

    /* Tao task nhay LED PA1 */
    xTaskCreate(LED_Task, "LED2", 128, &LED2, 1, NULL);

    /* Tao task nhay LED PA2 */
    xTaskCreate(LED_Task, "LED3", 128, &LED3, 1, NULL);

    /* Bat dau chay FreeRTOS */
    vTaskStartScheduler();

    while (1)
    {
    }
}