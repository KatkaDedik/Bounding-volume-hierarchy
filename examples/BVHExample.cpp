#include "BVHExample.h"


/////////////// Controls. ////////////////////////////////
// � Use the mouse + left button to rotate the view.
// � Use mouse + shift + left button to zoom in and out.
// � Use the LEFT and RIGHT arrows to select the active node(green).
// � Use UP and DOWN arrows to switch the depicted level of the hierarchy.
//		o If you press UP, the selected node will be picked, and its children will be depicted.
//		o If you press DOWN, the parent of the currently depicted nodes will be shown.
// � Use �v� to toggle highlighting of trianglesand bounding volumes are potentially visible from the camera.
//		o Note that this may overdraw the green values in some cases as well as to show triangles that were previously hidden based on the displayed depth of the tree.
// � Use 'w, a, s, d' to rotate and 'q, e' to move the camera placed in the scene(denoted by the plane with violet arrow).The camera looks away from the plane in the direction of the arrow.
// � Use 'r' to reset the camera position.
// � Use 'g' to change the geometry that should be used for building the BVH tree.
///////////////////////////////////////////////////////////

/////////////// Useful methods and code tips. ////////////
///    Use node->setParent, node->setLeft, and node->setRight methods and their 'get' counterparts to build the tree.
///    Do not forget to use new operator when creating the nodes.
///    Use node->getTriangles() to obtain all triangles stored in the node.
///    Use dynamics_cast to check if the object is of the given instance - if(dynamic_cast<AABB*>(node)).
//////////////////////////////////////////////////////////

// Defines the raw file that will be loaded.
//const string BVHExample::PATH = "models/armadillo.raw";
//const string BVHExample::PATH = "models/beethoven.raw";
//const string BVHExample::PATH = "models/bunny.raw";
//const string BVHExample::PATH = "models/camel.raw";
//const string BVHExample::PATH = "models/car.raw";
//const string BVHExample::PATH = "models/cow.raw";
//const string BVHExample::PATH = "models/cow_deform2.raw";
//const string BVHExample::PATH = "models/dolphins.raw";
//const string BVHExample::PATH = "models/horse.raw";
//const string BVHExample::PATH = "models/octopus.raw";
//const string BVHExample::PATH = "models/roseinvase.raw";
//const string BVHExample::PATH = "models/scene.raw";
//const string BVHExample::PATH = "models/sphere.raw";
//const string BVHExample::PATH = "models/subs.raw";
//const string BVHExample::PATH = "models/torus.raw";
const string BVHExample::PATH = "models/womanhead.raw";

/**
* @param point - point to be compared with current min and max
* @param curentMM - current min and max (minX, maxX, minY, maxY, minZ, maxZ)
*
* @return - nothing, currentMM are change if point has bigger extreme
**/
void comparePoint(const Tuple3f & point, std::tuple<float, float, float, float, float, float> & currentMM)
{
	if (point.GetX() < std::get<0>(currentMM)) {
		std::get<0>(currentMM) = point.GetX();
	}
	if (point.GetX() > std::get<1>(currentMM)) {
		std::get<1>(currentMM) = point.GetX();
	}
	if (point.GetY() < std::get<2>(currentMM)) {
		std::get<2>(currentMM) = point.GetY();
	}
	if (point.GetY() > std::get<3>(currentMM)) {
		std::get<3>(currentMM) = point.GetY();
	}
	if (point.GetZ() < std::get<4>(currentMM)) {
		std::get<4>(currentMM) = point.GetZ();
	}
	if (point.GetZ() > std::get<5>(currentMM)) {
		std::get<5>(currentMM) = point.GetZ();
	}
}

/**
* @param triangles - set to find min and max x, y, z
*
* @return - tuple with min and max (minX, maxX, minY, maxY, minZ, maxZ)
**/
const std::tuple<float, float, float, float, float, float> findMinsAndMax(const unordered_set<Triangle*>& triangles) 
{
	//initialize min and max
	float minX = (*triangles.begin())->v1.GetX();
	float maxX = (*triangles.begin())->v1.GetX();
	float minY = (*triangles.begin())->v1.GetY();
	float maxY = (*triangles.begin())->v1.GetY();
	float minZ = (*triangles.begin())->v1.GetZ();
	float maxZ = (*triangles.begin())->v1.GetZ();

	auto ret = std::make_tuple(minX, maxX, minY, maxY, minZ, maxZ);

	//find actual min and max
	for (Triangle* triangle : triangles)
	{	
		//check 1st point
		comparePoint(triangle->v1, ret);

		//check 2nd point
		comparePoint(triangle->v2, ret);

		//check 3rd point
		comparePoint(triangle->v3, ret);
	}
	return ret;
}

/**
* @param vertex1 - start vertex
* @param triangles - set to find the furthest vertex from vertex1
* 
* @return - furthest vertex from vertex1
**/
const Tuple3f findFurthestVertex(const Tuple3f vertex1, const unordered_set<Triangle*>& triangles)
{
	Tuple3f vertex2 = (*triangles.begin())->v1;
	float maxDistance = vertex1.distance(vertex1, vertex2);

	for (auto triangle : triangles)
	{
		float distance = vertex1.distance(vertex1, triangle->v1);
		if (maxDistance < distance) 
		{
			vertex2 = triangle->v1;
			maxDistance = distance;
		}

		distance = vertex1.distance(vertex1, triangle->v2);
		if (maxDistance < distance)
		{
			vertex2 = triangle->v2;
			maxDistance = distance;
		}

		distance = vertex1.distance(vertex1, triangle->v3);
		if (maxDistance < distance)
		{
			vertex2 = triangle->v3;
			maxDistance = distance;
		}
	}
	return vertex2;
}

/**
* this function combines Ritter's bounding sphere and making bounding spheres from "AABB"... because sometimes RBS doesn't compute
* the center very well, and in case that "AABB" sphere is smaller, program uses that.
* 
* @param triangles - set of triangles
*
* @return - std::tuple<center, riadial>
**/
const std::tuple<Tuple3f, float> computeSphere(const unordered_set<Triangle*>& triangles)
{
	//compute bounding sphere from "AABB" box
	auto box = findMinsAndMax(triangles);
	Tuple3f boxCenter(
		(std::get<0>(box) + std::get<1>(box)) / 2,
		(std::get<2>(box) + std::get<3>(box)) / 2,
		(std::get<3>(box) + std::get<4>(box)) / 2);
	Tuple3f m = findFurthestVertex(boxCenter, triangles);
	float boxRadius = m.distance(m, boxCenter);

	//Ritter's bounding sphere
	Tuple3f vertex1 = (*triangles.begin())->v1;
	Tuple3f vertex2 = findFurthestVertex(vertex1, triangles);
	vertex1 = findFurthestVertex(vertex2, triangles);

	Tuple3f center(
		(vertex1.GetX() + vertex2.GetX()) / 2, 
		(vertex1.GetY() + vertex2.GetY()) / 2, 
		(vertex1.GetZ() + vertex2.GetZ()) / 2);
	float minDistance = vertex1.distance(vertex1, vertex2) / 2;

	//make sphere bigger, if some vertex has bigger distance from center than current radius
	for (auto triangle : triangles) {
		float distance = center.distance(center, triangle->v1);
		if (minDistance < distance)
		{
			minDistance = distance;
		}

		distance = vertex1.distance(center, triangle->v2);
		if (minDistance < distance)
		{
			minDistance = distance;
		}

		distance = vertex1.distance(center, triangle->v3);
		if (minDistance < distance)
		{
			minDistance = distance;
		}
	}

	//compere witch sphere is smaller
	if (minDistance < boxRadius) {
		return std::make_tuple(center, minDistance);
	}
	else {
		return std::make_tuple(boxCenter, boxRadius);
	}
}

/*
* @param AABB - bounding box
*
* @return - std::tuple<which axis, where>
*			0 - cut x axis
*			1 - cut y axis
*			2 - cut z axis
**/
std::tuple<int, float> howShouldICut(const AABB & parent)
{
	Tuple3f min = parent.getMin();
	Tuple3f max = parent.getMax();

	float xAxisSize = max.x - min.x;
	float yAxisSize = max.y - min.y;
	float zAxisSize = max.z - min.z;

	int axisIndex = 0;
	float axisPosition = min.x + xAxisSize / 2;

	if (yAxisSize > xAxisSize)
	{

		if (zAxisSize > yAxisSize)
		{
			axisIndex = 2;
			axisPosition = min.z + zAxisSize / 2;
		}
		else
		{
			axisIndex = 1;
			axisPosition = min.y + yAxisSize / 2;
		}
	}
	else if (zAxisSize > xAxisSize)
	{
		axisIndex = 2;
		axisPosition = min.z + zAxisSize / 2;
	}
	return std::make_tuple(axisIndex, axisPosition);
}

std::tuple<int, float> howShouldICut(const BSV& parent)
{
	auto minmax = findMinsAndMax(parent.triangles);

	float xAxisSize = std::get<1>(minmax) - std::get<0>(minmax);
	float yAxisSize = std::get<3>(minmax) - std::get<2>(minmax);
	float zAxisSize = std::get<5>(minmax) - std::get<4>(minmax);

	int axisIndex = 0;
	float axisPosition = parent.getCenter().x;

	if (yAxisSize > xAxisSize)
	{

		if (zAxisSize > yAxisSize)
		{
			axisIndex = 2;
			axisPosition = parent.getCenter().z;
		}
		else
		{
			axisIndex = 1;
			axisPosition = parent.getCenter().y;
		}
	}
	else if (zAxisSize > xAxisSize)
	{
		axisIndex = 2;
		axisPosition = parent.getCenter().z;
	}
	return std::make_tuple(axisIndex, axisPosition);
}


std::tuple<unordered_set<Triangle*>, unordered_set<Triangle*>> cutModel(BVH & parent)
{
	std::tuple<int, float> cuttingPosition;
	if (AABB* aabb = dynamic_cast<AABB*>(&parent))
	{
		cuttingPosition = howShouldICut(*aabb);
	}
	else if (BSV* bsv = dynamic_cast<BSV*>(&parent)) 
	{
		cuttingPosition = howShouldICut(*bsv);
	}

	int axisIndex = std::get<0>(cuttingPosition);
	float axisPosition = std::get<1>(cuttingPosition);

	unordered_set<Triangle*> leftSet;
	unordered_set<Triangle*> rightSet;

	for(auto triangle : parent.getTriangles())
	{
		switch (axisIndex)
		{
		case(0):
			if (triangle->v1.x > axisPosition || triangle->v2.x > axisPosition || triangle->v3.x > axisPosition)
			{
				rightSet.insert(triangle);
			}
			if (triangle->v1.x < axisPosition || triangle->v2.x < axisPosition || triangle->v3.x < axisPosition)
			{
				leftSet.insert(triangle);
			}
			break;
		case(1):
			if (triangle->v1.y > axisPosition || triangle->v2.y > axisPosition || triangle->v3.y > axisPosition)
			{
				rightSet.insert(triangle);
			}
			if (triangle->v1.y < axisPosition || triangle->v2.y < axisPosition || triangle->v3.y < axisPosition)
			{
				leftSet.insert(triangle);
			}
			break;
		case(2):
			if (triangle->v1.z > axisPosition || triangle->v2.z > axisPosition || triangle->v3.z > axisPosition)
			{
				rightSet.insert(triangle);
			}
			if (triangle->v1.z < axisPosition || triangle->v2.z < axisPosition || triangle->v3.z < axisPosition)
			{
				leftSet.insert(triangle);
			}
			break;
		default:
			break;
		}
	}
	return std::make_tuple(leftSet, rightSet);
}

/**
 * This method will construct a binary bounding volume hierarchy (BVH) tree from the set of triangles of the given depth.
 * The geometry that should be used to build the tree is defined by the volumeType parameter and can be either axis-aligned bounding box or sphere.
 * You will get 15 points if you implement this method for one of the volume types or 20 points if your implementation supports both.
 *
 * The method should return the root of the BVH tree.
 * Each node in the tree is represented by an instance of either AABB (axis-aligned bounding box) or BSV (bounding sphere volume) class.
 * Note that both AABB and BSV are inheriting from the BVH class.
 * Make sure that you are properly set up the children and parents for each node; otherwise, the skeleton will not be able to visualize the tree correctly.
 * Also make sure you are assigning correct triangles that are inside the bounding volume represented by each node.
 *
 * @param triangles - The set of triangles.
 * @param depth - The maximum depth the binary tree should have.
 * @param volumeType - The flag determining the requested bounding volume.
 */
BVH* BVHExample::construct(const unordered_set<Triangle*> & triangles, int depth, VolumeType volumeType) const
{
	if (depth == 0)
	{
		return nullptr;
	}

	if (volumeType == VolumeType::AxisAlignedBoundingBox)
	{

		auto borders = findMinsAndMax(triangles);

		AABB *box = new AABB(std::get<0>(borders), std::get<2>(borders), std::get<4>(borders),
			std::get<1>(borders), std::get<3>(borders), std::get<5>(borders), triangles);

		auto children = cutModel(*box);

		auto leftChild = construct(std::get<0>(children), depth - 1, volumeType);
		auto rightChild = construct(std::get<1>(children), depth - 1, volumeType);
		
		if (depth != 1)
		{
			box->setLeft(leftChild);
			box->setRight(rightChild);

			leftChild->setParent(box);
			rightChild->setParent(box);
		}
		
		return box;
	}
	else
	{
		auto sphereTuple =computeSphere(triangles);

		BSV* sphere = new BSV(std::get<0>(sphereTuple).x, std::get<0>(sphereTuple).y, std::get<0>(sphereTuple).z,
			std::get<1>(sphereTuple) , triangles);

		auto children = cutModel(*sphere);

		auto leftChild = construct(std::get<0>(children), depth - 1, volumeType);
		auto rightChild = construct(std::get<1>(children), depth - 1, volumeType);

		if (depth != 1)
		{
			sphere->setLeft(leftChild);
			sphere->setRight(rightChild);

			leftChild->setParent(sphere);
			rightChild->setParent(sphere);
		}

		return sphere;
	}
}

bool isVertexVisible(const Tuple3f& vertex, const Tuple3f& cameraPosition, Vector3f cameraNormal)
{
	Vector3f v(cameraPosition.GetX() - vertex.GetX(), cameraPosition.GetY() - vertex.GetY(), cameraPosition.GetZ() - vertex.GetZ());
	return cameraNormal.Dot(v) <= 0.000001f;
}

/**
 * @return -1 if box is not visible
 *		   0 if box is partialy visible
 *		   1 if box is visible
 **/
int isBoxVisible(const AABB& box, const Tuple3f& cameraPosition, const Vector3f & cameraNormal)
{
	Tuple3f max = box.getMax();
	Tuple3f min = box.getMin();

	Vector3f radial1(max.GetX() - min.GetX(), max.GetY() - min.GetY(), max.GetZ() - min.GetZ());
	Vector3f radial2(min.GetX() - max.GetX(), max.GetY() - min.GetY(), max.GetZ() - min.GetZ());
	Vector3f radial3(max.GetX() - min.GetX(), min.GetY() - max.GetY(), max.GetZ() - min.GetZ());
	Vector3f radial4(min.GetX() - max.GetX(), min.GetY() - max.GetY(), max.GetZ() - min.GetZ());

	float maxDotProduct = std::abs(cameraNormal.Dot(radial1));
	float dotProduct2 = std::abs(cameraNormal.Dot(radial2));
	float dotProduct3 = std::abs(cameraNormal.Dot(radial3));
	float dotProduct4 = std::abs(cameraNormal.Dot(radial4));

	Tuple3f toCheckVertex1 = max;
	Tuple3f toCheckVertex2 = min;

	if (maxDotProduct < dotProduct2)
	{
		maxDotProduct = dotProduct2;
		toCheckVertex1 = Tuple3f(min.GetX(), max.GetY(), max.GetZ());
		toCheckVertex2 = Tuple3f(max.GetX(), min.GetY(), min.GetZ());
	}
	if (maxDotProduct < dotProduct3)
	{
		maxDotProduct = dotProduct3;
		toCheckVertex1 = Tuple3f(max.GetX(), min.GetY(), max.GetZ());
		toCheckVertex2 = Tuple3f(min.GetX(), max.GetY(), min.GetZ());
	}
	if (maxDotProduct < dotProduct4)
	{
		maxDotProduct = dotProduct4;
		toCheckVertex1 = Tuple3f(min.GetX(), min.GetY(), max.GetZ());
		toCheckVertex2 = Tuple3f(max.GetX(), max.GetY(), min.GetZ());
	}

	int ret = -1;
	if (isVertexVisible(toCheckVertex1, cameraPosition, cameraNormal)) {
		ret++;
	}
	if (isVertexVisible(toCheckVertex2, cameraPosition, cameraNormal)) {
		ret++;
	}
	return ret;
}

int isSphereVisible(const BSV& sphere, const Tuple3f& cameraPosition, const Vector3f& cameraNormal) 
{
	int ret = -1;
	if (isVertexVisible(sphere.getCenter(), cameraPosition, cameraNormal)) 
	{
		ret++;
		if (isVertexVisible(sphere.getCenter() - cameraNormal, cameraPosition, cameraNormal)) {
			ret++;
		}
	}
	else 
	{
		if (isVertexVisible(sphere.getCenter() + cameraNormal, cameraPosition, cameraNormal)) {
			ret++;
		}
	}
	return ret;
}

/**
 * This method will return all possibly visible triangles from the current camera (assuming orthographic projection).
 * In other words, the method will return all triangles that have at least one vertex in a half-space defined by a plane.
 * The plane is given by a point (cameraPosition) and normal (cameraNormal).
 * Additionally, you have also access to two vectors defining the exact camera rotation (cameraRightVector and cameraUpVector) if needed.
 * These two vectors are orthogonal to each other and form the basis of the plane.
 *
 * You have to return the triangles that have at least one vertex on the side of the plane where the normal points to.
 * Additionally, you also have to return all the bounding boxes that are partially visible from the camera (via visible volumes parameter).
 *
 * The most basic solution would be to brute-force the solution by simply testing all the triangles. Nevertheless, the goal of this assignment is to use the BVH tree you have constructed in the previous method to avoid testing all of them. To check how much you are saving, the framework will calculate the theoretical number of triangles that should be tested based on the visible volumes you returned. If you want to compare your own implementation with this value, you can also return the actual number of triangles you have tested in your algorithm via �testedTriangles� parameter.
 * Note that the PVS method only accepts the generic BVH instance on the input.
 * Hence you will have to determine the type of the node yourself in the code.
 * This can be done using dynamic_cast in C++ or instanceof in Java (see the code in the skeleton for more details).
 *
 * Again, you will get 15 points if you implement this method for one of the volume types or 20 points if your implementation supports both.
 *
 *
 * @param node - The root of the tree.
 * @param cameraPosition - The position of the camera.
 * @param cameraNormal - The normal of the camera plane, i.e., the direction in which the camera is pointing.
 * @param cameraRightVector - Defines the vector point to the right side of the camera. It is placesOne of the two vectors in the camera plane - it is orthogonal to cameraUpVector.
 * @param cameraUpVector - Second of the two vectors in the camera plane - it is orthogonal to cameraRightVector.
 * @param testedTriangles - At the end of the method this variable should contain the number of actually tested triangles.
 * @param visibleVolumes - At the end of the method this set should contain all volumes that the camera sees.
 *
 * @return The method will return all triangles that are visible from the camera
 */
unordered_set<Triangle*> BVHExample::pvs(BVH* node, const Tuple3f cameraPosition, const Vector3f cameraNormal, 
	const Vector3f cameraRightVector, const Vector3f cameraUpVector, 
	int& testedTriangles, unordered_set<BVH*>& visibleVolumes) const
{
	int visibilityCheck = -1;
	if (AABB* aabb = dynamic_cast<AABB*>(node))
	{
		visibilityCheck = isBoxVisible(*aabb, cameraPosition, cameraNormal);
	}
	else if (BSV* sbb = dynamic_cast<BSV*>(node))
	{
		visibilityCheck = isSphereVisible(*sbb, cameraPosition, cameraNormal);
	}
	unordered_set<Triangle*> visible;

	switch (visibilityCheck)
	{
	case(-1):
		return unordered_set<Triangle*>();
		break;
	case(0):
		visibleVolumes.insert(node);
		if (node->isLeaf())
		{
			for (auto triangle : node->getTriangles())
			{
				testedTriangles++;
				if (isVertexVisible(triangle->v1, cameraPosition, cameraNormal) ||
					isVertexVisible(triangle->v2, cameraPosition, cameraNormal) ||
					isVertexVisible(triangle->v3, cameraPosition, cameraNormal))
				{
					visible.insert(triangle);
				}
			}
		}
		else
		{
			auto leftVisible = pvs(node->getLeft(), cameraPosition, cameraNormal, cameraRightVector, cameraUpVector, testedTriangles, visibleVolumes);
			visible.insert(leftVisible.begin(), leftVisible.end());
			auto rightVisible = pvs(node->getRight(), cameraPosition, cameraNormal, cameraRightVector, cameraUpVector, testedTriangles, visibleVolumes);
			visible.insert(rightVisible.begin(), rightVisible.end());
		}
		return visible;
		break;
	case(1):
		visibleVolumes.insert(node);
		return node->getTriangles();
		break;
	default:
		break;
	}

	return node->getTriangles();
}
