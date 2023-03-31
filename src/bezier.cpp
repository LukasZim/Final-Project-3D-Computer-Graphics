#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>

void bezier_curve_2d(float t, int point_count, float points[]) {
	for (int i = 0; i <= point_count; i++) {
		points[i] = factorial(point_count)/
			(factorial(i)*factorial(2-i))*
			std::pow(1-t, point_count-i)*
			std::pow(t,i)*
			points[i];
	}
}



int factorial(int a) {
	return a == 0 ? 1 : a * factorial(a - 1);
}

float x[] = { 0.0, 1.0, 2.5, 3.0 };
float y[] = { 1.3, 2.3, 3.3, 4.3 };

int size_x = sizeof(x) / sizeof(x[0]);
int size_y = sizeof(y) / sizeof(y[0]);

