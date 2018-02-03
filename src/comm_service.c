/*
 * comm_service.c
 *
 *  Created on: Jan 28, 2018
 *      Author: abel
 */

#include "comm_service.h"

void parser_gprmc(char *str_to_parse)
{
   static int k=0;
   char *substr;
   substr = strstr(str_to_parse,"$GPRMC");

   while(substr[k]!='\n')
   {
      USART_TX_byte(substr[k]);
      k++;
   }
  substr[k]=0;

   USART_TX_byte('\n');

}

void Display_Time_LCD(char *str_to_parse, char *str_to_extract, const uint8_t offset)
{
	int k=0;
	volatile char *substr;
	char Time_buffer[10]={0};
    char gps_buffer[1024]={0};

	for(int i=0;i<strlen(str_to_parse);i++)
		gps_buffer[i] =  str_to_parse[i];


	            substr = strstr(gps_buffer,str_to_extract);
				k=offset;
				while(k<13)
				{
			    	Time_buffer[k-7]=(substr[k]);
			    	k++;
				}

				if(Time_buffer[1] == 0x39)
				{
					Time_buffer[1] = 0x30;
					Time_buffer[0]+=1;
				}
				else
					Time_buffer[1] += 1;


				LCD_Goto(7,1);
				LCD_Write_String(Time_buffer);

}
