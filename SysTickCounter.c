#include <stm32f4xx.h>
#include "SysTickCounter.h"
#include "I2C.h"



int msTick=0;																				// Delay Fonksiyonu Icin Kullaniliyor.

unsigned int  MainSceneDataCNTR = 0;								// Ana Sahnenin Ihtiyac Duydugu Datalarin Periyodik Gönderilmesi Icin Sayici

unsigned char LiftCounter = 0;
/**********************************SYSTICK COUNTER DEGISKENLERI*****************************************/
unsigned int GeneralCounter=0;											// 5ms'lik main  döngüsü olustrulmasi için sayici
unsigned int MainLoopFlag	=0;												// 5ms'lik main  döngüsüne girebilmek icin olusan flag
unsigned int MainLoopController = 0;
unsigned int MainLoopProtection = 0;
unsigned char InterrruptEnableFlag = 0;

unsigned char ConveyorInfo=0;

unsigned char YirmiBesMS   = 0;
unsigned char YuzMS        = 0;
unsigned char IkiYuzMS     = 0;
unsigned char IkiYuzElliMS = 0;
unsigned char UcYuzMS= 0;
unsigned char UcYuzElliMS = 0;
unsigned char DortYuzMS = 0;
unsigned char DortYuzElliMS = 0;
unsigned char BesYuzMS = 0;
unsigned char YediYuzElliMS = 0;
unsigned char BinMS = 0;

unsigned int  ReflectorFounder=0;										// Reflector Kaybi Sonrasi Cizgi Arama Icin 3sn Izin 
unsigned char ReflectorNotDetected=0;								// 3 sn boyunca Reflector Bulunamazsa Bu Flag SET olur 

int LineFounderCNTR = 0;
unsigned int	 		Nav350RequestDelayCNTR 					= 0;		// Systic counter'da 400' e kadar sayip 400x0.5ms geçtigi kontrol edilmektedir.

unsigned char MainSceneDataReq  = 0;								// Ana Sahnenin Ihtiyac Duydugu Datalarin Periyodik Gönderilmesi Icin Flag

int OyaLiftLoopCounter = 0;



/*********SYSTICK COUNTER(24 BIT SAYICI) SAYICISINI*****************
							   VE KESMESINI INITIALIZE ET
									
									--> [1/(168mhz)]*(84001-1)=0.5 ms  
								  --> Her 0.5 ms'de bir counter kesme olusturacak
						
*******************************************************************/
void SysTickBaslat(void){
	SysTick->CTRL = 0;									 // Ayarlamalar icin sysTick sayiciyi kapat	
	SysTick->LOAD = 0x00014821;					 // Saymaya baslayacagi sayiyi ayarla 0.5ms (Ayarlanan Deger=84001-1)
	SysTick->VAL  = 0;									 // En son degeri sifirla
	SysTick->CTRL|= 0x00000007;					 // Sayiciyi ana clk ve kesmeyle birlikte aktif et
}


/********************************************************************************************************
																	SYSTICK COUNTER KESME RUTINI
																	
												Her 0.5 ms'de bir Kesme Olusturmasi Icin Ayarlandi

									--> Kesme Icindeki Sayicilar ile Periyodik Döngüler Olusacaktir
********************************************************************************************************/
void SysTick_Handler(void){
	
	msTick++;																					// Delay Sayicisini Arttir(Her Tick 0.5 ms)
	GeneralCounter++;
	if(GeneralCounter == 10){										      // Her 10 kesme 5ms demek	--> 10 x 0.5 = 5ms
		MainLoopFlag=1;																	// Her 5ms'de bir Ana Döngüye Giris Bayragini Set Ediyoruz
		GeneralCounter=0;	
	
	}

	// LAZER NAVIGASYON START SONRASI DETECTION STATION VE STOP FONKSIYONLARI DEVREYE ALINMASI ICIN GECMESI GEREKEN SURE..
	
	
		if(I2C_Start_CheckFlag == 1 || I2C_Stop_CheckFlag == 1 || I2C_Write_CheckFlag || I2C_Read_CheckFlag || I2C_Address_CheckFlag){
		if(I2C_Start_CheckFlag == 1){
			I2C_Start_SystickCounter++;
		}
		if(I2C_Stop_CheckFlag == 1){
			I2C_Stop_SystickCounter++;
		}
		if(I2C_Write_CheckFlag == 1){
			I2C_Write_SystickCounter++;
		}
		if(I2C_Read_CheckFlag == 1){
			I2C_Read_SystickCounter++;
		}
		if(I2C_Address_CheckFlag == 1){
			I2C_Address_SystickCounter++;
		}
		
		if(I2C_Start_functionCrashCntr == I2C_CRASH_COUNT_5 || I2C_Stop_functionCrashCntr == I2C_CRASH_COUNT_5 || I2C_Write_functionCrashCntr == I2C_CRASH_COUNT_5 || I2C_Read_functionCrashCntr == I2C_CRASH_COUNT_5 || I2C_Address_functionCrashCntr == I2C_CRASH_COUNT_5){
			I2C_Crash_Stop = 1;
		}
	}
}

