#include "BVHExample.h"

/////////////// Controls. ////////////////////////////////
// • Use the mouse + left button to rotate the view.
// • Use mouse + shift + left button to zoom in and out.
// • Use the LEFT and RIGHT arrows to select the active node(green).
// • Use UP and DOWN arrows to switch the depicted level of the hierarchy.
//		o If you press UP, the selected node will be picked, and its children will be depicted.
//		o If you press DOWN, the parent of the currently depicted nodes will be shown.
// • Use ‘v’ to toggle highlighting of trianglesand bounding volumes are potentially visible from the camera.
//		o Note that this may overdraw the green values in some cases as well as to show triangles that were previously hidden based on the displayed depth of the tree.
// • Use 'w, a, s, d' to rotate and 'q, e' to move the camera placed in the scene(denoted by the plane with violet arrow).The camera looks away from the plane in the direction of the arrow.
// • Use 'r' to reset the camera position.
// • Use 'g' to change the geometry that should be used for building the BVH tree.
///////////////////////////////////////////////////////////

/////////////// Useful methods and code tips. ////////////
///    Use node->setParent, node->setLeft, and node->setRight methods and their 'get' counterparts to build the tree.
///    Do not forget to use new operator when creating the nodes.
///    Use node->getTriangles() to obtain all triangles stored in the node.
///    Use dynamics_cast to check if the object is of the given instance - if(dynamic_cast<AABB*>(node)).
//////////////////////////////////////////////////////////

// Defines the raw file that will be loaded.
const string BVHExample::PATH = "models/dolphins.raw";

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
BVH* BVHExample::construct(unordered_set<Triangle*> triangles, int depth, VolumeType volumeType) const
{
	if (volumeType == VolumeType::AxisAlignedBoundingBox)
	{
		return new AABB(-1, -1, -1, 1, 1, 1, triangles);
	}
	else
	{
		return new BSV(0, 0, 0, 1, triangles);

	}
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
 * The most basic solution would be to brute-force the solution by simply testing all the triangles. Nevertheless, the goal of this assignment is to use the BVH tree you have constructed in the previous method to avoid testing all of them. To check how much you are saving, the framework will calculate the theoretical number of triangles that should be tested based on the visible volumes you returned. If you want to compare your own implementation with this value, you can also return the actual number of triangles you have tested in your algorithm via ‘testedTriangles’ parameter.
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
unordered_set<Triangle*> BVHExample::pvs(BVH* node, const Tuple3f cameraPosition, const Vector3f cameraNormal, const Vector3f cameraRightVector, const Vector3f cameraUpVector, int& testedTriangles, unordered_set<BVH*>& visibleVolumes) const
{
	if (AABB* aabb = dynamic_cast<AABB*>(node))
	{
		visibleVolumes.insert(node);
		testedTriangles = -1;
		return 	aabb->getTriangles();
	}
	else if (BSV* sbb = dynamic_cast<BSV*>(node))
	{
		visibleVolumes.insert(node);
		testedTriangles = -1;
		return sbb->getTriangles();
	}

	testedTriangles = -999;
	return unordered_set<Triangle*>();
}
