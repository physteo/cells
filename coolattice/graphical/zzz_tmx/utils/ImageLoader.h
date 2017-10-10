#pragma once

#include <FreeImage/FreeImage.h>
#include <iostream>

namespace tmx { namespace utils {

	static BYTE* load_image_from_file(const char* filename, GLsizei* width, GLsizei* height)
	{

		BYTE* bits = nullptr;
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP* dib = nullptr;
		FIBITMAP* dib32 = nullptr;

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			throw std::runtime_error("Texture Error: File format unknown.");

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib)
			throw std::runtime_error("Texture Error: Failed to load image.");

		//retrieve the image data
		bits = FreeImage_GetBits(dib);

		//get the image width and height
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);

		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits == 0) || (*width == 0) || (*height == 0))
			throw std::runtime_error("Texture Error: Unknown error.");

		// TODO: look if this is needed! (sparky episode 15, at around minute 17)
		// FreeImage_Unload(dib);

		return bits;
	}

} }