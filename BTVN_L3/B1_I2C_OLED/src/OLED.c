#include "OLED.h"
#include "i2c.h"
#include "stm32f10x.h"                  // Device header

#define i2c_FM 0x2d// toc do
#define i2c_SM 0xB4// toc do

void oled_cmd_1byte(char i2c,char data){ // ham oled gui lenh 1byte de dieu khien oled
	i2c_start(i2c); // bat dau chuyen doi
	i2c_add(i2c,0x78, 0); //gui dia chi va che do gui lenh (lenh la dieu khien oled)
	i2c_data(i2c,0x00); // che do  gui lenh
	i2c_data(i2c,data); // gui du lieu
	i2c_stop(i2c);
	
	
}
void oled_cmd_2byte(char i2c,char data[]){ // ham oled gui lenh 2byte de dieu khien oled
	int i=0;
	i2c_start(i2c); // bat dau chuyen doi
	i2c_add(i2c,0x78, 0); //gui dia chi va che do gui lenh (lenh la dieu khien oled)
	i2c_data(i2c,0x00); // che do  gui lenh
		
	for(i=0;i<2;i++){
	i2c_data(i2c,data[i]);} // gui du lieu
	i2c_stop(i2c);
	
	
}
//oled init
char cmd[] = {0xA8,0x3F};
char cmd1[] = {0xD3,0x00};
char cmd2[] = {0xDA,0x12};             //size screen 64
//char cmd2[] = {0xDA,0x22};              //size 32
char cmd3[] = {0x81,0x7F};
char cmd4[] = {0xD5,0x80};
char cmd5[] = {0x8D,0x14};
char cmd6[] = {0x20,0x10};

void oled_init(char i2c){
	i2c_init(i2c,i2c_FM);
	oled_cmd_2byte(i2c,cmd);
	oled_cmd_2byte(i2c,cmd1);
	
	oled_cmd_1byte(i2c,0x40);
	oled_cmd_1byte(i2c,0xA1);
	
	oled_cmd_1byte(i2c,0xC8);
	
	oled_cmd_2byte(i2c,cmd2);
	
	oled_cmd_2byte(i2c,cmd3);
	
	oled_cmd_1byte(i2c,0xA4);
	oled_cmd_1byte(i2c,0xA6);
	
	oled_cmd_2byte(i2c,cmd4);
	
	oled_cmd_2byte(i2c,cmd5);
	
	oled_cmd_1byte(i2c,0xAF);
	
	oled_cmd_2byte(i2c,cmd6);
	
}
// oled data
void oled_data(char i2c,char data){ // ham oled gui data  de hien thi tren  oled
	i2c_start(i2c); // bat dau chuyen doi
	i2c_add(i2c,0x78, 0); //gui dia chi va che do gui lenh (lenh la dieu khien oled)
	i2c_data(i2c,0x40); // che do  gui data
	i2c_data(i2c,data); // gui du lieu
	i2c_stop(i2c);
	
	
}	

// vi tri tren oled 

void oled_pos(char i2c,char Ypos, char Xpos) // Y la page (0-7) moi page co 8 hang va 128 cot  X la cot
{
	oled_cmd_1byte(i2c,0x00 + (0x0F & Xpos));  // lay bon bit thap 
	oled_cmd_1byte(i2c,0x10 + (0x0F & (Xpos>>4))); // lay bon bit  cao  // day la ham co dinh roi//
	oled_cmd_1byte(i2c,0xB0 + Ypos);  // page y  
}

	// clear man hinh oled truoc
void oled_blank(char i2c)
{
	int i,j;
	for(i=0;i<8;i++) // 8 page
	{
		oled_pos(i2c, i, 0); // ? THÊM DÒNG NÀY: chuy?n t?i d?u page i tru?c khi ghi
		for(j=0;j<128;j++)  // moi dong cua page se có 128 cot 
		{
			oled_data(i2c,0x00); // xoa tung cot 1 cua page 
		}
	}
	oled_pos(i2c,0, 0);
}


//oled_print
void oled_print(char i2c,char str[]) //str la chuoi hien thi
{
	int i,j;
	i=0;
	while(str[i])// khi nao chuoi den \0 thi xong
	{
		for(j=0;j<5;j++)
		{
			oled_data(i2c,ASCII[(str[i]-32)][j]);// moi ki tu se làm j do trong aski
			
		}
		i++;
	}
}
//oled_msg
void oled_msg(char i2c,char Ypos, char Xpos,char str[])  // day la ham tong in chu chon vi tri bat ki
{
	oled_pos(i2c,Ypos,Xpos); // vi tri
	oled_print(i2c,str);  // in
}







