#include "config.h"
#include "readBMP280.h"
#include "tcToBin.h"
 /*******************************************************************************************************************
 * Function: readTrimParam
 * Overview: function reads temperature or pressure trim parameters into struct and returns whole struct to caller
 * Input: none
 * Output: struct containing Trim Parameters digT1, digT2, digT3
 *******************************************************************************************************************/
    
caliTemp readTrimParam(void)
{
    caliTemp word;
    uint8_t numLsb, numMsb;
   // int8_t decNumLsb;  // decNum contains the decimal equivalent of the TC byte read (range -128 to +127)
    //int8_t decNumMsb; 
    numLsb = readBMP280(0x88); // read TC contents of lsbT1
    numMsb = readBMP280(0x89); // read TC contents of msbT1
    
    //decNumLsb = tcToBin(numLsb);//convert from TC to decimal
    //decNumMsb = tcToBin(numMsb);
    //word.digT1 = (decNumMsb<<8) | decNumLsb;// combine the two 8bit decimal numbers into a 16 bit word
    word.digT1 = (numMsb<<8) | numLsb;
    numLsb = readBMP280(0x8a); // read contents of lsbT2
    numMsb = readBMP280(0x8b); // read contents of msbT2
    //decNumLsb = tcToBin(numLsb);//convert from TC to decimal
    //decNumMsb = tcToBin(numMsb);
    //word.digT2 = (decNumMsb<<8) | decNumLsb;
    word.digT2 = (numMsb<<8) | numLsb;
    numLsb = readBMP280(0x8c); // read contents of lsbT3
    numMsb = readBMP280(0x8d); // read contents of msbT3
    //decNumLsb = tcToBin(numLsb);//convert from TC to decimal
    //decNumMsb = tcToBin(numMsb);
    //word.digT3 = (decNumMsb<<8) | decNumLsb;
    word.digT3 = (numMsb<<8) | numLsb;
    
    return word;                // return struct containing members word.digT1, word.digT2, word.digT3
}
