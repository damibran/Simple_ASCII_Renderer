#pragma once
#include"../shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

class Mesh : public shape
{
public:
	// mesh Data
	vector<glm::vec3> vertices;
	vector<unsigned int> indices;

	// model data 
	string directory;

	// constructor, expects a filepath to a 3D model.
	Mesh(string const& path)
	{
		loadMesh(path);
	}
	Mesh(vector<glm::vec3> verts, std::vector<unsigned int> indes)
	{
		this->vertices = verts;
		this->indices = indes;
	}

	void drawChild(const glm::mat4& parent_trans)override
	{
		glm::mat4 thisTrans = parent_trans * position * rotation * scaling;

		for (int i = 0; indices.size()!=0 && i < indices.size()-1; ++i)
		{
			glm::vec4 a = glm::vec4(vertices[indices[i]],1.0f);
			glm::vec4 b = glm::vec4(vertices[indices[i+1]], 1.0f);

			glm::vec4 ndc_pos_a = thisTrans * a;
			glm::vec4 ndc_pos_b = thisTrans * b;

			int x0 = (ndc_pos_a)[0] / (ndc_pos_a)[3] * gScreen.XMAX + gScreen.XMAX / 2;
			int y0 = (ndc_pos_a)[1] / (ndc_pos_a)[3] * gScreen.YMAX + gScreen.YMAX / 2;

			int x1 = (ndc_pos_b)[0] / (ndc_pos_b)[3] * gScreen.XMAX + gScreen.XMAX / 2;
			int y1 = (ndc_pos_b)[1] / (ndc_pos_b)[3] * gScreen.YMAX + gScreen.YMAX / 2;

			if (ndc_pos_a.z <= ndc_pos_a.w && ndc_pos_a.z >= -ndc_pos_a.w && ndc_pos_b.z <= ndc_pos_b.w && ndc_pos_b.z >= -ndc_pos_b.w)
				put_line(x0, y0, x1, y1);
		}

		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}

private:
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadMesh(string const& path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 0);// aiProcess_Triangulate| aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::string err = importer.GetErrorString();
			gScreen.debug_massage("ERROR::ASSIMP:: "+ err);
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::shared_ptr<shape> child_mesh = std::shared_ptr<Mesh>(processMesh(mesh,scene));
			this->addChild(child_mesh);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		vector<glm::vec3> vertices;
		vector<unsigned int> indices;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 vertex; 

			vertex.x = mesh->mVertices[i].x;
			vertex.y = mesh->mVertices[i].y;
			vertex.z = mesh->mVertices[i].z;

			vertices.push_back(vertex);
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
		return new Mesh(vertices, indices);
	}

	void put_line(int x0, int y0, int x1, int y1)
		/* Алгоритм Брезенхэма для прямой:
		рисование отрезка прямой от (x0,y0) до (x1,y1).
		Уравнение прямой: b(x-x0) + a(y-y0) = 0.
		Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).  */
	{
		int dx = 1;
		int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
		int dy = 1;
		int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		int eps = 0;
		for (;;) { //Формирование прямой линии по точкам
			gScreen.put_point(x0, y0);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
	}
};
