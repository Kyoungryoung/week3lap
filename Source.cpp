
#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"

const int width = 1000;
const int height = 1000;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.
void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	if (i0 == i1)
	{
		for (int j = j0; j < j1; j++)
			drawPixel(i0, j, red, green, blue);
		return;
	}

	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}

//void drawCircle(const int& i0, const int& j0, const float& red, const float& green, const float& blue)
//{
//	int i = 0;
//	int r = 50;
//	
//
//	for (i = i0; i <= i0+100; i++)
//	{
//
//		
//		const int j =sqrt((double)(r*r - (i - i0)*(i - i0)))+j0;
//		drawPixel(i, j, red, green, blue);
//	}
//}

void drawOnPixelBuffer()
{
	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

	for (int i = 0; i<width*height; i++) {
		pixels[i * 3 + 0] = 1.0f; // red 
		pixels[i * 3 + 1] = 1.0f; // green
		pixels[i * 3 + 2] = 1.0f; // blue
	}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);

	// drawing a line
	//TODO: anti-aliasing
	//TODO: try moving object
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//thickner line
		for (int i = 0; i < 20; i++){
			drawLine(100, 50+i, 200, 80+i, 1.0f, 0.0f, 0.0f);
		}

		//square (empty)
		drawLine(100, 200, 200, 200, 1.0f, 0.0f, 0.0f);
		drawLine(100, 200, 100, 300, 1.0f, 0.0f, 0.0f);
		drawLine(100, 300, 200, 300, 1.0f, 0.0f, 0.0f);
		drawLine(200, 200, 200, 300, 1.0f, 0.0f, 0.0f);

		// square
		for (int k = 400; k < 500; k++){
			drawLine(100, k, 200, k, 1.0f, 1.0f, 0.0f);
		}

		//triangle
		drawLine(100, 600, 200, 600, 1.0f, 0.0f, 0.0f);
		for (int i = 0; i < 2; i++){
			drawLine(100, 600 + i, 150, 700 + i, 1.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < 2; i++){
			drawLine(150, 700 + i, 200, 600 + i, 1.0f, 0.0f, 0.0f);
		}

		//Pentagon
		drawLine(300, 600, 350, 600, 1.0f, 0.0f, 0.0f);
		for (int i = 0; i < 3; i++){
			drawLine(270, 650 + i, 300, 600 + i, 1.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < 3; i++){
			drawLine(350, 600 + i, 380, 650 + i, 1.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < 2; i++){
			drawLine(270, 650 + i, 325, 700 + i, 1.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < 2; i++){
			drawLine(325, 700 + i, 380, 650 + i, 1.0f, 0.0f, 0.0f);
		}

		const int i_center = 400, j_center = 250;
		const int thickness = 45;


		for (int a = 0; a < width; a++)
		{
			for (int b = 0; b < height; b++)
			{
				const int c1 = 400, c2 = 70;
				const int r = 50;
				if (sqrt((c1 - a)*(c1 - a) + (c2 - b)*(c2 - b)) < r)
					drawPixel(a, b, 1.0f, 0.0f, 0.0f);
				if (sqrt((400 - a)*(400 - a) + (70 - b)*(70 - b)) < 48)
					drawPixel(a, b, 1.0f, 1.0f, 1.0f);
			}
		}

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);



		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}
