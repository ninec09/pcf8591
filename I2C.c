#include <stm32f4xx.h>
#include "I2C.h"
#include "math.h"

/****************functionCrash Degiskenleri***************************

		 Slave cihazdan geri dönüs olmadigi için sonsuz while 
		kontrolünde belirli bir süre takilirsa functionCrash 
		flagleri kalkar ve I2C haberlesmesine reset atilir.
		
**********************************************************************/
unsigned int I2C_Start_functionCrashCntr=0;
unsigned int I2C_Stop_functionCrashCntr=0;
unsigned int I2C_Write_functionCrashCntr=0;
unsigned int I2C_Read_functionCrashCntr=0;
unsigned int I2C_Address_functionCrashCntr=0;



/****************functionStop Degiskenleri***************************

	  FunctionCrash flargleri kalkmasi sebebi ile
		belirli bir sayi reset almis ve hala problem devam 
		ediyor ise araca stop verdirilir.
		
**********************************************************************/
unsigned char I2C_Start_functionStop=0;
unsigned char I2C_Stop_functionStop=0;
unsigned char I2C_Write_functionStop=0;
unsigned char I2C_Read_functionStop=0;
unsigned char I2C_Address_functionStop=0;



/****************CheckFlag Degiskenleri******************************

	    Sonsuz while'in icine girdini ve systick counter'larin
		 saymaya baslamasi gerektigini belirtir.
		 
		  Logic 0'a geri dönmesi sonsuz while'den ciktigini belirtir.
		
**********************************************************************/
unsigned char I2C_Start_CheckFlag=0;
unsigned char I2C_Stop_CheckFlag=0;
unsigned char I2C_Write_CheckFlag=0;
unsigned char I2C_Read_CheckFlag=0;
unsigned char I2C_Address_CheckFlag=0;


/****************SystickCounter Degiskenleri**************************

	     CheckFlag kaltiginda sonsuz while'in icinde oldugunu
			anlar ve systick kesmesinde saymaya baslar
		
**********************************************************************/
unsigned int I2C_Start_SystickCounter=0;
unsigned int I2C_Stop_SystickCounter=0;
unsigned int I2C_Write_SystickCounter=0;
unsigned int I2C_Read_SystickCounter=0;
unsigned int I2C_Address_SystickCounter=0;


unsigned char I2C_Crash_Stop;		// I2C haberlesmesi belirli bir sayida arka arkaya crash yerse araci durdurur




/************************************init_i2c_user(void)*************************************************
*		I2C için gerekli GPIO ayarlari yapilir																															*
*		I2C iletisiminin baslamas1 için gerekli i2c registerlari configure edirli														*
*																																																				*
********************************************************************************************************/
void init_i2c_user(void){
		RCC->AHB1ENR |= 1<<1;              																							 //GPIOB clock açma
		RCC->APB1ENR |= 1 <<22;           																							 //I2C2  clock açma
		GPIOB->AFR[1] |= (4<< 8);        		  																					 //SCl -> PB10 
		GPIOB->AFR[1] |= (4<< 12);         																							 //SDA -> PB11  
		GPIOB->MODER &= ~(3 << 20);           																					 //20, 21 (PB10) temizle
		GPIOB->MODER |= 2 << 20;           																						 	 //MODER6[1:0] = 10 bin
		GPIOB->MODER &= ~(3 << 22);        																							 //22, 23 (PB11) temizle
		GPIOB->MODER |= 2 << 22;           																							 //MODER9[1:0] = 10 bin
		GPIOB->OTYPER |= 1 <<10;           																							 //PB10 open drain
		GPIOB->OTYPER |= 1 <<11;           																							 //PB11 open drain
		GPIOB->PUPDR &= ~(3 << 20);        																							 //20, 21 (PB10) temizle
		GPIOB->PUPDR &= ~(3 << 22);        																							 //22, 23 (PB11) temizle
		
		//I2C ayarlar1
		I2C2->CR2     = 0x0010;    					 																						 //16MHz HSI
		I2C2->CCR     = 0x0050;   				   	 																					 //100kHz Bit rate 
		I2C2->TRISE   = 0x0011;  					   	 																					 //1000 ns / 62.5 ns = 16 + 1
		I2C2->CR1    = 0x0001;  				     	 																					 //enable peripheral
}



/***********************I2C_Start()************************************
*	I2C iletisimine baslanmadan cagirilmasi gerekir											*	
*																																			*
*		- SDA logic 0'a çekilir(Start biti)         											*
*   - SCL'den clock sinyali gönderilmeye baslanir											*
**********************************************************************/
void I2C_Start(void) {
		char dontReset = 0;
  	I2C2->CR1 |= 1<<8;               		 																						//I2C basla komutu
	  
  	I2C_Start_CheckFlag      = 1;
	  while (!(I2C2->SR1 & 0x0001)){																									//start biti bekle	
			if(I2C_Start_SystickCounter == FUNCTION_CRASH_COUNT_1MS){
				I2C_Start_functionCrashCntr++;
				dontReset = 1;
				break;
			}
		}
		if(dontReset == 0)
			I2C_Start_functionCrashCntr = 0;
		
		I2C_Start_CheckFlag         = 0;
		I2C_Start_SystickCounter    = 0;
}



/***********************I2C_Stop(void)*********************************
*	I2C iletisimi bitiminde cagirilmasi gerekir													*	
*																																			*
*		- SDA logic 1'a çekilir(Stop biti)          											*
*   - SCL'deki clock sinyali durur																		*
**********************************************************************/
void I2C_Stop(void){
	 char dontReset = 0;
   I2C2->CR1 |= 0x0200;
   
	 I2C_Stop_CheckFlag = 1;
	 while (I2C2->SR2 & 0x0002){ 		       																						//wait for BUSY set  
			if(I2C_Stop_SystickCounter == FUNCTION_CRASH_COUNT_1MS){
				I2C_Stop_functionCrashCntr++;
				dontReset = 1;
				break;
			}		 
	 }
	 if(dontReset == 0)
		 I2C_Stop_functionCrashCntr = 0;
	 
	 I2C_Stop_CheckFlag         = 0;
	 I2C_Stop_SystickCounter    = 0;
}



/***********************void I2C_Write(unsigned char)*****************
*	I2C iletisimi baslatildiktan sonra bir byte bilgi gönderir					*	
*																																			*
*																																			*
*	Input:																															*
*			-c: gönderilicek bilgi																					*
**********************************************************************/
void I2C_Write(unsigned char c){
	char dontReset = 0;
	I2C2->DR = c;
  
	I2C_Write_CheckFlag = 1;
	while (!(I2C2->SR1 & (1<<7))){         																						// Wait TxE bit set - <A HREF="#linkSR">see status register</A>
			if(I2C_Write_SystickCounter == FUNCTION_CRASH_COUNT_1MS){
				I2C_Write_functionCrashCntr++;
				dontReset = 1;
				break;
			}		
	}
	if(dontReset == 0)
		I2C_Write_functionCrashCntr = 0;
	
	I2C_Write_CheckFlag         = 0;
  I2C_Write_SystickCounter    = 0;
}



/***********************unsigned char I2C_Read(int)*******************
*	I2C iletisiminde okuma yapar																				*	
*																																			*
*																																			*
*	Input:																															*
*			-ack: 1 -> daha fazla byte gelicek															*
*			      0 -> tek yada son byte																		*
**********************************************************************/
unsigned char I2C_Read(int ack) {
	char dontReset = 0;
	if (ack)
			I2C2->CR1 |=  0x0400;				      																					 //multiple bytes - set acknowledge bit in<A HREF="#linkcontrol"> I2C_CR1</A>
  else
  		I2C2->CR1 &= ~0x0400;	            																					 //single or last byte clear ack bit
	
	I2C_Read_CheckFlag = 1;	
	while (!(I2C2->SR1 & 0x00000040)){     																					 // Wait until RxNE bit set - <A HREF="#linkSR">see status register</A>
			if(I2C_Read_SystickCounter == FUNCTION_CRASH_COUNT_1MS){
				I2C_Read_functionCrashCntr++;
				dontReset = 1;
				break;
			}		
	}
	if(dontReset == 0)
		I2C_Read_functionCrashCntr = 0;
	
	I2C_Read_CheckFlag         = 0;
  I2C_Read_SystickCounter    = 0;
	
	return (I2C2->DR);
}


/***********************I2C_Address(unsigned char)********************
*	I2C iletisiminde write islemi yapilicak cihazin adresi	            *	
*	 gönderilir																													*	
*																																			*
*	Input:																															*
*			-adr: bilginin gönderilicegi slave cihazin adresi								*
**********************************************************************/
void I2C_Address(unsigned char adr){
	char dontReset = 0;
  char res;
	I2C2->DR = adr | 0;    			           																				   //Write to I2C Address register
	
	I2C_Address_CheckFlag = 1;
	while (!(I2C2->SR1 & 0x0002)){	       																					 //wait until address sent  
			if(I2C_Address_SystickCounter == FUNCTION_CRASH_COUNT_1MS){
				I2C_Address_functionCrashCntr++;
				dontReset = 1;
				break;
			}		
	}
	if(dontReset == 0)
		I2C_Address_functionCrashCntr = 0;
	
	I2C_Address_CheckFlag         = 0;
	I2C_Address_SystickCounter    = 0;
	
	res = (I2C2->SR2);                     																					 //dummy read to clear 
}



/******************************i2c_digitalToAnalog(unsigned char)*******************************
*	I2C fonksiyonlarini kullanarak  DAC'a istenilen analog degeri digital olarak iletir					 *
*																																															 *
*	Input:																																											 *
*			-data: 1. DAC'a gönderilicek digital voltaj bilgisi(digital olarak)										   *
*																																															 *
*	Return:																																											 *
*			- 1: Gönderilen data lastDatadan +-10 fakli oldugu icin güncellendi                      *
* 		- 0: Gönderilen data lastDatadan +-10 fakli olmadigi icin güncellenmedi  					       *
************************************************************************************************/
/*int i2c_digitalToAnalog(unsigned char data){
	static unsigned char lastData;
	static unsigned char cnt;
	cnt++;
	
	if(fabs(lastData - data) > 0 ){
		if(( lastData < data ) && ( cnt > 4) ){
			if ( fabs(lastData-data) > 10 ){
				lastData = lastData+4;
				cnt=0;
			}
			else
				lastData++;
				cnt=0;
		}
		if(( lastData > data ) && ( cnt > 4 )){
			if ( fabs(lastData-data) > 10 ){
				lastData = lastData-4;
				cnt=0;
			}
			else
				lastData--;
				cnt=0;
		}
		I2C_Start();
		I2C_Address(DAC_PCF8591_2);          																					//Write address of slave
		I2C_Write(PCF8591_DAC_COMMEND);
		I2C_Write(lastData);                     																					//set up register of interest (MAP))
		//lastData = data;   
		I2C_Stop();
		
		return 1;
	}
	else
		return 0;
	
}
*/


/************i2c_digitalToAnalog_withAdress(unsigned char, unsigned char)***********************
*	I2C fonksiyonlarini kullanarak  ISTENILEN CIHAZ ADRESINE																		 *
*		istenilen degeri digital olarak iletir																										 *
*																																															 *
*																																															 *
*	Input:																																											 *
*			-data: 1. DAC'a gönderilicek digital voltaj bilgisi(digital olarak)										   *
*																																															 *
*	Return:																																											 *
*			- 1: Gönderilen data lastDatadan +-10 fakli oldugu icin güncellendi                      *
* 		- 0: Gönderilen data lastDatadan +-10 fakli olmadigi icin güncellenmedi  					       *
************************************************************************************************/
int i2c_digitalToAnalog_withAdress(unsigned char data, unsigned char adress){

	
		I2C_Start();
		I2C_Address(adress); 				       																						  
		I2C_Write(PCF8591_DAC_COMMEND);																									//PCF8591'in D to A islemi yapicagini söyleyen komut
		I2C_Write(data);                   																						  //Analoga cevirilicek digital deger
		I2C_Stop();
		
	
}
