//////////////////////////////////////////////////////////////////////////////////
///      THIS FILE CONTAINS SOME CORE SCTRUCTURES REQUIRED BY THE PROJECT      /// 
/// IT ALSO ENSURES THAT ALL THE REQUIRED HEADERS ARE INCLUDED BEFORE THE GLUT ///
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#define _USE_MATH_DEFINES // To get access to M_PI variable
#include <vector>
#include <iostream>
#include "../core/glut.h" // GLUT needs to be included after the headers from the std library

using namespace std;

/**
* This structure represents a color value in the framework.
* It uses the RGBA (Red-Green-Blue-Alpha) system with float values in the range between 0.0 - 1.0.
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
struct Color
{
public:
	/** The constant for the red color: R=1.0, G=0.0, B=0.0, A=1.0. */
	static const Color RED;
	/** The constant for the green color: R=0.0, G=1.0, B=0.0, A=1.0. */
	static const Color GREEN;
	/** The constant for the blue color: R=0.0, G=0.0, B=1.0, A=1.0. */
	static const Color BLUE;
	/** The constant for the black color: R=0.0, G=0.0, B=0.0, A=1.0. */
	static const Color BLACK;
	/** The constant for the white color: R=1.0, G=1.0, B=1.0, A=1.0. */
	static  const Color WHITE;
	/** The constant for the yellow color: R=1.0, G=1.0, B=0.0, A=1.0. */
	static  const Color YELLOW;
	/** The constant for the magenta color: R=1.0, G=0.0, B=1.0, A=1.0. */
	static  const Color MAGENTA;
	/** The constant for the cyan color: R=0.0, G=1.0, B=1.0, A=1.0. */
	static  const Color CYAN;
	/** The constant for the orange color: R=1.0, G=0.43, B=0.2, A=1.0. */
	static  const Color ORANGE;
	/** The constant for the default color (gray): R=0.5, G=0.5, B=0.5, A=1.0. */
	static  const Color DEFAULT_COLOR;

	/** The red component. */
	float r;
	/** The green component. */
	float g;
	/** The blue component. */
	float b;
	/** The alpha component. */
	float a;

	/** Constructs a new (black) color. */
	Color() : r(0), g(0), b(0), a(1) {}

	/**
	* Constructs a new Color using the specified red, green, and blue values.
	*
	* @param red		The red component of the RGBA value.
	* @param green		The green component of the RGBA value.
	* @param blue		The blue component of the RGBA value.
	*/
	Color(float red, float green, float blue) : Color(red, green, blue, 1.0) {}

	/**
	* Constructs a new Color using the specified red, green, and blue values.
	*
	* @param red		The red component of the RGBA value.
	* @param green		The green component of the RGBA value.
	* @param blue		The blue component of the RGBA value.
	* @param alpha		The alpha component of the RGBA value.
	*/
	Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}

	/**
	* The '==' operator allowing to compare two colors for equality.
	* The colors are consider to be equal if their RBGA components are the same.
	*/
	bool operator==(const Color& c) const
	{
		return c.r == r && c.g == g && c.b == b && c.a == a;
	}

	/**
	* The '!=' operator allowing to compare two colors for non-equality.
	* @see '==' operator for details
	*/
	bool operator!=(const Color& c) const
	{
		return !(*this == c);
	}
};

/**
* The Point2D describes a point in 2D Cartesian space.
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
struct Point2D
{
	/** The X coordinate. */
	int x;

	/** The Y coordinate. */
	int y;

	/** Constructs a new Point2D at location (0,0). */
	Point2D() : x(0), y(0) {}

	/**
	* Constructs a new Point2D at location (x,y).
	* @param x			The X coordinate of the newly constructed Point2D.
	* @param y			The Y coordinate of the newly constructed Point2D.
	*/
	Point2D(int x, int y) : x(x), y(y) {}
};

/**
* The Point3D describes a point in 3D Cartesian space.
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
struct Point3D : Point2D
{
	/** The Z coordinate. */
	float z;

	/** Constructs a new Point2D at location (0,0). */
	Point3D() : Point2D(0, 0), z(0) {}

	/**
	* Constructs a new Point2D at location (x,y).
	* @param x			The X coordinate of the newly constructed Point3D.
	* @param y			The Y coordinate of the newly constructed Point3D.
	* @param y			The Z coordinate of the newly constructed Point3D.
	*/
	Point3D(float x, float y, float z) : Point2D(x, y), z(z) {}

	/**
	* Renders this point on the screen using the specified color.
	* @param color			The color that will be used to render this rectangle.
	*/
	void render(Color color) const
	{
		glPointSize(5.f);
		glBegin(GL_POINTS);
		glColor3f(color.r, color.g, color.b);
		glVertex3f(this->x, this->y, this->z);
		glEnd();
	}
};

/**
* The Line2D describes a directed line between two points in 2D Cartesian space.
*
* @author Jan Byška <jan.byska@gmail.com>
*/
struct Line2D
{
	/** The first point. */
	Point2D p1;
	/** The second point. */
	Point2D p2;

	/**
	* Constructs a new Line2D defined by two points (x1,y1) and (x2,y2).
	* @param x1			The X coordinate of the first point.
	* @param y1			The Y coordinate of the first point.
	* @param x2			The X coordinate of the second point.
	* @param y2			The Y coordinate of the second point.
	*/
	Line2D(int x1, int y1, int x2, int y2) {
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
	}

	/**
	* Constructs a new Line2D defined by two points (p1,p2).
	* @param p1			The first point.
	* @param p2			The second point.
	*/
	Line2D(Point2D p1, Point2D p2) {
		this->p1 = p1;
		this->p2 = p2;
	}

	/**
	* Renders this line on the screen using the specified color.
	* @param color			The color that will be used to render this line.
	*/
	void render(Color color) const
	{
		glBegin(GL_LINES);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(this->p1.x, this->p1.y);
		glVertex2f(this->p2.x, this->p2.y);
		glEnd();
	}

	void render(Color color, int width) const
	{
		glLineWidth(width);
		glBegin(GL_LINES);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(this->p1.x, this->p1.y);
		glVertex2f(this->p2.x, this->p2.y);
		glEnd();
		glLineWidth(1);
	}
};

/**
* The Rectangle2D describes a rectangle in 2D Cartesian space defined by a location (x,y) and two dimensions (width, height).
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
struct Rectangle2D
{
	/** The rectangle width. */
	float width = 0;
	/** The rectangle height. */
	float height = 0;
	/** The x position of the rectangle. */
	int x = 0;
	/** The y position of the rectangle. */
	int y = 0;

	/**
	* Constructs a new Rectangle2D of the specified with and height with upper-left corner placed at (x,y).
	* @param x			The X coordinate of the upper-left corner of the newly constructed Rectangle2D.
	* @param y			The Y coordinate of the upper-left corner of the newly constructed Rectangle2D.
	* @param w			The width of the newly constructed Rectangle2D.
	* @param h			The height of the newly constructed Rectangle2D.
	*/
	Rectangle2D(int x, int y, float width, float height) : x(x), y(y), width(width), height(height) {}

	/**
	* Tests if the specified coordinates are inside this rectangle.
	* @param x			The specified X coordinate to be tested.
	* @param y			The specified Y coordinate to be tested.
	*/
	inline bool contains(int x, int y) const
	{
		return x > this->x&& x < this->x + width && y > this->y&& y < this->y + height;
	}

	/**
	* The '==' operator allowing to compare two rectangles for equality.
	* The rectangles are consider to be equal if all their respective attributes (x,y,width,height) are the same.
	*/
	bool operator==(const Rectangle2D& r) const
	{
		return r.x == x && r.y == y && r.width == width && r.height == height;
	}

	/**
	* The '!=' operator allowing to compare two rectangles for non-equality.
	* @see '==' operator for details
	*/
	bool operator!=(const Rectangle2D& r) const
	{
		return !(*this == r);
	}

	/**
	* Renders this rectangle on the screen using the specified color.
	* @param color			The color that will be used to render this rectangle.
	*/
	void render(Color color) const
	{
		glBegin(GL_QUADS);
		glColor3f(color.r, color.g, color.b);
		glTexCoord2f(0, 0);
		glVertex2f(this->x, this->y);
		glTexCoord2f(1, 0);
		glVertex2f(this->x + width, this->y);
		glTexCoord2f(1, 1);
		glVertex2f(this->x + width, this->y + height);
		glTexCoord2f(0, 1);
		glVertex2f(this->x, this->y + height);
		glEnd();
	}
};