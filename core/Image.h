#pragma once

#include "Core.h"

class Image
{
private:

	/**
	 * The array of pixels - the array is organized as list of rows with 'nrChannels' values per pixel. 
	 * @see getIndex(int,int) method for more details
	 */
	unsigned char *data;

	/** The image width. */
	int width;

	/** The image height. */
	int height;

	/** The number of channels. */
	int nrChannels;

public:

	/** Constructs a new image from the given file. */
	Image(char const *filename);

	/** Constructs a new black image of the given size. */
	Image(int width, int height) : Image(width, height, 4) {}

	/** Constructs a new black image of the given size with the given number of channels. */
	Image(int width, int height, int nrChannels);

	/** Delete the data on image destruction. */
	~Image() {
		free(data);
	}

	/** Returns the image width. */
	inline int getWidth() const { return this->width; }
	/** Returns the image height. */
	inline int getHeight() const { return this->height; }
	/** Returns the raw data - useful for OpenGL textures. */
	inline unsigned char* getRawData() const { return this->data; }

	/**
	 * Sets the color of a pixel identified by the x (column) and y (row) coordinates.
	 * @param x the X-coordinate of the pixel to be changed
	 * @param y the Y-coordinate of the pixel to be changed
	 * @param color the new color
	 */
	void setPixelColor(int x, int y, Color color);

	/** Returns the current color of the raster pixel identified by the x (column) and y (row) coordinates. */
	Color getPixelColor(int x, int y) const;

private:

	/**
	 * Returns the first index (i.e., red channel) of the pixel at x,y in the data array. 
	 * Use: 
	 * getIndex(x,y) to get index for red value of pixel at x,y
	 * getIndex(x,y) + 1 to get index for green value of pixel at x,y 
	 * getIndex(x,y) + 2 to get index for blue value of pixel at x,y
	 *
	 * @param	x	The x coordinate in the image.
	 * @param	y	The y coordinate in the image.
	 *
	 * @returns	The corresponding index in the data array.
	 */
	inline int getIndex(int x, int y) const { return y * width * nrChannels + x * nrChannels; }
};

