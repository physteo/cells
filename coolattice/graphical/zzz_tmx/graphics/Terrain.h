#pragma once

#include <MYGLEW/glew.h>
#include "TexturedModel.h"
#include "Renderable.h"

namespace tmx {
	namespace graphics {

		class Terrain : public Renderable
		{
			const GLfloat SIZE = 100;
			const GLuint QUADS = 100;
		public:
			//Terrain(const TexturedModel* in_texturedModel, const StaticShader* terrainShader)
			//	: Renderable(in_texturedModel, terrainShader, glm::vec3{ 0,1,0 }, glm::vec3{0,0.0001,0}, 0.0f, 1.0f) {}


			Terrain(int gridX, int gridZ, const std::vector<std::string>& texturesPaths, const ShaderProgram* terrainShader, ColorCode image_format,
				ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity)
			{
				position.x = gridX * SIZE;
				position.y = 0;
				position.z = gridZ * SIZE;
				texturedModel = generateTerrain(texturesPaths, image_format,
					internal_format, level, border, shineDamper, reflectivity);
				shader = terrainShader;
				rotation.x = 0.f;
				rotation.y = 1.0f;
				rotation.z = 0.f;
				angle = 0.0f;
				scale = 1.0;
			}

		private:
			MTexturedModel* generateTerrain(const std::vector<std::string>& texturesPaths, ColorCode image_format,
				ColorCode internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity)
			{
				////GLfloat width = 1.0;
				////GLfloat height = 1.0;

				//TexturedModel* temp = new TexturedModel( width,  height, texturePath,
				//	 image_format,  internal_format,  level,  border,  shineDamper,  reflectivity);

				////std::vector<GLfloat> vertices = { -0.5f * width, 0.5f * height, 0.0f,
				////	-0.5f * width,-0.5f * height, 0.0f,
				////	0.5f * width,-0.5f * height, 0.0f,
				////	0.5f * width, 0.5f * height, 0.0f, };
				////
				////std::vector<GLfloat> normals		 = { 0,0,1, 0,0,1, 0,0,1, 0,0,1};
				////std::vector<GLfloat> textureCoords	 = { 0,0, 0,1, 1,1, 1,0 };
				////std::vector<GLuint>  indices		 = { 0, 1, 2, 2, 3, 0 };


				
				std::vector<GLuint>  indices;
				indices.reserve( 6 * QUADS * QUADS );

				for (GLuint j = 0; j < QUADS; j++)
				{
					for (GLuint i = 0; i < QUADS; i++)
					{
						indices.push_back(QUADS + 1 + j*(QUADS + 1) + i);
						indices.push_back(i + j*(QUADS + 1));
						indices.push_back(i + j*(QUADS + 1) + 1);
						indices.push_back(i + j*(QUADS + 1) + 1);
						indices.push_back(QUADS + 1 + i + j*(QUADS + 1) + 1);
						indices.push_back(QUADS + 1 + i + j*(QUADS + 1));
					}
				}
				std::vector<GLfloat> vertices((QUADS + 1) * (QUADS + 1) * 3);
				std::vector<GLfloat> normals((QUADS + 1) * (QUADS + 1) * 3);
				std::vector<GLfloat> textureCoords((QUADS + 1) * (QUADS + 1) * 2);

				GLuint vertex = 0;
				for (GLuint j = 0; j < QUADS+1; j++)
				{
					for (GLuint i = 0; i < QUADS+1; i++)
					{
						vertices.at(vertex * 3 + 0) = SIZE * (float)j /((float)QUADS);
						vertices.at(vertex * 3 + 1) = 0;
						vertices.at(vertex * 3 + 2) = SIZE * (float)i / ((float)QUADS);

						normals.at(vertex * 3 + 0) = 0;
						normals.at(vertex * 3 + 1) = 1;
						normals.at(vertex * 3 + 2) = 0;

						textureCoords.at(vertex * 2 + 0) = 0.1*(float)j;// / ((float)QUADS);
						textureCoords.at(vertex * 2 + 1) = 0.1*(float)i;// / ((float)QUADS);
						vertex++;
					}
				}


				//int count = VERTEX_COUNT * VERTEX_COUNT;
				//std::vector<GLfloat> vertices(count * 3);
				//std::vector<GLfloat> normals(count * 3);
				//std::vector<GLfloat> textureCoords(count * 2);
				//std::vector<GLuint> indices(6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1));
				
				//int vertexPointer = 0;
				//for (int i = 0; i < VERTEX_COUNT; i++) {
				//	for (int j = 0; j < VERTEX_COUNT; j++) {
				//		vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
				//		vertices[vertexPointer * 3 + 1] = 0;
				//		vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
				//		normals[vertexPointer * 3] = 0;
				//		normals[vertexPointer * 3 + 1] = 1;
				//		normals[vertexPointer * 3 + 2] = 0;
				//		textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				//		textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
				//		vertexPointer++;
				//	}
				//}
				//int pointer = 0;
				//for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
				//	for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
				//		int topLeft = (gz*VERTEX_COUNT) + gx;
				//		int topRight = topLeft + 1;
				//		int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
				//		int bottomRight = bottomLeft + 1;
				//		indices[pointer++] = topLeft;
				//		indices[pointer++] = bottomLeft;
				//		indices[pointer++] = topRight;
				//		indices[pointer++] = topRight;
				//		indices[pointer++] = bottomLeft;
				//		indices[pointer++] = bottomRight;
				//	}
				//}
				

				std::vector<std::vector<GLfloat> > attributes = { vertices, textureCoords, normals };
				std::vector<GLint> components = { 3, 2, 3 };
				
				MTexturedModel* temp = new MTexturedModel(indices, attributes,
					components, texturesPaths, image_format, internal_format,
					level, border, shineDamper, reflectivity);


				return temp;
			}


		};
//		class Terrain : public Renderable
//		{
//
//		private:
//			const GLfloat SIZE = 800;
//			const GLfloat VERTEX_COUNT = 128;
//
//		public:
//			Terrain(int gridX, int gridZ, const std::string& texturePath, const StaticShader* terrainShader, GLenum image_format,
//				GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity) : Renderable()
//			{
//				position.x = gridX * 1;//SIZE;
//				position.z = gridZ * 1;//SIZE;
//				texturedModel = generateTerrain(texturePath, image_format,
//					internal_format, level, border, shineDamper, reflectivity);
//				shader = terrainShader;
//			}
//			~Terrain()
//			{
//				delete texturedModel;
//			}
//		private:
//			TexturedModel* generateTerrain(const std::string& texturePath, GLenum image_format,
//				GLint internal_format, GLint level, GLint border, GLfloat shineDamper, GLfloat reflectivity)
//			{
//				//GLfloat width = 1.0;
//				//GLfloat height = 1.0;
//				//std::vector<GLfloat> vertices = { -0.5f * width, 0.5f * height, 0.0f,
//				//	-0.5f * width,-0.5f * height, 0.0f,
//				//	0.5f * width,-0.5f * height, 0.0f,
//				//	0.5f * width, 0.5f * height, 0.0f, };
//				//
//				//std::vector<GLfloat> normals = {0,0,1, 0,0,1, 0,0,1, 0,0,1};
//				//std::vector<GLfloat> textureCoords = { 0,0, 0,1, 1,1, 1,0 };
//				//std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0 };
//
//				/*
//				int count = VERTEX_COUNT * VERTEX_COUNT;
//				std::vector<GLfloat> vertices(count * 3);
//				std::vector<GLfloat> normals(count * 3);
//				std::vector<GLfloat> textureCoords(count * 2);
//				std::vector<GLuint> indices(6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1));
//				
//				int vertexPointer = 0;
//				for (int i = 0; i < VERTEX_COUNT; i++) {
//					for (int j = 0; j < VERTEX_COUNT; j++) {
//						vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
//						vertices[vertexPointer * 3 + 1] = 0;
//						vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
//						normals[vertexPointer * 3] = 0;
//						normals[vertexPointer * 3 + 1] = 1;
//						normals[vertexPointer * 3 + 2] = 0;
//						textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
//						textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
//						vertexPointer++;
//					}
//				}
//				int pointer = 0;
//				for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
//					for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
//						int topLeft = (gz*VERTEX_COUNT) + gx;
//						int topRight = topLeft + 1;
//						int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
//						int bottomRight = bottomLeft + 1;
//						indices[pointer++] = topLeft;
//						indices[pointer++] = bottomLeft;
//						indices[pointer++] = topRight;
//						indices[pointer++] = topRight;
//						indices[pointer++] = bottomLeft;
//						indices[pointer++] = bottomRight;
//					}
//				}
//				*/
//
//				//std::vector<std::vector<GLfloat> > attributes = { vertices, normals, textureCoords };
//				//std::vector<GLint> components = { 3, 3, 2 };
//				//
//				//TexturedModel* temp = new TexturedModel(indices, attributes,
//				//	components, texturePath, image_format, internal_format,
//				//	level, border, shineDamper, reflectivity);
//
//
//
//				TexturedModel* temp = new TexturedModel(1, 1, texturePath,
//					image_format, internal_format, level, border, shineDamper, reflectivity);
//
//				return temp;
//			}
//
//
//		};

	

} }