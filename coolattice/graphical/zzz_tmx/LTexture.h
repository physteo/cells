#pragma once

#include <iostream>
#include <MYGLEW/glew.h>
#include <FreeImage/FreeImage.h>
#include "./utils/ImageLoader.h"




namespace tmx { namespace graphics {

	enum ColorCode
	{
		RGB = 0,
		RGBA,
		BGR,
		BGRA,
		ABGR
	};

	class LTexture
	{
	private:
		//Texture ID
		GLuint mTextureID;

		//Texture dimensions
		GLsizei mTextureWidth;
		GLsizei mTextureHeight;
	public:
		LTexture();
		//Frees texture
		~LTexture();
		
		LTexture(const std::string& path, ColorCode image_format, ColorCode internal_format, GLint level, GLint border);

		// Explicitly deleting copy/assigment constructors
		// why? because I dont want multiple objects to point to the same location on the GPU
		// and the destructor gets called upon passing LTexture by value.. which is terrible
		LTexture(const LTexture&) = delete;
		LTexture& operator=(const LTexture&) = delete;

		void loadTextureFromFile(std::string filename, ColorCode image_format, ColorCode internal_format, GLint level, GLint border);

		GLuint getTextureID() const;
		GLuint textureWidth() const;
		GLuint textureHeight() const;

		void bind() const;
		void unbind() const;

	private:
		void freeTexture();
	};


}}