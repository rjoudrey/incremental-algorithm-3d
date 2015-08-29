#include <iostream>
#include <sstream>
#include "platform.h"
#include "system.h"
#include "Render.h"
#include "camera.h"
#include "shader.h"
#include "Color.h"
#include "Light.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <ctime>
#include <cmath>
using namespace std;

string assetsPath() {
	return "../../assets/";
}

string shadersPath() {
	return assetsPath() + "shaders/";
}

string pathForShaderWithName(string shaderName) {
	return shadersPath() + shaderName;
}

class Edge {
public:
	int indices[2];
	Edge(int i0, int i1) {
		indices[0] = i0;
		indices[1] = i1;
	}
	bool operator==(const Edge &other) const {
		return indices[0] == other.indices[0] && indices[1] == other.indices[1]
		   || indices[1] == other.indices[0] && indices[0] == other.indices[1];
	}
};

class Triangle {
public:
	Triangle(int i0, int i1, int i2) {
		indices[0] = i0;
		indices[1] = i1;
		indices[2] = i2;
		// Generate random rendering color
		for (int i = 0; i < 3; ++i)  {
		   color[i]	= (rand() % 1000) / 1000.0f;
		}
		color[3] = 1.0f;
	}
	int indices[3];
	float color[4];
	vector<Edge> Edges() {
		vector<Edge> edges;
		edges.push_back(Edge(indices[0], indices[1]));
		edges.push_back(Edge(indices[1], indices[2]));
		edges.push_back(Edge(indices[2], indices[0]));
		return edges;
	}
};

// Incremental Algorithm steps:
// 1.) Discard faces visible from v
//     a.) If no faces are visible from v, v is inside hull
// 2.) If v is not inside the hull:
//     a.) Discard vertices with no faces referring to them
//     b.) Construct the cone

class Hull {
public:
	vector<Vec3f>vertices;
	// Contains references to Vec3f indices
	vector<Triangle>faces;
	void AddVertex(Vec3f v) {
		vertices.push_back(v);
	}
   	void ConstructCone(vector<Edge>borderEdges, Vec3f v) {
		vertices.push_back(v);
		for (int i = 0; i < borderEdges.size(); ++i) {
			Edge e = borderEdges[i];
			AddFace(Triangle(e.indices[0], e.indices[1], vertices.size() - 1));
		}
	}
	vector<Edge> Edges() {
		vector<Edge>edges;
		for (int i = 0; i < faces.size(); ++i) {
			Triangle &t = faces[i];
			vector<Edge> tEdges = t.Edges();
			for (int j = 0; j < tEdges.size(); ++j) {
				edges.push_back(tEdges[j]);
			}
		}
		return edges;
	}
	// Every border edge has a single face referring to it
	vector<Edge> BorderEdges() {
		vector<Edge> borderEdges;
		vector<Edge> edges = Edges();
		for (int i = 0; i < edges.size(); ++i) {
			if (NumFacesReferringToEdge(edges[i]) == 1) {
				borderEdges.push_back(Edge(edges[i].indices[1], edges[i].indices[0]));
			}
		}
		return borderEdges;
	}
	int NumFacesReferringToEdge(Edge e) {
		int numReferences = 0;
		for (int i = 0; i < faces.size(); ++i) {
			Triangle &t = faces[i];
			vector<Edge> tEdges = t.Edges();
			for (int j = 0; j < tEdges.size(); ++j) {
				Edge tEdgeCur = tEdges[j];
				if (e == tEdgeCur) {
					++numReferences;
				}
			}
		}
		return numReferences;
	}
	int NumFacesReferringToVertex(int vIndex) {
		int numReferences = 0;
		for (int i = 0; i < faces.size(); ++i) {
			for (int j = 0; j < 3; ++j) {
				if (faces[i].indices[j] == vIndex) {
					++numReferences;
				}
			}
		}
		return numReferences;
	}
	void DiscardFace(int index) {
		faces.erase(faces.begin() + index);
	}
	void DiscardUnreferencedVertices() {
		for (int i = 0; i < vertices.size(); ++i) {
			if (!NumFacesReferringToVertex(i)) {
				DiscardVertex(i);
				--i;
			}
		}
	}
	// Each time a a vertex at index i is deleted,
	// all faces containing index references to any vertex j > i should be decremented once
	void DiscardVertex(int vIndex) {
				vertices.erase(vertices.begin() + vIndex);
		for (int i = 0; i < faces.size(); ++i) {
			for (int j = 0; j < 3; ++j) {
				if (faces[i].indices[j] > vIndex) {
					--faces[i].indices[j];
				}
			}
		}
	}
	int DiscardFacesVisibleFromVertex(Vec3f v) {
		int numDiscarded = 0;
		for (int i = 0; i < faces.size(); ++i) {
			if (FaceIsVisible(v, faces[i])) {
				DiscardFace(i);
				--i;
				++numDiscarded;
			}
		}
		return numDiscarded;
	}
	// A face t is "visible" from a vertex v 
	// if for the angle t between
	//  the direction d from v to some vertex w in t
	//  and the normal of the face n
	// t <= PI/2
	bool FaceIsVisible(Vec3f v, Triangle f) {
		Vec3f n = NormalForFace(f);
		Vec3f w = vertices[f.indices[0]];
		Vec3f d = (v - w).Normalize();
		float t = dot(d, n);
		float a = acos(t);
		return a <= (PI / 2.0f);
	}
	void AddFace(Triangle t) {
		faces.push_back(t);
	}
	Vec3f NormalForFace(Triangle t) {
	   return Vec3f::CalculateNormal(
		   vertices[t.indices[0]],
		   vertices[t.indices[1]],
		   vertices[t.indices[2]]
	   );
	}
	vector<Vec3f>normals() {
		vector<Vec3f>normals;
		for (int i = 0; i < faces.size(); ++i) {
			Vec3f n = NormalForFace(faces[i]);
			normals.push_back(n);
			normals.push_back(n);
			normals.push_back(n);
		}
		return normals;
	}
	void Render() {
		if (!faces.size())
			return;
		// Draw the triangles
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < faces.size(); ++i) {
			Triangle &t = faces[i];
			glNormal3fv(NormalForFace(t).values);
			glColor4fv(t.color);
			for (int j = 0; j < 3; ++j) {
				glVertex3fv(vertices[t.indices[j]].values);
			}
		}
		float color[4] = { 0, 0, 0, 0.5 };
		glColor4fv(color);
		glEnd();
		// Draw the vertices
		Render::EnableBlending();
		for (int i = 0; i < vertices.size(); ++i) {
			Vec3f &v = vertices[i];
			Render::SaveModelViewMatrix();
			Render::Translate(v.values);		
			Render::Scale(0.02f);
		   	Render::DrawCube();
			Render::RestoreModelViewMatrix();
		}
		Render::DisableBlending();
	}
};

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int windowW = 1024, windowH = 800;
	System::Initialize("CG Project - 3D Incremental Algorithm by Ricky Joudrey", windowW, windowH);
	Render::Initialize(windowW, windowH);
	Shader::InitShaderSystem(pathForShaderWithName("default.VS"), pathForShaderWithName("default.FS"));

	Shader shader = Shader::defaultShader;
	shader.Activate();

	Light light;
	light.position = Vec3f(10.0f, 10.0f, 10.0f);
	Color ambient, diffuse, specular;
	float shininess = 128.0f;
	ambient.SetColor(.6f, .6f, .6f, 1.0f);
	diffuse.SetColor(1, 1, 1, 1.0f);
	specular.SetColor(.05f, .05f, .05f, 1.0f);
	shader.UseDataInShader(Shader::FLOAT4D, "light.ambient", light.ambientColor.values);
	shader.UseDataInShader(Shader::FLOAT4D, "light.diffuse", light.diffuseColor.values);
	shader.UseDataInShader(Shader::FLOAT4D, "light.specular", light.specularColor.values);
	shader.UseDataInShader(Shader::FLOAT3D, "light.position", light.position.values);
	shader.UseDataInShader(Shader::FLOAT4D, "matColor.ambient", ambient.values);
	shader.UseDataInShader(Shader::FLOAT4D, "matColor.diffuse", diffuse.values);
	shader.UseDataInShader(Shader::FLOAT4D, "matColor.specular", specular.values);
	shader.UseDataInShader(Shader::FLOAT, "matColor.shininess", &shininess);

	Hull hull;
	// Construct the initial tetrahedron
	hull.AddVertex(Vec3f(1, 0, 0));
	hull.AddVertex(Vec3f(0, 1, 0));
	hull.AddVertex(Vec3f(-1, 0, 0));
	hull.AddVertex(Vec3f(0, 0, 1));
	hull.AddFace(Triangle(2, 1, 0));
	hull.AddFace(Triangle(0, 1, 3));
	hull.AddFace(Triangle(3, 1, 2));
	hull.AddFace(Triangle(0, 3, 2));

	const int numVertsToGenerate = 100;
	vector<Hull>hullHistory;
	// save initial tetrahedron
	hullHistory.push_back(hull);

	// Here, we generate a number of hulls,
	// such that for each hull h_i,
	// h_i+1 is the hull generated by adding a vertex
	// v to h_i using the incremental algorithm
	for (int n = 0; n < numVertsToGenerate; ++n) {
		Vec3f v;
		for (int i = 0; i < 3; ++i) {
			// floating point number between 0 and 2.5
			v.values[i] = ((rand() % 1001) / 100.0f) *.25;
		}
		
		// Add the vertex, save
		hull.AddVertex(v);
		hullHistory.push_back(hull);
		
		// If no faces were visible to this vertex,
		// then the vertex is inside the hull,
		// we will skip this vertex and generate a new one,
		// since showing a newly generated vertex is not
		// preferred for demonstration purposes
		int numDiscarded = hull.DiscardFacesVisibleFromVertex(v);
		if (!numDiscarded) {
			hull.DiscardVertex(hull.vertices.size() - 1);
			hullHistory.erase(hullHistory.end() - 1);
			continue;
		}

		else {			
			// Discard faces, save
			hullHistory.push_back(hull);
			hull.DiscardVertex(hull.vertices.size() - 1);
			// Discard unreferenced verts, save
			hull.DiscardUnreferencedVertices();
			hull.AddVertex(v);
			hullHistory.push_back(hull);
			hull.DiscardVertex(hull.vertices.size() - 1);
			// Construct cone, save
			hull.ConstructCone(hull.BorderEdges(), v);
			hullHistory.push_back(hull);
		}
	}
	int currentHullIndex = 0;
	bool autoAdvance = true;
	int timeOfLastUpdate = 0;
	do {
		int timeNow = time(NULL);
		int timeDiffSecs = difftime(timeNow, timeOfLastUpdate);
		if (autoAdvance && timeOfLastUpdate == 0 || timeDiffSecs == 1) {
			timeOfLastUpdate = timeNow;
			currentHullIndex = (currentHullIndex + 1) % hullHistory.size();
		}
		// Input
		System::UpdateState();
		Camera::HandleAim();
		System::CenterMouse();
		if (System::IsKeyDown("m"))
			Render::EnableWireframe();
		else if (System::IsKeyDown("n"))
			Render::DisableWireframe();
		else if (System::IsKeyDown("w"))
			Camera::Move(Camera::FORWARD);
		else if (System::IsKeyDown("s"))
			Camera::Move(Camera::BACKWARD);
		else if (System::IsKeyDown("z"))
			glEnable(GL_CULL_FACE);
		else if (System::IsKeyDown("x"))
			glDisable(GL_CULL_FACE);
		if (System::IsKeyDown("a"))
			Camera::Move(Camera::LEFT);
		else if (System::IsKeyDown("d"))
			Camera::Move(Camera::RIGHT);
		else if (System::IsKeyDown("k")) {
			if (currentHullIndex > 0)
		   	--currentHullIndex;
		}
		else if (System::IsKeyDown("l")) {
			if (currentHullIndex < hullHistory.size() - 1) 
				++currentHullIndex;
		}
		// Rendering
		Render::ClearBuffers(true, true, false);
		Render::ResetModelViewMatrix();
		
		// Set view matrix (world -> eye)
		Camera::LookAt();
		hullHistory[currentHullIndex].Render();
		Render::SwapBuffers();
		System::DelayFrameRate(60);
	} while (!System::IsKeyDown("ESC"));

	return 0;
}