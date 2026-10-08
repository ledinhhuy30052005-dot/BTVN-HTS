
#include "i2c.h"



void i2c_init(char i2c,unsigned short speed_mode)  // cau hinh cho i2c
{
	RCC->APB2ENR |= 1;      // bat AF
	
	if(i2c==1)
	{
		RCC->APB1ENR |= 0x200000;    // cap cl cho ic1
		// Enable clock GPIOB
    RCC->APB2ENR |= (1<<3);   // IOPBEN

// PB6 = SCL
    GPIOB->CRL &= ~(0xF << 24);   // clear config
    GPIOB->CRL |=  (0xF << 24);   // MODE=11 (50MHz) , CNF=11 (AF OpenDrain)

// PB7 = SDA
    GPIOB->CRL &= ~(0xF << 28);
    GPIOB->CRL |=  (0xF << 28);
		I2C1->CR1 |= 0x8000;            // bat reset de lam moi
		I2C1->CR1 &= ~0x8000;               // tat reset
	// Tìm trong i2c_init c?a i2c.c
I2C1->CR2 = 36;           // T?n s? bus APB1 là 36MHz (n?u chip ch?y 72MHz)
I2C1->CCR = 180;          // C?u hình cho t?c d? 100kHz
I2C1->TRISE = 37;
		I2C1->CR1 |= 1;               // KHOI DONG COHO i2c
	}
	else if(i2c==2)
	{
		RCC->APB1ENR |= 0x400000;
		// Enable clock GPIOB
    RCC->APB2ENR |= (1<<3);

// PB10 = SCL
     GPIOB->CRH &= ~(0xF << 8);
     GPIOB->CRH |=  (0xF << 8);

// PB11 = SDA
    GPIOB->CRH &= ~(0xF << 12);
    GPIOB->CRH |=  (0xF << 12);
		I2C2->CR1 |= 0x8000;
		I2C2->CR1 &= ~0x8000;
		I2C2->CR2 =0x8;
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		I2C2->CR1 |= 1;
	}

}

// Start step
void i2c_start(char i2c) 
{
    if(i2c == 1)
    {
        I2C1->CR1 |= 0x100;           // Set bit START trong thanh ghi CR1
        while (!(I2C1->SR1 & 1)){};   // Cho den khi bit SB=1 (Start Bit da duoc gui)
    }
    else if(i2c == 2)
    {
        I2C2->CR1 |= 0x100;
        while (!(I2C2->SR1 & 1)){};
    }
}
// Sending the address + R or Write	
void i2c_add(char i2c, char address, char RW) 
{
    volatile int tmp;
    if(i2c == 1)
    {
        I2C1->DR = (address | RW);    // Nap dia chi vao thanh ghi du lieu DR
        while((I2C1->SR1 & 2) == 0){}; // Cho bit ADDR=1 (Dia chi da duoc thiet bi nhan va ACK)
        
        // Buoc xoa co ADDR (doc SR1 roi doc SR2 theo dung quy trinh datasheet)
        while((I2C1->SR1 & 2)){      
            tmp = I2C1->SR1;             
            tmp = I2C1->SR2;
            if((I2C1->SR1 & 2) == 0) break;
        }
    }
    else if(i2c == 2)
    {
        I2C2->DR = (address | RW);
        while((I2C2->SR1 & 2) == 0){};
        while((I2C2->SR1 & 2)){
            tmp = I2C2->SR1;
            tmp = I2C2->SR2;
            if((I2C2->SR1 & 2) == 0) break;
        }
    }
}

// Sending data step
void i2c_data(char i2c,char data)  // gui data
{
	if(i2c==1)
	{
		while((I2C1->SR1 & 0x80) == 0){}  //Ðoi DR trong  moi gui du lieu
			I2C1->DR = data;
		while((I2C1->SR1 & 0x80) == 0){} // Ðoi DR trong lai
	}
	else if(i2c==2)
	{
		while((I2C2->SR1 & 0x80) == 0){}
			I2C2->DR = data;
		while((I2C2->SR1 & 0x80) == 0){}
	}
}
// Stop step
void i2c_stop(char i2c)  // sau khi gui du lieu xong thi dung chuyen data
{
	volatile int tmp;
	if(i2c==1)
	{
		tmp = I2C1->SR1;  // clear bi5t ADDR
		tmp = I2C1->SR2;
		I2C1->CR1 |= 0x200;  // bat bit stop 
	}
	else if(i2c==2)
	{
		tmp = I2C2->SR1;
		tmp = I2C2->SR2;
		I2C2->CR1 |= 0x200;
	}
}
// i2c_write()
void i2c_write(char i2c, char address,char data[])// tong cua cac ham tren
{
	int i = 0;  
	
	i2c_start(i2c);    // bat dau chuyen doi
	
	i2c_add(i2c, address,0);  // gui dia chi va wri5te
	
	while(data[i]!='\0')  // gui he5t da5ta
		{
			i2c_data(i2c,data[i]);
			i++;
		}
	i2c_stop(i2c); //stop bit
}

