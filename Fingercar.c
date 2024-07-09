#include <htc.h>
#include "smcl_lcd8.h"
#include "AT_serial.h"
sbit ent=P1^0;
sbit inc=P1^1;
sbit dec=P1^2;
sbit set=P1^2;

unsigned char rc=0,data_rcv[86],i,fp_id,fpid1,count=0,chk_sum,va,k=0;
bit chk0,chk1,chk2; 
void fpid(unsigned char val);
void ent_fpid();
void result();
 
void fd()
	{
		Lcd8_Command(0x01);
	//	 Lcd8_Display(0x80,"Finger Print    ",16);   
		 Lcd8_Display(0xc0,"Welcome         ",16); 
	}

void display(unsigned char val)
{
	switch(val)
	{ 	
		case 1:
		Lcd8_Display(0xc0,"--Enrl Usr FP.--",16);
		break;
		
		case 2:
		Lcd8_Display(0xc0,"--identify FP.--",16);

		break;
	
		case 3:
		Lcd8_Display(0xc0,"--Del All FP.---",16);
		break;

		default:		
		fd();
		break;
	}
}
void Serial() interrupt 4
{
	if(RI)
	{
		RI=0;
		data_rcv[rc]=SBUF;
			rc++;
	}
}


void txs(unsigned char val)
{
	Serial_Out(val);
}

void delay1()
	{
	 long i;
	 for(i=0;i<50000;i++);
	}					

void delay2()
	{
	 int i;
	 for(i=0;i<15000;i++);
	}


	void delay3(unsigned int del)
	{
	while(del--);
 	}

void sendd1()
	{
   	  txs('A');txs('T');txs('+');txs('C');txs('M');txs('G');
	  txs('S');txs('=');	  
		txs('"');
	  txs('9');txs('7');txs('8');txs('9');
	  txs('7');txs('2');txs('8');txs('7');txs('9');txs('0');	  
		txs('"');
	  txs(13); txs(10);
		delay3(60000); delay3(60000);delay3(60000); delay3(60000);delay3(60000); delay3(60000);
	}


void sms1()
	{
	  sendd1();	 	  delay3(10000);
		txs('A');txs('l');txs('e');txs('r');txs('t');    
	txs(26);
	 delay3(60000); delay3(60000);delay3(60000); delay3(60000);delay3(60000); delay3(60000);

	}




unsigned char rxs()
	{
		int c=0;	
	 	while(RI==0)
			{
			 c++;
			 if(c>25000)
			 	break;

			}
		RI=0;
		return SBUF;
	}


void rxmo()
	{
		 char i;
	 txs('A');txs('T');txs('+');txs('C');txs('M');txs('G');
					 txs('R');txs('=');txs('1');txs(13);  txs(10);

					 for(i=0;i<86;i++)
					 	{
						 data_rcv[i]=rxs();	
						 }
	}


void del1()
	{
				char i;
	 			txs('A');txs('T');txs('+');txs('C');txs('M');txs('G');
			    txs('D');txs('=');txs('1');txs(13);	 txs(10);
			 	for(i=0;i<13;i++)
			 	{
				 data_rcv[i]=rxs();							 
				}
	}

void main()
{
	Lcd8_Init();
		EA=1;
	  SCON=0x50;
		PCON=PCON | 0x80;
    TMOD=0X20;
    TH1=0XFf;
    TR1=1;

	
	txs('A');txs('T');txs(13);txs(10);
		delay1();
	txs('A');txs('T');txs('+');txs('C');txs('M');txs('G');
	txs('F');txs('=');txs('1');txs(13);txs(10);
Delay(60000);
			  del1();


	Lcd8_Display(0x80,"Show Finger      ",16);

	
	fd();	
	Receive(1); 
	Delay(65000);
	  for(k=0;k<=80;k++)	 
	   {
	   data_rcv[k]=0;
	   }
			 while(1)
	{ 

			if(!inc && !chk0)chk0=1;
			
			if(inc && chk0){count++;if(count>5)count=0;chk0=0;}
			
			if(!ent && !chk1)chk1=1;
			if(ent && chk1){chk1=0;fpid(count);}
	
			if(!dec && !chk2)chk2=1;
			if(dec && chk2){count--;if(count>5)count=5;chk2=0;}	
			
			display(count);	
	

	if(rc>10)
		{
			
		
	  for(k=0;k<=49;k++)	 //11
		{
		 Lcd8_Decimal3(0xc0,data_rcv[k]);
	
		 Delay(6500);
		 
		}
		
		Lcd8_Decimal3(0x80,data_rcv[49]);//
		Lcd8_Decimal3(0x85,data_rcv[47]); 
 
	 
		Lcd8_Decimal3(0x8c,rc);
		Delay(6500);
		
		result(); 
		rc=count=0;
		for(k=0;k<=80;k++)	 
	   {
	   data_rcv[k]=0;
	   } 
		}

	} 
}


 void fpid(unsigned char val)
{
	Lcd8_Display(0xc0,"--Processing.---",16);
	for(rc=0;rc<50;rc++)data_rcv[rc]=0;
	rc=0;
	switch(val)
	{
		  case 1:	  			
			ent_fpid();
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x03\x01\x00\x05",12);
			Delay(65000);Delay(65000);
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x04\x02\x01\x00\x08",13);
			Delay(65000);Delay(65000);
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x03\x01\x00\x05",12);
			Delay(65000);Delay(65000);	
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x04\x02\x02\x00\x09",13);
			Delay(65000);Delay(65000);
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x03\x05\x00\x09",12);
			Delay(5000);
			Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x06\x06\x02",11);
			Serial_Out(0);
			Serial_Out(fp_id);
			chk_sum=fp_id+15;
			Serial_Out(0);
			Serial_Out(chk_sum);
			Delay(65000);Delay(65000);Delay(65000) ;
			i=1;
			break;

		case 2:		  
		
			
		Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x03\x01\x00\x05",12);  //read	or search
		Delay(65000);Delay(65000);Delay(40000);//Delay(30000);
		Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x04\x02\x01\x00\x08",13);//read
		Delay(40000);Delay(20000);//Delay(10000);
		Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x08\x1b\x01\x00\x00\x01\x01\x00\x27",17);//read 	
   		Delay(65000);Delay(65000);i=2;

		
		break;

	 
		case 3://EF 01 FF FF FF FF 01 00 03 0D 00 11
		Serial_Conout("\xEF\x01\xFF\xFF\xFF\xFF\x01\x00\x03\x0D\x00\x11",12);
		i=4;			 
		break;
	  

	
		default:
		break; 
	}
		
	fd();
}

void ent_fpid()
{
	Lcd8_Display(0x80,"Enter ID No: 000",16);
	fp_id=0;		
	while(ent)
	{
		if(!inc){while(!inc);fp_id++;if(fp_id>=255)fp_id=0;Lcd8_Decimal3(0x8D,fp_id);}	
		if(!dec){while(!dec);fp_id--;if(fp_id>=255)fp_id=255;Lcd8_Decimal3(0x8D,fp_id);}
	}					 
}



void result()
{	
char flg=0,hbt=0,hbtt=0,hb=0,f1=0;
  Lcd8_Command(0x01);	
	 	 Delay(65000);
  if(i==1)
  {
		   	if((data_rcv[69]==0x00)&&(rc>70))
			{
			 Lcd8_Display(0xC0,"    Success     ",16);
			}
			else if(data_rcv[69]==0x02)
			{
			 Lcd8_Display(0x80,"   No Finger    ",16);
			}
			else
			{
			  Lcd8_Display(0xC0,"  Not  Success  ",16);
			}
		   	i=0; 
  }

  else if(i==2)
  {
		  	if((data_rcv[33]==0x00)&&(data_rcv[35]!=0))
			{
			 	Lcd8_Display(0x80,"    Success     ",16);
				Lcd8_Display(0xc0," Finger ID:     ",16);
				Lcd8_Decimal3(0xcb,data_rcv[35]);
		Lcd8_Display(0x80,"IR  SENSOR       ",16);	
	while(P0_6==1)
	{

	}			
			
				while(1)
				{
					
				


	//	Lcd8_Display(0x80,"Vibration        ",16);		
	//while(P3_5==1)
	//{		
	//}

	Lcd8_Display(0xc0,"ALCOHOL CHECK   ",16);		
	if(P0_7==1)
	{
	Lcd8_Display(0xc0,"ALCOHOL DETECTED   ",16);
	sms();		
	while(1);		
	}					
	


		  
	if(P0_5==1)
		{
		if(flg==0)
		if(hbt<100)
		{
		 hbt++;
		 flg=1;
		}
		}
	if(P0_5==0)
		{
		 flg=0;
		}			
	hb++;
	if(hb==120)
		{
			hbtt=hbt*3;
			hbt=0;
			hb=0;
		}

	Lcd8_Command(0x80);	
	Lcd8_Write(0x80,'H');
	Lcd8_Write(0x81,'e');
	Lcd8_Write(0x82,'a');
	Lcd8_Write(0x83,'r');
	Lcd8_Write(0x84,'t');
	Lcd8_Write(0x85,' ');
	Lcd8_Write(0x86,'B');
	Lcd8_Write(0x87,'t');
	Lcd8_Write(0x88,'=');
	Lcd8_Write(0x89,((hbtt/100)+0x30));
	Lcd8_Write(0x8a,(((hbtt%100)/10)+0x30));
	Lcd8_Write(0x8B,((hbtt%10)+0x30));	
	Lcd8_Write(0x8c,' ');
	Lcd8_Write(0x8d,((hbt/100)+0x30));
	Lcd8_Write(0x8E,(((hbt%100)/10)+0x30));
	Lcd8_Write(0x8F,((hbt%10)+0x30));	


			  if(hbtt>10)
			  	{
				 if(hbtt<50)
				 	{
						if(f1==0)
						{
					  sms();
					  P0_1=0;
					  f1=1;
					  }
					}
				 else
				 	{
					f1=0;
					 P0_1=1;
					}
				}
					
				}
				
			}
			else //if(data_rcv[33]==0x09)
			{
					sms1();
					P0_0=0;	
				Lcd8_Display(0x80,"Not Find Finger ",16);
				ES=0;
			
				while(1)
				{

			rxmo();
				delay1();
					delay1();
			Lcd8_Write(0xc0,data_rcv[72]);
			Lcd8_Write(0xc1,data_rcv[73]);
			Lcd8_Write(0xc2,data_rcv[74]);
			Lcd8_Write(0xc3,data_rcv[75]);
			Lcd8_Write(0xc4,data_rcv[76]);
			Lcd8_Write(0xc5,data_rcv[77]);
			Lcd8_Write(0xc6,data_rcv[78]);
			Lcd8_Write(0xc7,data_rcv[79]);
			Lcd8_Write(0xc8,data_rcv[80]);
			Lcd8_Write(0xc9,data_rcv[81]);
			Lcd8_Write(0xcA,data_rcv[82]);
			Lcd8_Write(0xcB,data_rcv[83]);
			Lcd8_Write(0xcC,data_rcv[84]);
			Lcd8_Write(0xcD,data_rcv[85]);




			if(data_rcv[75]=='H')
				{
				del1();
				delay1();

	Lcd8_Display(0x80,"IR  SENSOR       ",16);	
								while(P0_6==1)
								{
							
								}			



							  				while(1)
				{
					
				


	//	Lcd8_Display(0x80,"Vibration        ",16);		
	//while(P3_5==1)
	//{		
	//}

	Lcd8_Display(0xc0,"ALCOHOL CHECK   ",16);		
	if(P0_7==1)
	{
	Lcd8_Display(0xc0,"ALCOHOL DETECTED   ",16);
	sms();		
	while(1);		
	}					
	  
	if(P0_5==1)
		{
		if(flg==0)
		if(hbt<100)
		{
		 hbt++;
		 flg=1;
		}
		}
	if(P0_5==0)
		{
		 flg=0;
		}			
	hb++;
	if(hb==120)
		{
			hbtt=hbt*3;
			hbt=0;
			hb=0;
		}

	Lcd8_Command(0x80);	
	Lcd8_Write(0x80,'H');
	Lcd8_Write(0x81,'e');
	Lcd8_Write(0x82,'a');
	Lcd8_Write(0x83,'r');
	Lcd8_Write(0x84,'t');
	Lcd8_Write(0x85,' ');
	Lcd8_Write(0x86,'B');
	Lcd8_Write(0x87,'t');
	Lcd8_Write(0x88,'=');
	Lcd8_Write(0x89,((hbtt/100)+0x30));
	Lcd8_Write(0x8a,(((hbtt%100)/10)+0x30));
	Lcd8_Write(0x8B,((hbtt%10)+0x30));	
	Lcd8_Write(0x8c,' ');
	Lcd8_Write(0x8d,((hbt/100)+0x30));
	Lcd8_Write(0x8E,(((hbt%100)/10)+0x30));
	Lcd8_Write(0x8F,((hbt%10)+0x30));	


			  if(hbtt>10)
			  	{
				 if(hbtt<50)
				 	{
						if(f1==0)
						{
					  sms();
					  P0_1=0;
					  f1=1;
					  }
					}
				 else
				 	{
					f1=0;
					 P0_1=1;
					}
				}
					
				}





				}
			if(data_rcv[85]=='H')
				{
				del1();
				delay1();
						
						 		Lcd8_Display(0x80,"IR  SENSOR       ",16);	
								while(P0_6==1)
								{
							
								}			



							  				while(1)
				{
					
				


	//	Lcd8_Display(0x80,"Vibration        ",16);		
	//while(P3_5==1)
	//{		
	//}

	Lcd8_Display(0xc0,"ALCOHOL CHECK   ",16);		
	if(P0_7==1)
	{
	Lcd8_Display(0xc0,"ALCOHOL DETECTED   ",16);
	sms();		
	while(1);		
	}					
	


		  
	if(P0_5==1)
		{
		if(flg==0)
		if(hbt<100)
		{
		 hbt++;
		 flg=1;
		}
		}
	if(P0_5==0)
		{
		 flg=0;
		}			
	hb++;
	if(hb==120)
		{
			hbtt=hbt*3;
			hbt=0;
			hb=0;
		}

	Lcd8_Command(0x80);	
	Lcd8_Write(0x80,'H');
	Lcd8_Write(0x81,'e');
	Lcd8_Write(0x82,'a');
	Lcd8_Write(0x83,'r');
	Lcd8_Write(0x84,'t');
	Lcd8_Write(0x85,' ');
	Lcd8_Write(0x86,'B');
	Lcd8_Write(0x87,'t');
	Lcd8_Write(0x88,'=');
	Lcd8_Write(0x89,((hbtt/100)+0x30));
	Lcd8_Write(0x8a,(((hbtt%100)/10)+0x30));
	Lcd8_Write(0x8B,((hbtt%10)+0x30));	
	Lcd8_Write(0x8c,' ');
	Lcd8_Write(0x8d,((hbt/100)+0x30));
	Lcd8_Write(0x8E,(((hbt%100)/10)+0x30));
	Lcd8_Write(0x8F,((hbt%10)+0x30));	


			  if(hbtt>10)
			  	{
				 if(hbtt<50)
				 	{
						if(f1==0)
						{
					  sms();
					  P0_1=0;
					  f1=1;
					  }
					}
				 else
				 	{
					f1=0;
					 P0_1=1;
					}
				}
					
				}








						

				}		

				}
			}
//			Serial_Init(9600);
			i=0; 
  }

   else if(i==3)
  {

			if(data_rcv[9]==0x00)
			{
			//	Lcd8_Display(0x80,"    Finger Id   ",16);
				Lcd8_Display(0xc0," Deleted        ",16);
			}
			else if(data_rcv[9]==0x10)
			{
			//	Lcd8_Display(0xC0,"Fail To delete  ",16);
			//	Lcd8_Display(0xc0," Finger Id      ",16);
			}
			else if(data_rcv[9]==0x01)
			{
			//	Lcd8_Display(0x80,"Receiving       ",16);
				Lcd8_Display(0xc0," Error          ",16);
			}
			i=0;
  }
  			else if(i==4)
  			{

				 if(data_rcv[9]==0x00)
				{
					Lcd8_Display(0xc0," Deleted        ",16);
				}

				else if(data_rcv[9]==0x11)
				{
				//	Lcd8_Display(0x80,"Fail To delete  ",16);
				//	Lcd8_Display(0xc0,"Finger Id's     ",16);
				}
				else if(data_rcv[9]==0x01)
				{
			//		Lcd8_Display(0x80,"Receiving       ",16);
			//		Lcd8_Display(0xc0," Error          ",16);
				}
				i=0;

			}
			else if(i==5)
  			{								 
			 va=data_rcv[49]-data_rcv[47];
			 Lcd8_Decimal3(0xcA,va);
			 Delay(65000);Delay(65000);
			 //if(va==0x0c||va==12)
			 if(data_rcv[45]==0x00&&va==12)
			 {
			  //	Lcd8_Display(0x80,"Matching        ",16);
				Lcd8_Display(0xc0," Success        ",16);
			 }
			 else if(va==13)
			 {

				//Lcd8_Display(0x80,"Receiving       ",16);
				Lcd8_Display(0xc0," Error          ",16);
			 }
			 else
			 {
				//Lcd8_Display(0x80,"Matching        ",16);
				Lcd8_Display(0xc0," Not Success    ",16);
				 							P3_7=1;
					P3_5=0;
					P0=0;	
			 } i=0;

			}
  	 		Delay(6500);

 }