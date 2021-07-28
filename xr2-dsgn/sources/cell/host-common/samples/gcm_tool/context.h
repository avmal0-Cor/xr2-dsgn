/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

//
// Command Context Class
//
class CommandContext {
	std::string		filename;	// output filename
	std::ofstream	outfile;	// output filestream

	uint32_t size;		// command byte size
	uint32_t *buffer;	// command buffer

	cell::Gcm::Unsafe::CellGcmContext	unsafe;		// Unsafe Context
	cell::Gcm::Measure::CellGcmContext	measure;	// Measure Context

public:
	CommandContext();
	~CommandContext();

	void open(std::string str);
	void create();
	void write();
};
