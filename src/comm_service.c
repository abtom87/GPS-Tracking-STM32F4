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
