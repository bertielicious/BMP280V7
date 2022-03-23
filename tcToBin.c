#include "config.h"
#include "binaryToDecimal.h"
/****************************algorithm*****************************************************************
check bit7 of the 2's compliment byte to determine if it is '1' or '0'
if bit7 = 0 // 2's compliment number is positive
    convert the byte directly to decimal by modulo division from 128, 64, 32, 16, 8, 4, 2 to 1
else if bit7 = 1// 2's compliment number is negative
    invert each bit of the byte
    add 1
    convert the byte to decimal (don't forget the minus sign)
end
inputs: 2's compliment byte
output: decimal number ranging from +127 to -128
*******************************************************************************************************/
int8_t tcToBin(uint8_t twosCompNumber)
{
    int8_t decimalNum;
    uint8_t checkPolarity = 0b10000000;
    uint8_t minusF;
    if((checkPolarity & twosCompNumber) == 128)// check bit7 for '1' = -ve number
    {
        minusF = 1;
        printf("check for negative num, %d\n", checkPolarity & twosCompNumber);
        printf("negative number\n");
        twosCompNumber = ~twosCompNumber;         //invert the TC byte
        twosCompNumber = twosCompNumber + 1;
        decimalNum = binaryToDecimal(twosCompNumber, minusF);
    }
    else if((checkPolarity & twosCompNumber) == 0)// check bit7 for '' = +ve number
    {
        minusF = 0;
        printf("check for positive num, %d\n", checkPolarity & twosCompNumber);
        printf("positive number\n");
        decimalNum = binaryToDecimal(twosCompNumber, minusF);   // convert the positive TC number to decimal
    }
        return decimalNum;
}