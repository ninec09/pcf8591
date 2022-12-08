#include "stm32f4xx.h"
#include "I2C.h"
#include "SysTickCounter.h"

int sonuc;
int data;
int data2;
int steeringVoltageDecOne;
int steeringVoltageDecTwo;
int angle=0;
int steeringOffset;

void clockConfig(void);
void clockConfig(void){
	unsigned int i;
  for (i=0;i<0x00100000;i++);     // OSC oturtma ve kurtarma rutini
  RCC->CFGR |= 0x00009400;        // AHB ve APB hizlarini max degerlere set edelim
  RCC->CR |= 0x00010000;          // HSE Xtal osc calismaya baslasin 
  while (!(RCC->CR & 0x00020000));// Xtal osc stabil hale gelsin
  //RCC->PLLCFGR = 0x07402A04;      // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim   168 Mhz 
  RCC->CR |= 0x01000000;          // PLL calismaya baslasin  (Rehber Sayfa 95)
  while(!(RCC->CR & 0x02000000)); // Pll hazir oluncaya kadar bekle
  FLASH->ACR = 0x00000605;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
  RCC->CFGR |= 0x00000002;        // Sistem Clk u PLL uzerinden besleyelim
  while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
	RCC->AHB1ENR |= 0x0000003F; 		// A,B,C,D,E ve F Portlari için Clock Sinyali Aktif
}


void Delay(int DelayTime);
void Delay(int DelayTime){
	int currentTime;
	currentTime = 0;
	msTick = 0;
	DelayTime = DelayTime *2;
	
	while ((msTick - currentTime) < DelayTime);
}

int test=0;

int main(){
	
	clockConfig();
	SysTickBaslat();
	GPIOD->MODER		|=	(1UL << 2*14);			
	Delay(4000);
	init_i2c_user();
	Delay(200);
	
	I2C_Start();																																								
	I2C_Address(0x98); 
	I2C_Write(0x40);                   			
	I2C_Write(0x00);	
	I2C_Stop();	
	
	I2C_Start();																																								
	I2C_Address(0x9C); 
	I2C_Write(0x40);                   			
	I2C_Write(0x00);	
	I2C_Stop();	
	
	
	while(1){
		if (MainLoopFlag == 1) {
		
		GPIOD->ODR |= (1UL << 14);

//		angle=90;
//		steeringOffset=25;
//		steeringVoltageDecOne = (char)((angle * 80) / 180) + steeringOffset;		 // hesaplanmis olan direksiyon aci degerinin arac icin decimal voltaj degere dönüstürülmesi
//		steeringVoltageDecTwo = 129 - steeringVoltageDecOne;   	
			
		I2C_Start();																																								
		I2C_Address(0x98); 
		I2C_Write(0x40);                   			
		I2C_Write(data2);	
		I2C_Stop();	
	
		I2C_Start();																																								
		I2C_Address(0x9C); 
		I2C_Write(0x40);                   			
		I2C_Write(data);	
		I2C_Stop();	
				
		MainLoopFlag = 0;
		}

}

}
	
	
		