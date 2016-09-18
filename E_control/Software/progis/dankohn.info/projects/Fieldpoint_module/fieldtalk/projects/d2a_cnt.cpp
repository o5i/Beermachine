/**
 * @internal
 * @file d2a_cnt.cpp
 *
 * @if NOTICE
 *
 * $Id: d2a_cnt.cpp,v 1.0 2009/05/12 Daniel Kohn
 *
 * based on tcpsimple.cpp
 *
 * Copyright (c) 2002 FOCUS Software Engineering, Australia.
 * All rights reserved. <www.focus-sw.com>
 *
 * This file is for demonstration purposes only.
 *
 * No part of this material may be reproduced or transmitted in any
 * form or by any means or used to make any derivative work without
 * express written consent from the copyright holders.
 *
 * This material is provided "AS IS", WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. Any use is at your own risk.
 *
 * @endif
 */


// Platform header
#include <stdio.h>
#include <stdlib.h>

// Include FieldTalk package header
#include "MbusTcpMasterProtocol.hpp"



/*****************************************************************************
 * String constants
 *****************************************************************************/

const char versionStr[]= "$Revision: 0.99 $";
const char progName[] = "d2a_cnt";
const char bannerStr[] =
"%s - FieldTalk(tm) Modbus(R) D/A Setup Utility\n"
"Copyright (c) 2002-2004 FOCUS Software Engineering Pty Ltd\n"
#ifdef __WIN32__
"Getopt Library Copyright (C) 1987-1997	Free Software Foundation, Inc.\n"
#endif
;

const char usageStr[] =
"%s [options] host \n"
"Arguments: \n"
"host          Host name or dotted ip address when using MODBUS/TCP protocol \n"
"General options: \n"
"-a #          Slave address (1-255 for RTU/ASCII, 0-255 for TCP, 1 is default)\n"
"-r #          register\n"
"-v #          value to send (0 - 65535)\n"
"";

/*****************************************************************************
 * Gobal data
 *****************************************************************************/

char *hostName;
MbusTcpMasterProtocol mbusProtocol;


/*****************************************************************************
 * Function implementation
 *****************************************************************************/

/**
 * Opens protocol
 */
void openProtocol()
{
   int result;

   result = mbusProtocol.openProtocol(hostName);
   if (result != FTALK_SUCCESS)
   {
      fprintf(stderr, "Error opening protocol: %s!\n",
                       getBusProtocolErrorText(result));
      exit(EXIT_FAILURE);
   }
}


/**
 * Closes protocol
 */
void closeProtocol()
{
   mbusProtocol.closeProtocol();
}


/**
 * Main function.
 *
 * @return Error code: 0 = OK, else error
 */
int main(int argc, char *argv[])
{
   int result,c, slaveAddr, regAddr;
   short int regVal;
   



   while((c = getopt(argc, argv, "a:r:v:")) != -1)
   {
      switch(c)
      {
         case'a':
            slaveAddr=strtol(optarg, NULL, 0);
            if ((slaveAddr<0) || (slaveAddr > 255))
            {
               printf("Invalid slave address parameter (-a) \n");
               exit(EXIT_FAILURE);
            }
         break;
         case'r':
            regAddr=strtol(optarg, NULL, 0);
            if ((regAddr<1) || (regAddr > 0x10000))
            {
               printf("Invalid bit address (-b) \n");
               exit(EXIT_FAILURE);
            }   
         break;
         case'v':
            regVal=strtol(optarg, NULL, 0);
            if ((regVal<0) || (regVal > 0xffff))
            {
               printf("Invalid bit value (-v) \n");
               exit(EXIT_FAILURE);
            }   
         break;
         default:
            printf("Unrecognized option or missing option parameter\n");
            exit(EXIT_FAILURE);
         break;
      }
   }

   hostName = argv[optind];

   printf(bannerStr, progName);
   
   openProtocol();

   result=mbusProtocol.writeSingleRegister(slaveAddr, regAddr, regVal);

   closeProtocol();

   return (EXIT_SUCCESS);
}
