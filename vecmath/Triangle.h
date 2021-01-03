#pragma once
#include "Tuple3f.h"
#include <vector>
#include "../core/Core.h"

/**
* The Triangle describes a triangle in 3D Cartesian space defined by a three points.
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
class Triangle
{
public:
	/** The first point. */
	Tuple3f v1;
	/** The second point. */
	Tuple3f v2;
	/** The third point. */
	Tuple3f v3;

	/** Constructs a new triangle.
	 * @param v1	The first point.
	 * @param v2	The second point.
	 * @param v3	The third point.
	 */
	Triangle(const Tuple3f v1, const Tuple3f v2, const Tuple3f v3) : v1(v1), v2(v2), v3(v3)
	{
	}

	/** Returns all vertices. */
	vector<Tuple3f> getVertices();

	/**
	* Renders this triangle on the screen using the specified color.
	* @param color			The color that will be used to render this rectangle.
	*/
	void render(Color color) const;
};