#pragma once
#ifndef BALL_H
#define BALL_H
class BALL {
	unsigned int VAO, texture;
	int count;
public:
	BALL(const int &m, const int &n, const float &r, const char *path);
	~BALL();
	void Draw();
};
#endif // !BALL_H
