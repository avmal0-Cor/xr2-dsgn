/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#include <iostream>
#include <fstream>
#include <string>
#include <gcm_tool.h>
#include "context.h"
#include "template.h"

using namespace std;
using namespace cell::Gcm;

// setup context
static void setupContext(CellGcmContextData &context, uint32_t *address, uint32_t size, CellGcmContextCallback callback)
{
	context.begin = address;
	context.current = address;
	context.end = address + size/sizeof(uint32_t);
	context.callback = callback;
}


// constructor
CommandContext::CommandContext()
{
	size = 0;
	buffer = NULL;
}

// destructor
CommandContext::~CommandContext()
{
	if(buffer != NULL){
		delete buffer;
	}
}


// open outfile
void CommandContext::open(string str)
{
	filename = str;

	outfile.open(filename.c_str(), ios::out | ios::binary);
	if(!outfile){
		perror(filename.c_str());
	}
}

// create command
void CommandContext::create()
{
	// use measure to get size
	setupContext(measure, NULL, 0, NULL);
	createCommand(measure);

	// get size as byte
	size = (uint32_t)measure.current;

	// allocate buffer
	buffer = new uint32_t[size];

	// use unsafe to create commands
	setupContext(unsafe, buffer, size, NULL);
	createCommand(unsafe);
}

// write outfile
void CommandContext::write()
{
	outfile.write((char *)buffer, size);
}
