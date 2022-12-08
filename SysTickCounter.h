


/********************************* INTERRUPT DEFINITIONS ******************************/
#define DISABLE_INTERRUPTS 	0xFFFFFFFF
#define ENABLE_INTERRUPS   	0x008001E0
#define MAIN_LOOP_TIMEOUT_1	40
#define MAIN_LOOP_TIMEOUT_2	40

extern unsigned int MainLoopProtection ;
extern unsigned int MainLoopFlag	     ;												// 5ms'lik main  döngüsüne girebilmek icin olusan flag
extern unsigned char OSSD_PreStartFlag ;								// Alan Tarayici Sürücü Enerji Kesmesi Sonrasi Yeniden Mühürleme Ön Flag
extern unsigned char MainSceneDataReq  ;								// Ana Sahnenin Ihtiyac Duydugu Datalarin Periyodik Gönderilmesi Icin Flag
extern unsigned int MainLoopController ;
extern unsigned char ReflectorNotDetected;								// 3 sn boyunca Reflector Bulunamazsa Bu Flag SET olur 




/*************SYSTICK COUNTER(24 BIT SAYICI) SAYICISINI************
							       VE KESMESINI INITIALIZE ET
									
									--> [1/(168mhz)]*(84001-1)=0.5 ms  
								  --> Her 0.5 ms'de bir counter kesme olusturacak
						
*******************************************************************/
void SysTickBaslat(void);
extern int msTick;	