#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void GPIOC_configure(){
  RCC->APB2ENR |= 0x10;//clock PC
	GPIOC->CRH &= ~(0xF << 20);
	GPIOC->CRH |= 0x3<<20; 
}

void Button_configure(){
    RCC->APB2ENR |= (1<<2); // GPIOA
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0x8 << 0);   // input pull-up/down
    GPIOA->ODR |= 0x1<<0;

}
int main(){
	GPIOC_configure();
	Button_configure();
  int prev_state=1;// trang thai nut nhan truoc do
 int led_state =0;	// cap nhat trang thai led 0:tat 1:sang
   while(1){
    int button_state = (GPIOA->IDR >> 0) & 1;// trang thai nut nhan PB3 hien tai
		 if(prev_state==1 && button_state==0 ){  // led chi thay doi khi trang thai nut nhan trc do  la 1 va trang thai nut nhan hien tai la 0;note:moi trang thai nhu: trc do la 0, hien tai la 0 thi led ko thay doi;trc do la 0 va hien tai la 1 cx ko thay doi led
         led_state = !led_state ; // dao trang thai led bat->tat,tat->bat
			   if(led_state!=0){  // led dang sang
					 GPIOC->ODR &=~(0x1<<13);//bat led PC3
				 }
					else{
            GPIOC->ODR |=0x1<<13;// tat led PC3 						
				 
				 }
		 } 
		 prev_state=button_state;// cap nhat trang thai trc do cua nut nhan roi tiep tuc vong lap tiep theo
	
	
	}
}
