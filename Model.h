#pragma once

#include "OBJ_Loader.h" 
#include <GL/freeglut.h>
#include "texture.h"
#include <map>

using namespace std;

class Model {
protected:
	objl::Loader meshes; // collection of triangles
	map<string, Texture *> texID;
	

public:
	void init(string path, string filename) {
		string fullpath = path + "/" + filename;
		int start = glutGet(GLUT_ELAPSED_TIME);
		
		
		if (!meshes.LoadFile(fullpath)) {
			cout << "Error: Loading " << fullpath << endl;
			exit(1);
		}
		int diff = glutGet(GLUT_ELAPSED_TIME) - start;
		cout << "Loaded in " << diff << " ms" << endl;

		cout << "Loaded 3D Model : " << fullpath << endl;
		loadTextures(path);
	
	}
	void applyMaterial(objl::Mesh &curMesh) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&curMesh.MeshMaterial.Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&curMesh.MeshMaterial.Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&curMesh.MeshMaterial.Ks);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, curMesh.MeshMaterial.Ns);
	}
	void render() {
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (int i = 0; i < meshes.LoadedMeshes.size(); i++) {
			objl::Mesh curMesh = meshes.LoadedMeshes[i];

			applyMaterial(curMesh);
			glPushAttrib(GL_ENABLE_BIT);
			if (texID.find(curMesh.MeshMaterial.map_Kd) == texID.end()) {
				glDisable(GL_TEXTURE_2D);
				glColor3fv((GLfloat *)&curMesh.MeshMaterial.Kd);
			//	cout << curMesh.MeshMaterial.Kd.Z << endl;
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texID[curMesh.MeshMaterial.map_Kd]->id);
			}

			
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < curMesh.Indices.size(); j++) {
				int V = curMesh.Indices[j];
				glTexCoord2f(curMesh.Vertices[V].TextureCoordinate.X, curMesh.Vertices[V].TextureCoordinate.Y);
				glNormal3f(curMesh.Vertices[V].Normal.X, curMesh.Vertices[V].Normal.Y, curMesh.Vertices[V].Normal.Z);
				glVertex3f(curMesh.Vertices[V].Position.X, curMesh.Vertices[V].Position.Y, curMesh.Vertices[V].Position.Z);
			
			}
			glEnd();
	
			glPopAttrib();
		}
	}

	void loadTextures(string path) {
		for (int i = 0; i < meshes.LoadedMeshes.size(); i++) {
			objl::Mesh curMesh = meshes.LoadedMeshes[i];
			string textureName = curMesh.MeshMaterial.map_Kd; // diffuse texture mapping

			if (!textureName.empty() && texID.find(textureName) == texID.end()) {
				Texture *t = new Texture();
				string fullpath = path + "/" + textureName;
				int res = t->Load(fullpath.c_str()); // string -> const char *
				if (!res) {
					cout << "Error : Texture File " << fullpath << endl;
				}
				else {
					texID.insert(make_pair(curMesh.MeshMaterial.map_Kd, t)); // insert <string, Texture*> into map
					cout << "Loaded Texture:" << fullpath << endl;
				}

			}
		}
	}
};
