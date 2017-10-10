#include "LTexture.h"

namespace tmx { namespace graphics {

	static void convert_ABGR_to_RGBA(BYTE* pixels, GLsizei&& size);


	LTexture::LTexture()
	{
		//Initialize texture ID
		mTextureID = 0;
		mTextureWidth = 0;
		mTextureHeight = 0;
	}
	
	
	LTexture::~LTexture()
	{
		std::cout << "Texture " << this << " being deleted..." << std::endl;
		freeTexture();
	}


	LTexture::LTexture(const std::string& path, ColorCode image_format, ColorCode internal_format, GLint level, GLint border) : LTexture()
	{
		loadTextureFromFile(path, image_format, internal_format, level, border);
	}


	//! level: for now put to zero (it has to do with 3d textures..?
	//! image_format and internal_format: set to GL_RGB for the moment
	void LTexture::loadTextureFromFile(std::string filename, ColorCode image_format, ColorCode internal_format, GLint level, GLint border)
	{
		freeTexture();


		BYTE* pixels = nullptr;
		pixels = tmx::utils::load_image_from_file(filename.c_str(), &mTextureWidth, &mTextureHeight);

		GLenum glImageFormat;
		GLenum glInternalFormat;

		if (image_format == RGB)
		{
			glImageFormat = GL_RGB;
		}
		else if (image_format == RGBA)
		{
			glImageFormat = GL_RGBA;
		}
		else if (image_format == BGR)
		{
			glImageFormat = GL_BGR;
		}
		else if (image_format == BGRA)
		{
			glImageFormat = GL_BGRA;
		}
		else if (image_format == ABGR)
		{
			glImageFormat = GL_RGBA;
			convert_ABGR_to_RGBA(pixels, mTextureWidth * mTextureHeight);
		}


		if (internal_format == RGB)
		{
			glInternalFormat = GL_RGB;
		}
		else if (internal_format == RGBA)
		{
			glInternalFormat = GL_RGBA;
		}
		else if (internal_format == BGR)
		{
			glInternalFormat = GL_BGR;
		}
		else if (internal_format == BGRA)
		{
			glInternalFormat = GL_BGRA;
		}
		else if (internal_format == ABGR)
		{
			throw std::runtime_error("Internal format ABGR is not supported for textures.");
		}
		


		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &mTextureID);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		// setting some parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//store the texture data for OpenGL use
		glTexImage2D(GL_TEXTURE_2D, level, glInternalFormat, mTextureWidth, mTextureHeight,
			border, glImageFormat, GL_UNSIGNED_BYTE, (GLuint*) pixels);

		// generating mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.f);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			throw std::runtime_error("Error loading texture.");
			//printf("Error loading texture from %p pixels! %s\n", pixels, error);
			//std::cout << "Error loading Texture: " << error << std::endl;
		}

		// TODO: figure out if I need to do this deletion
		// delete pixels[]
	}

	void LTexture::freeTexture()
	{

		//Freeing texture
		if (mTextureID != 0)
		{
			std::cout << "...freeing Texture " << this << std::endl;
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}

		mTextureWidth = 0;
		mTextureHeight = 0;
	}

	GLuint LTexture::getTextureID() const
	{
		return mTextureID;
	}

	GLuint LTexture::textureWidth() const
	{
		return mTextureWidth;
	}

	GLuint LTexture::textureHeight() const
	{
		return mTextureHeight;
	}


	void LTexture::bind() const
	{
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}

	void LTexture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}



	static void convert_ABGR_to_RGBA(BYTE* pixels, GLsizei&& size)
	{
		for (int p = 0; p < size; p++)
		{
			BYTE a = pixels[p * 4 + 0];
			BYTE b = pixels[p * 4 + 1];
			BYTE g = pixels[p * 4 + 2];
			BYTE r = pixels[p * 4 + 3];

			pixels[p * 4 + 0] = r;
			pixels[p * 4 + 1] = g; 
			pixels[p * 4 + 2] = b;
			pixels[p * 4 + 3] = a;
		}
	}


}}












// WITH DEVIL
//void LTexture::loadTextureFromFile2(std::string path)
//{
//	// Init DevIl
//	ilInit();
//
//	//Texture loading success
//	bool textureLoaded = false;
//
//	//Generate and set current image ID
//	ILuint imgID = 0;
//	ilGenImages(1, &imgID);
//	ilBindImage(imgID);
//	//Load image
//	ILboolean success = IL_FALSE;
//	try {
//		success = ilLoadImage((const ILstring)path.c_str());
//	}
//	catch (...) {
//		std::cout << ilGetError() << std::endl;
//		throw;
//	}
//
//
//	//Image loaded successfully
//	if (success == IL_TRUE)
//	{
//		//Convert image to RGBA
//		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//		if (success == IL_TRUE)
//		{
//			//Create texture from file pixels
//			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
//		}
//		else {
//			throw std::runtime_error("Unable to load image\n");
//		}
//		//Delete file from memory
//		ilDeleteImages(1, &imgID);
//	}
//	else
//	{
//		throw std::runtime_error("Unable to load image\n");
//	}
//	//Report error
//	if (!textureLoaded)
//	{
//		//printf("Unable to load %s\n", path.c_str());
//		throw std::runtime_error("Unable to load image\n");
//	}
//
//}
//
//bool LTexture::loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height)
//{
//	//Free texture if it exists
//	freeTexture();
//
//	//Get texture dimensions
//	mTextureWidth = width;
//	mTextureHeight = height;
//
//	//Generate texture ID
//	glGenTextures(1, &mTextureID);
//
//	//Bind texture ID
//	glBindTexture(GL_TEXTURE_2D, mTextureID);
//	//Generate texture
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//	//Set texture parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	//Unbind texture
//	glBindTexture(GL_TEXTURE_2D, NULL);
//	//Check for error
//	GLenum error = glGetError();
//	if (error != GL_NO_ERROR)
//	{
//		printf("Error loading texture from %p pixels! %s\n", pixels, error);
//		std::cout << "Error loading Texture: " << error << std::endl;
//		return false;
//	}
//
//
//	return true;
//}