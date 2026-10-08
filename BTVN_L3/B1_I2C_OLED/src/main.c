#include "stm32f10x.h"
#include "i2c.h"
//#include "ina219.h"
//#include "relay.h"
#include "OLED.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>


char rx_buffer[100];               
volatile uint8_t rx_index = 0;     
volatile uint8_t data_ready = 0;   

// Hàm ph?c v? ng?t nh?n UART1
void USART1_IRQHandler(void)
{	   
	
    if (USART1->SR & (1 << 5))
    {
	        char c = (char)(USART1->DR & 0xFF); 		
			
			GPIOC->ODR ^= (1<<13);		

        
        if (c == '!') // Ký t? k?t thúc
        {
            rx_buffer[rx_index] = '\0'; 
            data_ready = 1;             
            rx_index = 0;               
        }
        else
        {
            if (rx_index < 99) 
            {
                rx_buffer[rx_index++] = c;
            }
        }
    }
}

int main(void)
{
    // Kh?i t?o h? th?ng và UART1
    SystemInit();


    // 1. Kh?i t?o màn hình OLED. 
    // Tham s? '1' thu?ng d?i di?n cho b? I2C1 (Chân PB6, PB7)
    oled_init(1);

    // 2. Xóa s?ch rác (các di?m ?nh ng?u nhiên) trên màn hình khi m?i c?p ngu?n
    oled_blank(1);

    // 3. In ch? "Hello" lên màn hình
    // Cú pháp: oled_msg(b?_i2c, Ypos, Xpos, "Chu?i ký t?");
    // - Ypos = 0: Dòng trên cùng (Màn hình thu?ng có 8 dòng, dánh s? t? 0 d?n 7)
    // - Xpos = 0: C?t sát l? bên trái (Ðánh s? t? 0 d?n 127)
    oled_msg(1, 0, 0, "Heloo");
    
    // In thêm m?t dòng n?a ? v? trí khác cho sinh d?ng
    oled_msg(1, 2, 20, "STM32 OLED");

    while (1)
    {
        // Vòng l?p chính d? tr?ng, ch? dã du?c luu trên RAM c?a màn hình OLED
    }
}


