#pragma once
#include <iostream>

/** 
* The simple class representing a 3D tuple. 
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
class Tuple3f
{

public:
	/** The X coordinate. */
	float x;
	/** The Y coordinate. */
	float y;
	/** The Z coordinate. */
	float z;

public:
	/** The default constructor that creates zero tuple (0,0,0). */
	Tuple3f();

	/**
	* The constructor allowing to set the actual x,y,z, coordinates.
	*
	* @param x			The X coordinate.
	* @param y			The Y coordinate.
	* @param z			The Z coordinate.
	*/
	Tuple3f(float x, float y, float z);

	/**
	* The copy constructor.
	*
	* @param other		The tuple its values will be copied into the new tuple.
	*/
	Tuple3f(const Tuple3f& other);

	/** Destructor that releases the allocated memory if needed. */
	~Tuple3f();

	/**
	* Returns the current X coordinate.
	*
	* @return the current X coordinate.
	*/
	inline float GetX() const { return this->x; };

	/**
	* Returns the current Y coordinate.
	*
	* @return the current Y coordinate.
	*/
	inline float GetY() const { return this->y; };

	/**
	* Returns the current Z coordinate
	*
	* @return the current Z coordinate.
	*/
	inline float GetZ() const { return this->z; };

	/** 
	* Computes the distance between two tuples. 
	* @param t1			The first tuple.
	* @param t2			The second tuple.
	*/
	static float distance(const Tuple3f t1, const Tuple3f t2);

	/**
	* Computes the squared distance between two tuples. 
	* This is useful for comparing distance since we do not need to compute the square root.
	* Note that: 
	*	distanceSquared(Tuple3f, Tuple3f) == distance(Tuple3f, Tuple3f) * distance(Tuple3f, Tuple3f)
	*
	* @param t1			The first tuple.
	* @param t2			The second tuple.
	*/
	static float distanceSquared(const Tuple3f t1, const Tuple3f t2);

	/**
	* The new definition for the ostream '<<' operator allowing to print the tuple into a stream.
	*
	* @param lhs		The reference to the output stream on the left side.
	* @param rhs		The reference to the tuple object on the right side.
	* @return			The reference to the updated stream.
	*/
	friend std::ostream& operator<< (std::ostream& lhs, const Tuple3f& rhs)
	{
		lhs << rhs.x << "," << rhs.y << "," << rhs.z;
		return lhs;
	}

	/**
	* The new definition for the '+' operator allowing to sum two tuples coordinates-wise.
	*
	* @param rhs		The reference to the tuple object on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The new tuple that is a result of this operator.
	*/
	Tuple3f operator+ (const Tuple3f& rhs)  const;

	Tuple3f& operator+= (const Tuple3f& rhs);

	/**
	* The new definition for the '-' operator allowing to deduct two tuples coordinates-wise.
	*
	* @param rhs		The reference to the number on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The new tuple that is a result of this operator.
	*/
	Tuple3f operator- (const Tuple3f& rhs)  const;

	/**
	* The new definition for the '*' operator allowing to multiply all coordinates by a number.
	*
	* @param rhs		The reference to the number the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The new tuple that is a result of this operator.
	*/
	Tuple3f operator* (const float& rhs) const;

	/**
	* The new definition for the '/' operator allowing to divide all coordinates by a number.
	*
	* @param rhs		The reference to the tuple object on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The new tuple that is a result of this operator.
	*/
	Tuple3f operator/ (const float& rhs) const;

	/**
	* The new definition for the '==' operator allowing to compare equality.
	*
	* @param rhs		The reference to the tuple object on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The {@p true} if this tuple is the same as the 'rhs' tuple; false otherwise.
	*/
	bool operator== (Tuple3f& rhs) const;

	/**
	* The new definition for the '!=' operator allowing to compare non-equality.
	*
	* @param rhs		The reference to the tuple object on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The {@p true} if this tuple not the same as the 'rhs' tuple; false otherwise.
	*/
	bool operator!= (Tuple3f& rhs) const;

	/**
	* The new definition for the '<' operator allowing us to define order on tuples.
	* It is also necessary for putting objects into the set container.
	* The tuple is smaller if x1 < x2 || x1 == x2 && y1 < y2) || (x == x && y == y && z < z)
	*
	* @param rhs		The reference to the tuple object on the right side of this operator. Note that the left side of this operator will be this object.
	* @return			The {@p true} if this tuple is smaller then the 'rhs' tuple; false otherwise.
	*/
	bool operator< (const Tuple3f& rhs) const;
};

