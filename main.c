/*
 * File:   main.c
 * Author: User
 *
 * Created on 06 December 2021, 14:11
 * 
 * 
 * *                  16f1459
 *                  ---------
 *   +5 Volts    1 |Vdd      | 20 0 Volts
    DOWN BUTTON  2 |RA5   RA0| 19   
 *    UP BUTTON  3 |RA4   RA1| 18   
      MCLR       4 |RA3      | 17  
 *               5 |RC5   RC0| 16 
 *               6 |RC4   RC1| 15  
 *               7 |RC3   RC2| 14  
 *               8 |RC6   RB4| 13  SDA
  DIAGNOSTIC LED 9 |RC7   RB5| 12  ENT BUTTON
 *    TX        10 |RB7   RB6| 11  SCL
 *                  ---------
 * 
 * 
 */


#include "config.h"
#include "main.h"
#include "configOsc.h"
#include "configPorts.h"
#include "configEUSART.h"
#include "configBaud.h"
#include "configI2C.h"
#include "i2cStart.h"
#include "i2cWrite.h"
#include "i2cRead.h"
#include "i2cReStart.h"
#include "i2cStop.h"
#include "oLedWrite.h"
#include "configSH1106.h"
#include "setPageAddress.h"
#include "setColumnAddress.h"
#include "clearSH1106.h"
#include "selectDigit.h"
#include "split0To99.h"
#include "write8x8ToOled.h"
#include "writeBMP280.h"
#include "i2cCheckBusIdle.h"
#include "readRegDispOled.h"
#include "readTrimParam.h"
#include "geTadc_T.h"
#include "calcTemp.h"
#include "readPCF8583.h"
#include "bcdToDecimal.h"
#include "zoomChar.h"
#include "writeColon.h"
#include "writePCF8583.h"
#include "decToBcd.h"
#include "UserInterface.h"
void main(void) 
{
    zoomPix xonk;
    uint16_t *num;
    uint8_t decimal = 0;
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;
    uint8_t  i, j;
    splitData woofer;
    //int temp = 0;
    caliTemp result;
    int32_t rawTemp;
    int32_t accurateTemp;
    
    splitData bertie;
    configOsc();
    configPorts();
    configEUSART();
    configBaud();
    configI2C();
    __delay_ms(100);
    configSH1106();
    __delay_ms(100);
    clearSH1106();          // writes 0x00 to each column from 0 to 131, of each page 0 to 7 of the oled display, to clear random data held in display RAM
    writeBMP280(RESET_REG, 0xB6);   // software reset of BMP280
    writeBMP280(CONFIG_REG, 0x0C);
    writeBMP280(CTRL_MEAS_REG, 0x2F);
   
   
    
   

    while(state < 16)
    {
      state = UserInterface(state);
    }
  
    
    while(1)
    {
        
        
        seconds = readPCF8583(0xa0, 0x02);
        decimal = bcdToDecimal(seconds);
        woofer = split0To99(decimal);
        //decimal = bcdToDecimal(minutes);
        //woofer = split0To99(decimal);
        
        
        
         /************************1 page digit*******************************************/
        setColumnAddress(123);
        setPageAddress(0);
        
        num = selectDigit(woofer.tens); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
        setColumnAddress(123);
        setPageAddress(2);
        
        num = selectDigit(woofer.units);// seconds units
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
       
         /************************2 page digit*******************************************/
       
       /**********BMP280 TEMPERATURE display*************/
      /*  setColumnAddress(40);
        setPageAddress(6);
     //   num = selectDigit(bertie.hundreds);
        num = selectDigit(bertie.units);
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
           xonk = zoomChar(*num,2);
           for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
           }
           num++;
        }
        setColumnAddress(40);
        setPageAddress(7);
        //num = selectDigit(bertie.hundreds);
        num = selectDigit(bertie.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,2);
            for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
           }
           num++;
         }*/
        
        /**********************write deg C*****************/
      /*  writeColon(58, 7, 'T', 8); 
        /******decimal point****************************************/
      /*  writeColon(34, 7, 'd', 3);  // write decimal point 12.3
        
        /*******************tens********************************/
     /*   setColumnAddress(18);
        setPageAddress(6);
        // num = selectDigit(bertie.thousands);
        num = selectDigit(bertie.tens);
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
           xonk = zoomChar(*num,2);
           for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
           }
           num++;
        }
        setColumnAddress(18);
        setPageAddress(7);
       // num = selectDigit(bertie.thousands);
         num = selectDigit(bertie.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,2);
            for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
           }
           num++;
         }
        /***********************hundreds******************************************/
    /*   setColumnAddress(0);
        setPageAddress(6);
        // num = selectDigit(bertie.tensThousands);
         num = selectDigit(bertie.hundreds);
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
           xonk = zoomChar(*num,2);
           for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
           }
           num++;
        }
        setColumnAddress(0);
        setPageAddress(7);
        num = selectDigit(bertie.hundreds);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,2);
            for(j = 0; j < 2; j++)
           {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
           }
           num++;
         }*/
        
        minutes = readPCF8583(0xa0, 0x03);
        decimal = bcdToDecimal(minutes);
        woofer = split0To99(decimal);
       
        /************************4 page digit minutes*******************************************/
        
        setColumnAddress(91);
        setPageAddress(0);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
            for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
            }
            num++;
         }
        
        setColumnAddress(91);
        setPageAddress(1);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
            }
            num++;
         }
        
        setColumnAddress(91);
        setPageAddress(2);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte2);
            }
            num++;
         }
        
        setColumnAddress(91);
        setPageAddress(3);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte3);
            }
            num++;
         }
        
        /************************4 page digit minutes***********************/
        
        setColumnAddress(63);
        setPageAddress(0);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
            }
            num++;
         }
        
        setColumnAddress(63);
        setPageAddress(1);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
            }
            num++;
         }
        
        setColumnAddress(63);
        setPageAddress(2);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte2);
            }
            num++;
         }
        
        setColumnAddress(63);
        setPageAddress(3);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte3);
            }
            num++;
         }
        
        hours = readPCF8583(0xa0, 0x04);
        decimal = bcdToDecimal(hours);
        woofer = split0To99(decimal);
        
         /************************4 page digit hours***********************/
        
        setColumnAddress(28);
        setPageAddress(0);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
            }
            num++;
         }
        
        setColumnAddress(28);
        setPageAddress(1);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
            }
            num++;
         }
        
        setColumnAddress(28);
        setPageAddress(2);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte2);
            }
            num++;
         }
        
        setColumnAddress(28);
        setPageAddress(3);
        
        num = selectDigit(woofer.units);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte3);
            }
            num++;
         }
        writeColon(60,2, 'c', 2);
         /************************4 page digit tens hours***********************/
        
        setColumnAddress(0);
        setPageAddress(0);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte0);
            }
            num++;
         }
        
        setColumnAddress(0);
        setPageAddress(1);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte1);
            }
            num++;
         }
        
        setColumnAddress(0);
        setPageAddress(2);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte2);
            }
            num++;
         }
        
        setColumnAddress(0);
        setPageAddress(3);
        
        num = selectDigit(woofer.tens);
         for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            xonk = zoomChar(*num,4);
             for(j = 0; j < 4; j++)
            {
                oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, xonk.zoomedByte3);
            }
            num++;
         }
        /********************BMP280 temperature calculation****************/
            
            result = readTrimParam();
            
            
            rawTemp = geTadc_T();
            accurateTemp = calcTemp(rawTemp, result);
           accurateTemp = accurateTemp;
          //  printf("%f\n", accurateTemp);
            
            bertie = split0To99(accurateTemp);     
            
        setColumnAddress(2);
        setPageAddress(6);
        
        num = selectDigit(bertie.tensThousands); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
        
        setColumnAddress(12);
        setPageAddress(6);
        
        num = selectDigit(bertie.thousands); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
        
        setColumnAddress(22);
        setPageAddress(6);
        
        num = selectDigit(bertie.hundreds); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
        
        setColumnAddress(32);
        setPageAddress(6);
        
        num = selectDigit(bertie.tens); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
        
        setColumnAddress(42);
        setPageAddress(6);
        
        num = selectDigit(bertie.units); // seconds tens
        for (i = 0; i < 8; i++)     // write the selected 8x8 bit map array to the display, using pointer *number which contains the 1st element of the bit map array eg array[0]]
        {
            oLedWrite(SH1106_WRITE_ADDRESS_COMMAND, DISPLAY_DATA_ONE_CTRL_BYTE_DATA_BYTES_TO_FOLLOW, *num);// write the first array element to the display
            num++; // write the 2nd - 8th element of the array to the display
        }
          
    } 
}