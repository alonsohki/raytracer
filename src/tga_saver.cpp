// This code is "Public Domain", no rights reserved.

#include "tga_saver.h"

#include <stdio.h>
#include <assert.h>

template<typename T>
static void Write(FILE *file, T value, unsigned int amount = 1)
{
	assert(amount >= 1);
	for (unsigned int i = 0; i < amount; ++i)
		fwrite(&value, sizeof(T), 1, file);
}

void Write_Tga(
	const char	*target_filename,
	int			resolution_x,
	int			resolution_y,
	void 		*data_ptr)
{
	FILE *file = fopen(target_filename, "wb");

	// Note: Error handling omitted for simplicity.
	assert(file);

	// TGA Format for example at: http://www.gamers.org/dEngine/quake3/TGA.txt

	// Header
	{
		// ID + Color map type (none)
		Write<char>(file, 0, 2);

		// RGB image type
		Write<char>(file, 2);

		// Dummy color map
		Write<char>(file, 0, 5);

		// Image specification
		Write<short>(file, 0, 2);
		Write<short>(file, resolution_x);
		Write<short>(file, resolution_y);
		Write<char>(file, 24); // Bits per pixel
		Write<char>(file, 0);
	}

	// Actual image data
	fwrite(data_ptr, sizeof(char), resolution_x * resolution_y * 3, file);

	fclose(file);
}
