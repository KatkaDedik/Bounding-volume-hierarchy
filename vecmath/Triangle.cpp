#include "Triangle.h"

/** Returns all vertices. */
vector<Tuple3f> Triangle::getVertices()
{
	vector<Tuple3f> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	return vertices;
}


/**
* Renders this triangle on the screen using the specified color.
* @param color			The color that will be used to render this rectangle.
*/
void Triangle::render(Color color) const
{
	glBegin(GL_TRIANGLES);
	glColor3f(color.r, color.g, color.b);
	glVertex3f(this->v1.x, this->v1.y, this->v1.z);
	glVertex3f(this->v2.x, this->v2.y, this->v2.z);
	glVertex3f(this->v3.x, this->v3.y, this->v3.z);
	glEnd();
}