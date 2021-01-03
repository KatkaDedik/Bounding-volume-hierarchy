#include "Image.h"

// implements the STB library
#define STB_IMAGE_IMPLEMENTATION 
#include "../core/TextureLoader.h"

Image::Image(char const *filename)
{
	this->data = stbi_load(filename, &width, &height, &nrChannels, 0);
}

Image::Image(int width, int height, int nrChannels) : width(width), height(height), nrChannels(nrChannels)
{
	// we use old C to ensure compatibility with stb_image and GLUT libraries
	this->data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * nrChannels); 

	for (int i = 0; i < width*height*nrChannels; i++) {
		this->data[i] = 255;
	}
}

void Image::setPixelColor(int x, int y, Color color)
{
	int index = this->getIndex(x, y);
	this->data[index] = (int)(color.r * 255);
	this->data[index + 1] = (int)(color.g * 255);
	this->data[index + 2] = (int)(color.b * 255);
	this->data[index + 3] = (int)(color.a * 255);
}

Color Image::getPixelColor(int x, int y) const
{
	int index = this->getIndex(x, y);
	return Color(data[index] / 255.0, data[index + 1] / 255.0, data[index + 2] / 255.0, data[index+3] / 255.0);
}
