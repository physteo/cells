#pragma once

#include <MYGLEW/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "StringUtils.h"

namespace tmx { namespace utils {

	static void load_OBJ(const std::string& path,
						 std::vector<std::vector<GLfloat> >& attributes,
						 std::vector<GLint>& components,
						 std::vector<GLuint>& indices)
	{
		
		std::vector<std::vector<std::string> > lines = load_text_file(path);
		//std::string text = read_file(path.c_str());
		//std::vector<std::string> lines = splitString(text, "\n");


		std::vector<GLfloat> positions;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;

		std::vector<GLfloat> sortedTextures;
		std::vector<GLfloat> sortedNormals;




		size_t numberOfVertices = 0;
		std::string delimiter = "/";

		for (size_t l = 0; l < lines.size(); l++)
		{

			auto& line = lines.at(l);

			if (line.at(0) == "v")
			{
				// vertex position
				positions.push_back(std::stof(line.at(1)));
				positions.push_back(std::stof(line.at(2)));
				positions.push_back(std::stof(line.at(3)));
				numberOfVertices++;
			}
			else if (line.at(0) == "vt")
			{
				// vertex texture
				textures.push_back(glm::vec2{ std::stof(line.at(1)),
					std::stof(line.at(2)) });
			}
			else if (line.at(0) == "vn")
			{
				// vertex normal
				normals.push_back(glm::vec3{ std::stof(line.at(1)),
					std::stof(line.at(2)),
					std::stof(line.at(3)) });
			}
			else if (line.at(0) == "f")
			{
				// this line contains a face: three triplets (i.e. a/b/c), each one is data for one vertex
				//numberOfVertices = vertices.size();
				sortedNormals.resize(numberOfVertices * 3);
				sortedTextures.resize(numberOfVertices * 2);
				break;
			}
		}


		// now process only the f lines
		for (size_t l = 0; l < lines.size(); l++)
		{
			auto& line = lines.at(l);
			if (line.at(0) != "f") {
				continue;
			}
			else if (line.at(0) == "f") {

				std::vector<glm::vec3> face;
				for (size_t w = 1; w < line.size(); w++)
				{
					auto word = line.at(w);
					std::vector<std::string> splittedWord = splitString(word, "/");
					face.push_back(glm::vec3{ std::stoi(splittedWord.at(0)),
						std::stoi(splittedWord.at(1)),
						std::stoi(splittedWord.at(2)) });
				}

				// now face vector contains three vec3s, each for one vertex of the face
				// each vec3 contains three information: index of the vertex, index of the texture, index of the normal
				//std::cout << face.at(0).x << "/" << face.at(0).y << "/" << face.at(0).z << "  ";
				//std::cout << face.at(1).x << "/" << face.at(1).y << "/" << face.at(1).z << "  ";
				//std::cout << face.at(2).x << "/" << face.at(2).y << "/" << face.at(2).z << std::endl;


				for (size_t faceVertex = 0; faceVertex < face.size(); faceVertex++)
				{
					glm::vec3 vertexData = face.at(faceVertex);

					GLint currentVertexIndex = vertexData.x - 1;
					indices.push_back(currentVertexIndex);

					glm::vec2 currentTex = textures.at(vertexData.y - 1);
					sortedTextures.at(currentVertexIndex * 2 + 0) = currentTex.x;
					sortedTextures.at(currentVertexIndex * 2 + 1) = currentTex.y;

					glm::vec3 currentNormal = normals.at(vertexData.z - 1);
					sortedNormals.at(currentVertexIndex * 3 + 0) = currentNormal.x;
					sortedNormals.at(currentVertexIndex * 3 + 1) = currentNormal.y;
					sortedNormals.at(currentVertexIndex * 3 + 2) = currentNormal.z;
				}

			}
		}

		// now indices goes into the IBO
		// vertices goes to the VBO 0
		// textures to VBO 1
		// normals to VBO 2


		attributes = { positions, sortedTextures, sortedNormals };
		components = { 3, 2, 3 };
	}


	static void load_OBJ2(const std::string& path,
		std::vector<std::vector<GLfloat> >& attributes,
		std::vector<GLint>& components,
		std::vector<GLuint>& indices)
	{

		//std::vector<std::vector<std::string> > lines = load_text_file(path);
		std::string text = read_file(path.c_str());
		std::vector<std::string> stringLines = splitString(text, "\n");

		std::vector<std::vector<std::string> > lines;
		for (int i = 0; i < stringLines.size(); i++)
		{
			lines.push_back(splitString(stringLines.at(i), " "));
		}

		std::vector<GLfloat> positions;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;

		std::vector<GLfloat> sortedTextures;
		std::vector<GLfloat> sortedNormals;




		size_t numberOfVertices = 0;
		std::string delimiter = "/";

		for (size_t l = 0; l < lines.size(); l++)
		{

			auto& line = lines.at(l);

			if (line.at(0) == "v")
			{
				// vertex position
				positions.push_back(std::stof(line.at(1)));
				positions.push_back(std::stof(line.at(2)));
				positions.push_back(std::stof(line.at(3)));
				numberOfVertices++;
			}
			else if (line.at(0) == "vt")
			{
				// vertex texture
				textures.push_back(glm::vec2{ std::stof(line.at(1)),
					std::stof(line.at(2)) });
			}
			else if (line.at(0) == "vn")
			{
				// vertex normal
				normals.push_back(glm::vec3{ std::stof(line.at(1)),
					std::stof(line.at(2)),
					std::stof(line.at(3)) });
			}
			else if (line.at(0) == "f")
			{
				// this line contains a face: three triplets (i.e. a/b/c), each one is data for one vertex
				//numberOfVertices = vertices.size();
				sortedNormals.resize(numberOfVertices * 3);
				sortedTextures.resize(numberOfVertices * 2);
				break;
			}
		}


		// now process only the f lines
		for (size_t l = 0; l < lines.size(); l++)
		{
			auto& line = lines.at(l);
			if (line.at(0) != "f") {
				continue;
			}
			else if (line.at(0) == "f") {

				std::vector<glm::vec3> face;
				for (size_t w = 1; w < line.size(); w++)
				{
					auto word = line.at(w);
					std::vector<std::string> splittedWord = splitString(word, "/");
					face.push_back(glm::vec3{ std::stoi(splittedWord.at(0)),
						std::stoi(splittedWord.at(1)),
						std::stoi(splittedWord.at(2)) });
				}

				// now face vector contains three vec3s, each for one vertex of the face
				// each vec3 contains three information: index of the vertex, index of the texture, index of the normal
				//std::cout << face.at(0).x << "/" << face.at(0).y << "/" << face.at(0).z << "  ";
				//std::cout << face.at(1).x << "/" << face.at(1).y << "/" << face.at(1).z << "  ";
				//std::cout << face.at(2).x << "/" << face.at(2).y << "/" << face.at(2).z << std::endl;


				for (size_t faceVertex = 0; faceVertex < face.size(); faceVertex++)
				{
					glm::vec3 vertexData = face.at(faceVertex);

					GLint currentVertexIndex = vertexData.x - 1;
					indices.push_back(currentVertexIndex);

					glm::vec2 currentTex = textures.at(vertexData.y - 1);
					sortedTextures.at(currentVertexIndex * 2 + 0) = currentTex.x;
					sortedTextures.at(currentVertexIndex * 2 + 1) = currentTex.y;

					glm::vec3 currentNormal = normals.at(vertexData.z - 1);
					sortedNormals.at(currentVertexIndex * 3 + 0) = currentNormal.x;
					sortedNormals.at(currentVertexIndex * 3 + 1) = currentNormal.y;
					sortedNormals.at(currentVertexIndex * 3 + 2) = currentNormal.z;
				}

			}
		}

		// now indices goes into the IBO
		// vertices goes to the VBO 0
		// textures to VBO 1
		// normals to VBO 2


		attributes = { positions, sortedTextures, sortedNormals };
		components = { 3, 2, 3 };
	}






} }