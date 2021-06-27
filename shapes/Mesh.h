#pragma once
#include"../utils/MVP_mat.h"
#include"shape.h"
#include"../renderers/Rasterizer.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
};

class Mesh : public Shape
{
public:
	// mesh Data
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

	// model data 
	std::string directory;

	// constructor, expects a filepath to a 3D model.
	Mesh(Rasterizer& r, std::string const& path) :raster(r)
	{
		loadMesh(path);
	}

	void drawChild(const MVP_mat& parent_trans)override
	{
		MVP_mat thisTrans(parent_trans);
		thisTrans.model = parent_trans.model * position * rotation * scaling;

		for (int i = 0; indices.size() != 0 && i <= indices.size() - 3; i += 3)
		{
			raster.process_trngl(thisTrans, vertices[indices[i]].pos, vertices[indices[i + 1]].pos, vertices[indices[i + 2]].pos);
		}

		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}

private:
	Rasterizer& raster;
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	Mesh(Rasterizer& r, std::vector<vertex> verts, std::vector<unsigned int> indes) :raster(r)
	{
		this->vertices = verts;
		this->indices = indes;
	}

	void loadMesh(std::string const& path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 0);// aiProcess_Triangulate| aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::string err = importer.GetErrorString();
			//gScreen.debug_massage("ERROR::ASSIMP:: "+ err);
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(const aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::shared_ptr<Shape> child_mesh = std::shared_ptr<Mesh>(processMesh(mesh, scene));
			this->addChild(child_mesh);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh* processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertex vrtx;

			vrtx.pos.x = mesh->mVertices[i].x;
			vrtx.pos.y = mesh->mVertices[i].y;
			vrtx.pos.z = mesh->mVertices[i].z;

			if (mesh->HasNormals())
			{
				vrtx.norm.x = mesh->mNormals[i].x;
				vrtx.norm.y = mesh->mNormals[i].y;
				vrtx.norm.z = mesh->mNormals[i].z;
			}

			vertices.push_back(vrtx);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// return a mesh object created from the extracted mesh data
		return new Mesh(raster, vertices, indices);
	}
};
