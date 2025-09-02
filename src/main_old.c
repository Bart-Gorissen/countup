#include <stdio.h>
#include <stdlib.h>

/**
 * Reads command line input
 * input data: <n> [x1 x2 ... xn]: number of elements followed by the elements
 * flags:
 * -f, --file : input file (following the above convention on single line)
 * -a, --all : use all elements
 * -c, --complete : exhaustive search (all valid combinations are reported)
 * -m, --mode : mode (recursive 0, table 1)
 * -t, --table : print table of reachable numbers (following -all)
 * -v, --verbose : verbose output
 * -h, --help : print help message 
 * 
 * in settings: record (from least to most significant bit):
 * - bit 0: all
 * - bit 1: complete
 * - bit 2: table
 * - bit 3-7: mode (0 - recursive, 1 - table)
 * 
 * in verbosity: record (from least to most significant bit):
 * - bit 0: report all solutions encountered
 * - ...
 */
int inputreader(
    int argc,
    const char** argv,
    long* target,
    int* n,
    long** elements,
    __uint8_t* settings,
    __uint8_t* verbosity
    )
{
    const char* USAGEMESSAGE =
        "Usage: <target> <n> [x1 x2 ... xn]\n"
        "Flags:\n"
        "  -f, --file     : input file (following the above convention on single line)\n"
        "  -a, --all      : use all elements\n"
        "  -c, --complete : exhaustive search (all valid combinations are reported)\n"
        "  -m, --mode     : mode (recursive 0, table 1)\n"
        "  -t, --table    : print table of reachable numbers (following -all)\n"
        "  -v, --verbose  : verbose output\n"
        "  -h, --help     : help message\n";

    *target = -1;
    *n = -1;
    int nelements = 0;
    *settings = 0;
    *verbosity = 0;

    if (argc < 2)
    {
        printf("No input provided\n%s", USAGEMESSAGE);
        return 1;
    }

    char openflag = 0;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if ( argv[i][0] == 'h' )
            {
                printf("%s", USAGEMESSAGE);
                if ( *elements != NULL )
                {
                    free(*elements);
                }
                return 0;
            }

            if ( openflag != 0 )
            {
                printf("Flag %c given without value\n%s", openflag, USAGEMESSAGE);
                if ( *elements != NULL )
                {
                    free(*elements);
                }
                return 1;
            }

            openflag = argv[i][1];

            if ( openflag == 'a' )
            {
                *settings |= 1 << 0;
                openflag = 0;
            }
            else if ( openflag == 'c' )
            {
                *settings |= 1 << 1;
                openflag = 0;
            }
            else if ( openflag == 't' )
            {
                *settings |= 1 << 2;
                openflag = 0;
            }
            else if ( openflag == 'm' )
            {

            }
            else if ( openflag == 'v' )
            {
                
            }
            else if ( openflag == 'f' )
            {
                
            }
            else
            {
                printf("Unknown flag: %c\n%s", openflag, USAGEMESSAGE);
                if ( *elements != NULL )
                {
                    free(*elements);
                }
                return 1;
            }
        }
        else
        {
            /* if we are dealing with a flag, then proceed with binary*/
            if ( openflag != 0 )
            {
                /* TODO: read from file */

                /* compute the binary encoding provided after the flag */
                __uint8_t value = 0;
                int j = 0;
                while (argv[i][j] != '\0')
                {
                    value = value * 10;
                    int digit = argv[i][j] - '0';
                    if ( digit < 0 || digit > 9 )
                    {
                        printf("Invalid character '%c' in argument %d. Expected a digit\n%s", argv[i][j], i, USAGEMESSAGE);
                        if ( *elements != NULL )
                        {
                            free(*elements);
                        }
                        return 1;
                    }
                    value += digit;
                    j++;
                }

                if ( openflag == 'm' )
                {
                    if ( value > 1 )
                    {
                        printf("Invalid mode value: %d. Expected 0 or 1\n%s", value, USAGEMESSAGE);
                        if ( *elements != NULL )
                        {
                            free(*elements);
                        }
                        return 1;
                    }
                    *settings |= value << 3; /* store mode in bits 3-7 */
                }
                else if ( openflag == 'v' )
                {
                    if ( value > 1 )
                    {
                        printf("Invalid verbosity value: %d. Expected 0 or 1\n%s", value, USAGEMESSAGE);
                        if ( *elements != NULL )
                        {
                            free(*elements);
                        }
                        return 1;
                    }
                    *verbosity |= value; // store verbosity in bit 0
                }
                openflag = 0; // reset flag
            }

            /* otherwise, we are dealing with the (number of) elements, use integers */
            else
            {
                /* compute the value */
                long value = 0;
                int j = 0;
                while (argv[i][j] != '\0')
                {
                    value = value * 10;
                    int digit = argv[i][j] - '0';
                    if ( digit < 0 || digit > 9 )
                    {
                        printf("Invalid character '%c' in argument %d. Expected a digit\n%s", argv[i][j], i, USAGEMESSAGE);
                        if ( *elements != NULL )
                        {
                            free(*elements);
                        }
                        return 1;
                    }
                    value += digit;
                    j++;
                }

                if ( *target == -1 )
                {
                    *target = value;
                }
                else if ( *n == -1 )
                {
                    *n = value;
                    *elements = malloc((*n) * sizeof(long));
                    if ( *elements == NULL )
                    {
                        printf("Memory allocation failed for elements array\n");
                        return 1;
                    }
                }
                else
                {
                    if ( nelements >= *n )
                    {
                        printf("Too many elements provided. Expected %d\n%s", *n, USAGEMESSAGE);
                        free(*elements);
                        return 1;
                    }
                    (*elements)[nelements] = value;
                    nelements++;
                }
            }
        }
    }

    if ( *target == -1 )
    {
        printf("No target provided\n%s", USAGEMESSAGE);
        if ( *elements != NULL )
        {
            free(*elements);
        }
        return 1;
    }

    if ( *n == -1 )
    {
        printf("No elements provided\n%s", USAGEMESSAGE);
        if ( *elements != NULL )
        {
            free(*elements);
        }
        return 1;
    }

    return 0;
}



/** main function */
int main(
    int argc,
    const char** argv
    )
{
    long target;
    int n;
    long* elts;
    __uint8_t settings;
    __uint8_t verbosity;

    int res = inputreader(argc, argv, &target, &n, &elts, &settings, &verbosity);
    if ( res != 0 )
    {
        return res;
    }

    printf("Number of elements: %d\n", n);
    printf("Elements: ");
    for (int i = 0; i < n; ++i)
    {
        printf("%ld ", elts[i]);
    }
    printf("\n");
    printf("Settings: %u\n", settings);
    printf("Verbosity: %u\n", verbosity);

    // call function
    // todo

    free(elts);
    return 0;
}