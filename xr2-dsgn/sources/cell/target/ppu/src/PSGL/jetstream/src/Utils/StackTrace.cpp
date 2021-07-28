/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include "PSGL/StackTrace.h"
#include <stdio.h>
#include <string.h>

void _jsGetStackTrace( int depthStart, int depthCount, char *buffer, int size )
{
    // Register r1 contains the stack pointer
    // framepointer[1] will contain the back chain
    // framepoitner[5] will contain the link register
    // Note:  Even though pointers are 32bit,
    //        the stack frame seems to be 64bit.
    //
    int byteWritten = 0;
    short count = 0;
    void **framepointer;
    register void **fp asm( "r1" );
    framepointer = fp;
    count = 0;

    //needed for the update of byteWritten
    memset( buffer, 0, size );
    size--;//let's decrease size of 1 to keep a NULL char at the end of the string

    if ((( void ** )NULL != framepointer ) && ((( void ** )( -1U ) ) >= framepointer ) )
    {
        while ( count < ( depthStart + depthCount ) && ( size - byteWritten ) )
        {
            if ((( void * )NULL == framepointer[1] ) || (( void * )NULL == framepointer[5] )
                    || (( void * )NULL == (( void** )framepointer[1] )[1] ) || (( void * )NULL == (( void** )framepointer[1] )[5] ) )
            {
                break;
            }

            if ( count >= depthStart )
            {
                long address = ( long ) framepointer[5];
                snprintf( buffer + byteWritten, size - byteWritten, "0x%08x (level #%d)\n", ( unsigned int )address, count - depthStart );
                //update byteWritten
                byteWritten += strlen( buffer + byteWritten );
            }

            count++;

            // let's point up the chain
            framepointer = ( void ** )framepointer[1];
        }
    }
}
