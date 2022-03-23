#include "config.h"

int8_t binaryToDecimal (uint8_t num, uint8_t minusFlag)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
         num = (num & 0b00000001 <<i) | num;
    }
    if(minusFlag == 0)
    {
        return num;
    }
    else if(minusFlag == 1)
    {
       return num * (-1);
    }
}
