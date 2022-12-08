

#define DAC_PCF8591_1 										0x94
#define DAC_PCF8591_2 										0x96

#define	PCF8591_DAC_COMMEND 		  				0x40


#define FUNCTION_CRASH_COUNT_1MS					2
#define FUNCTION_CRASH_COUNT_2MS					4
#define FUNCTION_CRASH_COUNT_5MS					10
#define FUNCTION_CRASH_COUNT_10MS					20
#define FUNCTION_CRASH_COUNT_15MS					30
#define FUNCTION_CRASH_COUNT_30MS					60

#define I2C_CRASH_COUNT_5									5
#define I2C_CRASH_COUNT_7									5	
#define I2C_CRASH_COUNT_10								5	
#define I2C_CRASH_COUNT_15								5	

#define KONTAK_AC    0x40
#define KONTAK_KAPAT  0x66

//extern unsigned char I2C_Crash_Stop;		// I2C haberlesmesi belirli bir sayida arka arkaya crash yerse araci durdurur

/****************functionCrash Degiskenleri***************************

		 Slave cihazdan geri dönüs olmadigi için sonsuz while 
		kontrolünde belirli bir süre takilirsa functionCrash 
		flagleri kalkar ve I2C haberlesmesine reset atilir.
		
**********************************************************************/
extern unsigned int I2C_Start_functionCrashCntr;
extern unsigned int I2C_Stop_functionCrashCntr;
extern unsigned int I2C_Write_functionCrashCntr;
extern unsigned int I2C_Read_functionCrashCntr;
extern unsigned int I2C_Address_functionCrashCntr;





/****************functionStop Degiskenleri***************************

	   FunctionCrash flargleri kalkmasi sebebi ile
		belirli bir sayi reset almis ve hala problem devam 
		ediyor ise araca stop verdirilir.
		
**********************************************************************/
extern unsigned char I2C_Start_functionStop;
extern unsigned char I2C_Stop_functionStop;
extern unsigned char I2C_Write_functionStop;
extern unsigned char I2C_Read_functionStop;
extern unsigned char I2C_Address_functionStop;



/****************CheckFlag Degiskenleri******************************

	    Sonsuz while'in icine girdini ve systick counter'larin
		 saymaya baslamasi gerektigini belirtir.
		 
		  Logic 0'a geri dönmesi sonsuz while'den ciktigini belirtir.
		
**********************************************************************/
extern unsigned char I2C_Start_CheckFlag;
extern unsigned char I2C_Stop_CheckFlag;
extern unsigned char I2C_Write_CheckFlag;
extern unsigned char I2C_Read_CheckFlag;
extern unsigned char I2C_Address_CheckFlag;



/****************SystickCounter Degiskenleri**************************

	     CheckFlag kaltiginda sonsuz while'in icinde oldugunu
			anlar ve systick kesmesinde saymaya baslar
		
**********************************************************************/
extern unsigned int I2C_Start_SystickCounter;
extern unsigned int I2C_Stop_SystickCounter;
extern unsigned int I2C_Write_SystickCounter;
extern unsigned int I2C_Read_SystickCounter;
extern unsigned int I2C_Address_SystickCounter;



extern unsigned char I2C_Crash_Stop;		// I2C haberlesmesi belirli bir sayida arka arkaya crash yerse araci durdurur



/************************************init_i2c_user(void)*************************************************
*		I2C için gerekli GPIO ayarlari yapilir																															*
*		I2C iletisiminin baslamas1 için gerekli i2c registerlari configure edirli														*
*																																																				*
********************************************************************************************************/
void init_i2c_user(void);




/***********************I2C_Start()************************************
*	I2C iletisimine baslanmadan cagirilmasi gerekir											*	
*																																			*
*		- SDA logic 0'a çekilir(Start biti)         											*
*   - clock sinyali gönderilmeye baslanir														  *
**********************************************************************/
void I2C_Start(void);




/***********************I2C_Stop(void)*********************************
*	I2C iletisimi bitiminde cagirilmasi gerekir													*	
*																																			*
*		- SDA logic 1'a çekilir(Stop biti)          											*
*   - clock sinyali durur																							*
**********************************************************************/
void I2C_Stop (void);



/***********************void I2C_Write(unsigned char)*****************
*	I2C iletisimi baslatildiktan sonra bir byte bilgi gönderir					*	
*																																			*
*																																			*
**********************************************************************/
void I2C_Write (unsigned char);




/***********************unsigned char I2C_Read(int)*******************
*	I2C iletisiminde okuma yapar																				*	
*																																			*
*																																			*
**********************************************************************/
unsigned char I2C_Read(int);




/***********************I2C_Start()************************************
*	I2C iletisiminde write islemi yapilicak cihazin adresi	            *	
*	 gönderilir																													*	
*																																			*
*																																			*
**********************************************************************/
void I2C_Address (unsigned char);



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
int i2c_digitalToAnalog(unsigned char);




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
int i2c_digitalToAnalog_withAdress(unsigned char, unsigned char);



void AnalogOutEZS350Steering(unsigned char targetSteeringAngle);



