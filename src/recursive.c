#include <stdio.h>
#include <stdlib.h>

#define computeall(x) ((x >> 0) & 1)
#define computecomplete(x) ((x >> 1) & 1)
#define printtable(x) ((x >> 2) & 1)


int recursiveComputeWrapper(
    long target,
    int n,
    long* elements,
    __uint8_t settings,
    __uint8_t verbosity
    )
{




}


int recursiveComputeInner(
    long target,
    int n,
    long* elements,
    __uint8_t settings,
    __uint8_t verbosity,
    int navailable,
    __uint8_t* available /* mask for elements, length n */
    )
{
    if ( navailable == 0 )
    {
        return target == 0;
    }

    if ( target == 0 && !computeall(settings) )
    {
        return 1;
    }

    
}