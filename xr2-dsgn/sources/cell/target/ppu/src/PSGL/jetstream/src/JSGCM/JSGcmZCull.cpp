/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * Functions for modifying Z Cull related state
 */

#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmResource.h>

// Write the stats to the current report slots, and then reset the stats.
static void _jsGcmGetZCullStats()
{

    // Disable Zcull stat
    GCM_FUNC( cellGcmSetZcullStatsEnable, CELL_GCM_FALSE );

	if ( _jsGcmState.reportsInMainMemory )
	{	
		// According to gcm document, report values have to be written to
		// local memory location specified by unique id.
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS, JSGCM_MM_ZCULL_STAT0 );  // Just a random unique id for now.
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS1, JSGCM_MM_ZCULL_STAT1 ); 
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS2, JSGCM_MM_ZCULL_STAT2 );
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS3, JSGCM_MM_ZCULL_STAT3 );

		// Need to store the zcull values somewhere in jsGcmSTate 
		// Then, values can be read
		_jsGcmState.zCull[0] = cellGcmGetReportDataLocation( JSGCM_MM_ZCULL_STAT0, CELL_GCM_LOCATION_MAIN );
		_jsGcmState.zCull[1] = cellGcmGetReportDataLocation( JSGCM_MM_ZCULL_STAT1, CELL_GCM_LOCATION_MAIN );
		_jsGcmState.zCull[2] = cellGcmGetReportDataLocation( JSGCM_MM_ZCULL_STAT2, CELL_GCM_LOCATION_MAIN );
		_jsGcmState.zCull[3] = cellGcmGetReportDataLocation( JSGCM_MM_ZCULL_STAT3, CELL_GCM_LOCATION_MAIN );
	}
	else
	{
		// According to gcm document, report values have to be written to
		// local memory location specified by unique id.
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS, JSGCM_LM_ZCULL_STAT0 );  // Just a random unique id for now.
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS1, JSGCM_LM_ZCULL_STAT1 ); 
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS2, JSGCM_LM_ZCULL_STAT2 );
		GCM_FUNC( cellGcmSetReport, CELL_GCM_ZCULL_STATS3, JSGCM_LM_ZCULL_STAT3 );

		// Need to store the zcull values somewhere in jsGcmSTate 
		// Then, values can be read
		_jsGcmState.zCull[0] = cellGcmGetReport( CELL_GCM_ZCULL_STATS, JSGCM_LM_ZCULL_STAT0 );
		_jsGcmState.zCull[1] = cellGcmGetReport( CELL_GCM_ZCULL_STATS1, JSGCM_LM_ZCULL_STAT1 );
		_jsGcmState.zCull[2] = cellGcmGetReport( CELL_GCM_ZCULL_STATS2, JSGCM_LM_ZCULL_STAT2 );
		_jsGcmState.zCull[3] = cellGcmGetReport( CELL_GCM_ZCULL_STATS3, JSGCM_LM_ZCULL_STAT3 );
	}

    // Clear stats
    GCM_FUNC( cellGcmSetClearReport, CELL_GCM_ZCULL_STATS );

    // Enable stats again
    GCM_FUNC( cellGcmSetZcullStatsEnable, CELL_GCM_TRUE );


	if ( _jsGcmState.reportsInMainMemory )
	{
		// NOTE setting main memory report zero to non zero value so we can check it later 
		// for completion 
		if (_jsGcmState.reportAddressMain)
		{
			_jsGcmState.reportAddressMain[JSGCM_MM_ZCULL_STAT0].zero	= 0xFFFFFFFF;
			_jsGcmState.reportAddressMain[JSGCM_MM_ZCULL_STAT0].value	= 0xFFFFFFFF; 
		}
	}
	else
	{
		// make sure our completion values is what we expect 
		_jsGcmState.reportCompletionValue = JSGCM_REPORT_COMPLETE_VALUE; 

		// set the inline trasfer to put our completion value ( JSGCM_REPORT_COMPLETE_VALUE ) 
		// to happen after the above calls have completed.  We will use this to sync on later. 
		GCM_FUNC( cellGcmInlineTransfer, _jsGcmState.reportCompletionOffset + (JSGCM_LM_ZCULL_STAT0 * 4), 
				  &_jsGcmState.reportCompletionValue, 1, CELL_GCM_LOCATION_MAIN);

		//printf(" Done jsGcmGetZCullStats \n" ); fflush(stdout ); 
	}
}

void _jsGcmDisableClearZCullStats( int disable )
{
    // when running a zcull experiment, we cannot clear zcull stats
    _jsGcmState.state.zcull.disableClearStats = disable;
}


/* zcull mojo
**
** perf note:
**  - this should be called once per frame
**  - this is no exact science (...)
*/
void _jsGcmRequestAndUpdateZCullFeedback( void )
{
    jsGcmZCullState *zcull = &_jsGcmState.state.zcull;
    GLuint nextPushBack;
    GLuint nextMoveForward;

    if ( !zcull->disableClearStats ) 
	{
		_jsGcmGetZCullStats();

		// update the fifo put 
		_jsGcmFifoFlush( &_jsGcmState.fifo );
	}

	if ( _jsGcmState.reportsInMainMemory )
	{

		if ( *(volatile GLuint *)&_jsGcmState.reportAddressMain[JSGCM_MM_ZCULL_STAT0].zero == 0 )
		{
			// set move forward and push back 
			GLuint maxSlope;
			GLuint numSlopeSamples;
			GLuint slopeSum;
			GLuint numTilesConsidered;
			GLuint numTilesCulled;
			GLuint avgSlope;
			GLfloat ratioTilesCulled;
			
			// dynamic nextMoveForward/nextStep
			maxSlope = (( _jsGcmState.zCull[0] ) >> 16 ) & 0xffff;
			numSlopeSamples = ( _jsGcmState.zCull[0] ) & 0xffff;
			slopeSum = _jsGcmState.zCull[1];
			numTilesConsidered = _jsGcmState.zCull[2];
			numTilesCulled = _jsGcmState.zCull[3];
			avgSlope = numSlopeSamples ? slopeSum / numSlopeSamples : 0;
			ratioTilesCulled = ( GLfloat )numTilesCulled / ( GLfloat )numTilesConsidered;
			
			// the easy version
			nextMoveForward = ( avgSlope + maxSlope ) / 2;
			
			if ( nextMoveForward < 1 )
			{
			   nextMoveForward = 1;
			}
			nextPushBack = nextMoveForward / 2;
			if ( nextPushBack < 1 )
			{
			   nextPushBack = 1;
			}
			
			// set this report no longer valid by putting in none zero in the zero and value area of the report B
			_jsGcmState.reportAddressMain[JSGCM_MM_ZCULL_STAT0].zero = 0xFFFFFFFF;
			_jsGcmState.reportAddressMain[JSGCM_MM_ZCULL_STAT0].value	= 0xFFFFFFFF; 

			zcull->hwControl1 = ( nextPushBack & 0xffff ) | (( nextMoveForward & 0xffff ) << 16 );

		}
	}
	else
	{
		// Check to see if our reportCompletionValue is there (JSGCM_REPORT_COMPLETE_VALUE)
		// if so our z
		if( *(volatile GLuint *)&_jsGcmState.reportCompletionValues[JSGCM_LM_ZCULL_STAT0] == 
			_jsGcmState.reportCompletionValue )
		{
			// set move forward and push back 
			GLuint maxSlope;
			GLuint numSlopeSamples;
			GLuint slopeSum;
			GLuint numTilesConsidered;
			GLuint numTilesCulled;
			GLuint avgSlope;
			GLfloat ratioTilesCulled;
			
			// dynamic nextMoveForward/nextStep
			maxSlope = (( _jsGcmState.zCull[0] ) >> 16 ) & 0xffff;
			numSlopeSamples = ( _jsGcmState.zCull[0] ) & 0xffff;
			slopeSum = _jsGcmState.zCull[1];
			numTilesConsidered = _jsGcmState.zCull[2];
			numTilesCulled = _jsGcmState.zCull[3];
			avgSlope = numSlopeSamples ? slopeSum / numSlopeSamples : 0;
			ratioTilesCulled = ( GLfloat )numTilesCulled / ( GLfloat )numTilesConsidered;
			
			// the easy version
			nextMoveForward = ( avgSlope + maxSlope ) / 2;
			
			if ( nextMoveForward < 1 )
			{
			   nextMoveForward = 1;
			}
			nextPushBack = nextMoveForward / 2;
			if ( nextPushBack < 1 )
			{
			   nextPushBack = 1;
			}
			
			// no more current data
			_jsGcmState.reportCompletionValues[JSGCM_LM_ZCULL_STAT0] = 0;

			zcull->hwControl1 = ( nextPushBack & 0xffff ) | (( nextMoveForward & 0xffff ) << 16 );
		}
	}
}

/* zcull mojo
**
** perf note:
**  - should be called after every depth buffer change because mf/pb are
**    per zcregion/depth buffer
*/
void _jsGcmUpdateZCullFeedbackParams( void )
{
    jsGcmZCullState *zcull = &_jsGcmState.state.zcull;

    GCM_FUNC( cellGcmSetZcullLimit, ( zcull->hwControl1 >> 16 ) & 0xffff, zcull->hwControl1 & 0xffff );
}

/* zcull mojo
**
** perf note:
**  - this call invalidates zcull until the next none-quad stencil/depth clear
**  - should be called before every depth clear
*/
void _jsGcmUpdateZCullDepthParams( void )
{
    jsGcmZCullState *zcull = &_jsGcmState.state.zcull;

    GCM_FUNC( cellGcmSetZcullControl,
              ( zcull->hwControl0 & 0x1 ) ? CELL_GCM_ZCULL_GREATER : CELL_GCM_ZCULL_LESS,
              (( zcull->hwControl0 >> 4 ) & 0x1 ) ? CELL_GCM_ZCULL_LONES : CELL_GCM_ZCULL_MSB );
}

/* more zcull mojo
**
** perf note:
**  - everything beside SSV will suffer right now.
**  - this call invalidates zcull until the next none-quad stencil/depth clear
**  - be careful not to invalidate zcull!
*/
void _jsGcmUpdateZCullStencilParams( void )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;

    GCM_FUNC( cellGcmSetScullControl, s->hwStencilCullHint & 0xFFFF, ( s->hwStencilCullHint >> 16 ) & 0xFF, ( s->hwStencilCullHint >> 24 ) & 0xFF );
}

/* more zcull mojo
**
** perf note:
**  - sets zcull's stencil cull hint that is loaded on the next depths/stencil clear
**  - if a app plans to to use a specific stencil test later on, it is recommended to
**    to clear the depth/stencil buffer with the proper stencil func already set!
*/
void _jsGcmSetStencilCullHint( jsGcmEnum func, GLint ref, GLuint mask )
{
    jsGcmStencilState *s = &_jsGcmState.state.stencil;
    GLuint hwStencilCullHint;

    hwStencilCullHint = ( ref << 16 ) // Ref goes into bit 16
                        | ( mask << 24 ); // mask goes into bit 24

    // Now for the func
    switch ( func )
    {
        case JSGCM_NEVER:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_NEVER;
            break;
        case JSGCM_LESS:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_LESS;
            break;
        case JSGCM_EQUAL:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_EQUAL;
            break;
        case JSGCM_LEQUAL:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_LEQUAL;
            break;
        case JSGCM_GREATER:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_GREATER;
            break;
        case JSGCM_NOTEQUAL:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_NOTEQUAL;
            break;
        case JSGCM_GEQUAL:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_GEQUAL;
            break;
        case JSGCM_ALWAYS:
            hwStencilCullHint |= CELL_GCM_SCULL_SFUNC_ALWAYS;
            break;
        default:
            JS_ASSERT( 0 ); // chip exception ahead!
            break;
    }
    JS_ASSERT(( ref >= 0 ) && ( ref <= 0xff ) );
    s->hwStencilCullHint = hwStencilCullHint;
}
