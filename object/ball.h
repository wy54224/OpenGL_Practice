#pragma once
#ifndef BALL_H
#define BALL_H
#include "mesh.h"
#include "colorMesh.h"
namespace Object {
	Mesh getBall(const int &m, const int &n, const float &r, const bool &isReverse, const unsigned int &diffuse, const unsigned int &specular) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int>indices;
		std::vector<Texture>textures;
		const float PI = 3.1415926535897932384626;
		float AngZ = 0, AngXY = 0, IncAngZ = PI / m, IncAngXY = PI * 2 / n;
		for (int i = 0; i <= m; ++i) {
			float RsinZ = r * (float)sin(AngZ), RcosZ = r * (float)cos(AngZ);
			float tX = i / (float)m;
			for (int j = 0; j <= n; ++j) {
				Vertex vertex;
				vertex.Position.x = RsinZ * (float)cos(AngXY);
				vertex.Position.y = RsinZ * (float)sin(AngXY);
				vertex.Position.z = RcosZ;
				vertex.TexCoord.x = j / (float)n;
				vertex.TexCoord.y = tX;
				vertex.Normal.x = RsinZ * (float)cos(AngXY);
				vertex.Normal.y = RsinZ * (float)sin(AngXY);
				vertex.Normal.z = RcosZ;
				vertices.push_back(vertex);
				AngXY += IncAngXY;
			}
			AngXY = 0;
			AngZ += IncAngZ;
		}
		if (isReverse) {
			for (int i = 1; i <= m; ++i)
				for (int j = 1; j <= n; ++j) {
					int left_top, left_buttom, right_top, right_buttom;
					right_buttom = i * (n + 1) + j;
					left_buttom = right_buttom - 1;
					right_top = right_buttom - n - 1;
					left_top = right_top - 1;
					indices.push_back(right_top);
					indices.push_back(left_buttom);
					indices.push_back(left_top);
					indices.push_back(right_buttom);
					indices.push_back(left_buttom);
					indices.push_back(right_top);
				}
		}
		else {
			for (int i = 1; i <= m; ++i)
				for (int j = 1; j <= n; ++j) {
					int left_top, left_buttom, right_top, right_buttom;
					right_buttom = i * (n + 1) + j;
					left_buttom = right_buttom - 1;
					right_top = right_buttom - n - 1;
					left_top = right_top - 1;
					indices.push_back(left_top);
					indices.push_back(left_buttom);
					indices.push_back(right_top);
					indices.push_back(right_top);
					indices.push_back(left_buttom);
					indices.push_back(right_buttom);
				}
		}
		textures.push_back(Texture(diffuse, "diffuse"));
		textures.push_back(Texture(specular, "specular"));
		return Mesh(vertices, indices, textures);
	}

	class SkyBall : public Mesh {
	public:
		SkyBall(const float &r, const unsigned int &diffuse, const unsigned int &specular, const bool &isReverse):
			Mesh(getBall(20, 20, r, isReverse, diffuse, specular)){}
	};
}
#endif // !BALL_H
