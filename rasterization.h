#pragma once
#ifndef RASTERIZATION_H
#define RASTERIZATION_H
#include <glad\glad.h>
#include <cstdio>
namespace rasterization {
	#define ROUND(a) (a > 0 ? (int)(a + 0.5) : (int)(a - 0.5))
	#define ABS(a) ((a) < 0 ? -(a) : (a))
	#define MAX(a, b) ((a) < (b) ? (b) : (a))
	#define MIN(a, b) ((a) < (b) ? (a) : (b))

	template<class T>
	void Swap(T &lsh, T &rsh) {
		T tmp = lsh;
		lsh = rsh;
		rsh = tmp;
	}

	void LineDDA(int xa, int ya, int xb, int yb, int viewSize) {
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		int dx = xb - xa, dy = yb - ya, adx, ady, x, inc1, k, count;
		float y, inc2;
		GLfloat *vert;
		adx = ABS(dx);
		ady = ABS(dy);

		if (adx > ady) {
			count = adx * 3 + 3;
			vert = new GLfloat[count + 1];
			vert[0] = xa / (GLfloat)viewSize;
			vert[1] = ya / (GLfloat)viewSize;
			vert[2] = 0;
			x = (GLfloat)xa;
			y = (GLfloat)ya;
			inc1 = xb > xa ? 1 : -1;
			inc2 = dy / (GLfloat)dx;
			for (k = 1; k <= adx; ++k) {
				x += inc1;
				y += inc2;
				vert[3 * k] = x / (GLfloat)viewSize;
				vert[3 * k + 1] = ROUND(y) / (GLfloat)viewSize;
				vert[3 * k + 2] = 0;
			}
		}
		else {
			count = ady * 3 + 3;
			vert = new GLfloat[count];
			vert[0] = xa / (GLfloat)viewSize;
			vert[1] = ya / (GLfloat)viewSize;
			vert[2] = 0;
			x = (GLfloat)ya;
			y = (GLfloat)xa;
			inc1 = yb > ya ? 1 : -1;
			inc2 = dx / (float)dy;
			for (k = 1; k <= ady; ++k) {
				x += inc1;
				y += inc2;
				vert[3 * k] = ROUND(y) / (GLfloat)viewSize;
				vert[3 * k + 1] = x / (GLfloat)viewSize;
				vert[3 * k + 2] = 0;
			}
		}
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vert, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, count / 3);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		delete[] vert;
	}

	void LineBresenham(int xa, int ya, int xb, int yb, int viewSize) {
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		bool steep = ABS(yb - ya) > ABS(xb - xa);
		GLfloat *vert;
		int tmp, count;
		if (steep) {
			Swap(xa, ya);
			Swap(xb, yb);
		}
		if (xa > xb) {
			Swap(xa, xb);
			Swap(ya, yb);
		}
		int dy = ABS(yb - ya), dx = xb - xa, pi, x, y = ya, inc = yb > ya ? 1 : -1;
		pi = -dx;
		dy <<= 1;
		dx <<= 1;
		count = (xb - xa) * 3 + 3;
		vert = new GLfloat[count];
		if (steep) {
			for (x = xa; x <= xb; ++x) {
				tmp = (x - xa) * 3;
				vert[tmp] = y / (GLfloat)viewSize;
				vert[tmp + 1] = x / (GLfloat)viewSize;
				vert[tmp + 2] = 0;
				pi += dy;
				if (pi > 0) {
					y += inc;
					pi -= dx;
				}
			}
		}
		else {
			for (x = xa; x <= xb; ++x) {
				tmp = (x - xa) * 3;
				vert[tmp] = x / (GLfloat)viewSize;
				vert[tmp + 1] = y / (GLfloat)viewSize;
				vert[tmp + 2] = 0;
				pi += dy;
				if (pi > 0) {
					y += inc;
					pi -= dx;
				}
			}
		}
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vert, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, count / 3);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		delete[] vert;
	}

	void CircleBresenham(int x0, int y0, int r, int viewSize) {
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		int x = 0, y = r, pi = 4 * r - 1, count, index = 0;
		GLfloat *vert;
		count = ((int)(0.7071067811865475 * r) + 2) * 24;
		vert = new GLfloat[count];
		while (x <= y)
		{
			vert[index] = (x0 + x) / (GLfloat)viewSize;
			vert[index + 1] = (y0 + y) / (GLfloat)viewSize;
			vert[index + 2] = 0;
			vert[index + 3] = (x0 + y) / (GLfloat)viewSize;
			vert[index + 4] = (y0 + x) / (GLfloat)viewSize;
			vert[index + 5] = 0;
			vert[index + 6] = (x0 + x) / (GLfloat)viewSize;
			vert[index + 7] = (y0 - y) / (GLfloat)viewSize;
			vert[index + 8] = 0;
			vert[index + 9] = (x0 + y) / (GLfloat)viewSize;
			vert[index + 10] = (y0 - x) / (GLfloat)viewSize;
			vert[index + 11] = 0;
			vert[index + 12] = (x0 - x) / (GLfloat)viewSize;
			vert[index + 13] = (y0 + y) / (GLfloat)viewSize;
			vert[index + 14] = 0;
			vert[index + 15] = (x0 - y) / (GLfloat)viewSize;
			vert[index + 16] = (y0 + x) / (GLfloat)viewSize;
			vert[index + 17] = 0;
			vert[index + 18] = (x0 - x) / (GLfloat)viewSize;
			vert[index + 19] = (y0 - y) / (GLfloat)viewSize;
			vert[index + 20] = 0;
			vert[index + 21] = (x0 - y) / (GLfloat)viewSize;
			vert[index + 22] = (y0 - x) / (GLfloat)viewSize;
			vert[index + 23] = 0;
			index += 24;
			if (pi < 0) {
				pi += 8 * (y - x) - 12;
				--y;
			}
			else pi -= 8 * x + 4;
			++x;
		}
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vert, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, count / 3);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		delete[] vert;
	}

	void EdgeWalking(int x0, int y0, int x1, int y1, int x2, int y2, int viewSize) {
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (y0 < y1 || (y0 == y1 && x0 > x1)) {
			Swap(x0, x1);
			Swap(y0, y1);
		}
		if (y0 < y2 || (y0 == y2 && x0 > x2)) {
			Swap(x0, x2);
			Swap(y0, y2);
		}
		if (y1 < y2 || (y1 == y2 && x1 > x2)) {
			Swap(x1, x2);
			Swap(y1, y2);
		}
		int minX, minY, maxX, maxY, x, y, iXL, iXR;
		maxY = y0;
		minY = y2;
		minX = MIN(x0, MIN(x1, x2));
		maxX = MAX(x0, MAX(x1, x2));
		int count = (maxX - minX + 1) * (maxY - minY + 1);
		float XL, XR, k1, k2;
		GLfloat *vert = new GLfloat[count * 3];
		count = 0;
		if (y0 == y1) {
			XL = x0;XR = x1;
			k1 = (x0 - x2) / (GLfloat)(y2 - y0);
			k2 = (x1 - x2) / (GLfloat)(y2 - y1);
			for (y = y0; y >= y2; --y) {
				iXL = ROUND(XL); iXR = ROUND(XR);
				for (x = iXL; x <= iXR; ++x) {
					vert[count++] = x / (GLfloat)viewSize;
					vert[count++] = y / (GLfloat)viewSize;
					vert[count++] = 0;
				}
				XL += k1;XR += k2;
			}
		}
		else
		if (y1 == y2) {
			XL = x0; XR = x0;
			k1 = (x0 - x1) / (GLfloat)(y1 - y0);
			k2 = (x0 - x2) / (GLfloat)(y2 - y0);
			for (y = y0; y >= y2; --y) {
				iXL = ROUND(XL); iXR = ROUND(XR);
				for (x = iXL; x <= iXR; ++x) {
					vert[count++] = x / (GLfloat)viewSize;
					vert[count++] = y / (GLfloat)viewSize;
					vert[count++] = 0;
				}
				XL += k1; XR += k2;
			}
		}
		else {
			XL = x0; XR = x0;
			k1 = (x0 - x1) / (GLfloat)(y1 - y0);
			k2 = (x0 - x2) / (GLfloat)(y2 - y0);
			if (k1 > k2)
				Swap(k1, k2);
			for (y = y0; y > y1; --y) {
				iXL = ROUND(XL); iXR = ROUND(XR);
				for (x = iXL; x <= iXR; ++x) {
					vert[count++] = x / (GLfloat)viewSize;
					vert[count++] = y / (GLfloat)viewSize;
					vert[count++] = 0;
				}
				XL += k1; XR += k2;
			}
			if (x1 < x2)
				k1 = (x1 - x2) / (GLfloat)(y2 - y1);
			else
				k2 = (x1 - x2) / (GLfloat)(y2 - y1);
			for (y = y1; y >= y2; --y) {
				iXL = ROUND(XL); iXR = ROUND(XR);
				for (x = iXL; x <= iXR; ++x) {
					vert[count++] = x / (GLfloat)viewSize;
					vert[count++] = y / (GLfloat)viewSize;
					vert[count++] = 0;
				}
				XL += k1; XR += k2;
			}
		}

		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vert, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, count / 3);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		delete[] vert;
	}

	void EdgeEquations(int x0, int y0, int x1, int y1, int x2, int y2, int viewSize) {
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		int minX, minY, maxX, maxY, x, y;
		maxY = MAX(y0, MAX(y1, y2));
		minY = MIN(y0, MIN(y1, y2));
		minX = MIN(x0, MIN(x1, x2));
		maxX = MAX(x0, MAX(x1, x2));
		int count = (maxX - minX + 1) * (maxY - minY + 1);
		GLfloat *vert = new GLfloat[count * 3];
		count = 0;
		for(x = minX; x <= maxX; ++x)
			for (y = minY; y <= maxY; ++y) {
				int cross1 = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
				int cross2 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
				int cross3 = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
				if ((cross1 > 0 && cross2 > 0 && cross3 > 0) || (cross1 < 0 && cross2 < 0 && cross3 < 0) || (cross1 == 0 && !((cross2 > 0) ^ (cross3 > 0))) || (cross2 == 0 && !((cross1 > 0) ^ (cross3 > 0))) || (cross3 == 0 && !((cross2 > 0) ^ (cross1 > 0)))) {
					vert[count++] = x / (GLfloat)viewSize;
					vert[count++] = y / (GLfloat)viewSize;
					vert[count++] = 0;
				}
			}
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), vert, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, count / 3);
		glBindVertexArray(0);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		delete[] vert;
	}
}
#endif // !RASTERIZATION_H
