#pragma once
#ifndef RASTERIZATION_H
#define RASTERIZATION_H
#include <glad\glad.h>
namespace rasterization {
	#define ROUND(a) (a > 0 ? (int)(a + 0.5) : (int)(a - 0.5))
	#define ABS(a) ((a) < 0 ? -(a) : (a))

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
			tmp = xa; xa = ya; ya = tmp;
			tmp = xb; xb = yb; yb = tmp;
		}
		if (xa > xb) {
			tmp = xa; xa = xb; xb = tmp;
			tmp = ya; ya = yb; yb = tmp;
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
}
#endif // !RASTERIZATION_H
