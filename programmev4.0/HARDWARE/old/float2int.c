
#include<stdio.h> //printf()
#include<stdlib.h> //atof()
 
typedef struct FloatNode
{
    unsigned int mantissa : 23; //????
    unsigned int exponent : 8; //????
    unsigned int sign : 1;  //???
}FloatNode;
 
int GetSign(const FloatNode *fn); //?????
int GetExp(const FloatNode *fn); //??????
int Float_To_Int(float num); //??????


int GetSign(const FloatNode *fn) //?????
{
    return fn->sign == 1 ? -1 : 1;
}
 
int GetExp(const FloatNode *fn) //?????
{
    return (fn->exponent - 127); //??IEEE754,??127????????
}
 
int Float_To_Int(float num) //?float???int
{
    FloatNode *fn = (FloatNode*)&num;
    int exp = GetExp(fn);
 
    if(exp >= 0)
    {
        int mov = 23 - exp;
        int res = (fn->mantissa | 1<<23) >> mov;
        return res*GetSign(fn);
    }
    else
    {
        return 0;
    }
