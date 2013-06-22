// This code is "Public Domain", no rights reserved

#ifndef TGA_SAVER_H
#define TGA_SAVER_H

// Writes a TGA image file with the given frame data.
//
// data_ptr should point to a resolution_x * resolution_y buffer of 24bit
// values in BGR format (byte per channel)

void Write_Tga(
	const char	*target_filename,
	int			resolution_x,
	int			resolution_y,
	void 		*data_ptr);

#endif
