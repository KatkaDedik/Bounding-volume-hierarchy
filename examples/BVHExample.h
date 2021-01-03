#pragma once
#include "../core/BaseWindow.h"
#include <tuple>
#include <cmath>
#include <random>
#include <sstream>
#include <fstream>
#include "../core/Image.h"
#include "../vecmath/Triangle.h"
#include "../vecmath/Vector3f.h"
#include <unordered_set>

/**
 * The base class defining the shared functionality for all bounding volume hierarchy (BVH) trees.
 * The tree is binary and each instance of BVH class represents a single node in the tree.
 */
class BVH
{

private:

	/** The set of triangles in the node. */
	unordered_set<Triangle*> triangles;
	/** The point to the parent node (if any). */
	BVH* parent = nullptr;
	/** The point to the left node (if any). */
	BVH* left = nullptr;
	/** The point to the right node (if any). */
	BVH* right = nullptr;

public:

	/** Constructs a new node in the tree encapsulating the given set of triangle. */
	BVH(unordered_set<Triangle*> triangles) : triangles(triangles)
	{
	}

	/** Renders the node with a given color. The implementation is left to the child classes. */
	virtual void render(Color color, GLfloat matrix[4][4]) = 0;

	/** Returns the depth of the node in the whole tree. */
	int getDepth() const
	{
		int depth = 0;
		const BVH* current = this;
		while (current->getParent() != nullptr)
		{
			depth++;
			current = current->getParent();
		}
		return depth;
	}

	/** Returns the triangles stored in the node. */
	unordered_set<Triangle*>& getTriangles()
	{
		return triangles;
	}

	/** Returns the parent node. */
	BVH* getParent() const
	{
		return parent;
	}

	/** Sets a new parent node. */
	void setParent(BVH* parent)
	{
		this->parent = parent;
	}

	/** Returns the left node. */
	BVH* getLeft() const
	{
		return left;
	}

	/** Sets a new left node. This method also automatically sets parent node in the specified node to this. */
	void setLeft(BVH* left)
	{
		this->left = left;
		if (left != nullptr)
			left->setParent(this);
	}

	/** Returns the right node. */
	BVH* getRight() const
	{
		return right;
	}

	/** Sets a new right node. This method also automatically sets parent node in the specified node to this. */
	void setRight(BVH* right)
	{
		this->right = right;
		if (right != nullptr)
			right->setParent(this);
	}

	/** Checks if the node is a leaf. */
	bool isLeaf()
	{
		return this->right == nullptr && this->left == nullptr;
	}
};

/** The axis aligned bounding box implementation of the BVH node. */
class AABB : public BVH
{

private:

	/** The point on the bounding box with minimum values in all axes (x,y,z). */
	Tuple3f min;
	/** The point on the bounding box with maximum values in all axes (x,y,z). */
	Tuple3f max;

public:

	/** Constructs a new AABB node from the specified values. */
	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, unordered_set<Triangle*> triangles) : AABB(Tuple3f(minX, minY, minZ), Tuple3f(maxX, maxY, maxZ), triangles)
	{
	}

	/** Constructs a new AABB node from the specified values. */
	AABB(Tuple3f min, Tuple3f max, unordered_set<Triangle*> triangles) : min(min), max(max), BVH(triangles)
	{
	}

	/** Returns the minimum point on the bounding box. */
	Tuple3f getMin() const
	{
		return min;
	}

	/** Returns the maximum point on the bounding box. */
	Tuple3f getMax() const
	{
		return max;
	}

	/** Sets a new minimum node for this node. */
	void setMin(const Tuple3f min)
	{
		this->min = min;
	}

	/** Sets a new maximum point for this node. */
	void setMax(const Tuple3f max)
	{
		this->max = max;
	}

	/** Renders the node with a specified color. */
	void render(Color color, GLfloat matrix[4][4]) {
		glColor3f(color.r, color.g, color.b);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		//FRONT
		glVertex3f(min.x, min.y, max.z);
		glVertex3f(max.x, min.y, max.z);
		glVertex3f(max.x, max.y, max.z);
		glVertex3f(min.x, max.y, max.z);
		// BACK
		glVertex3f(min.x, min.y, min.z);
		glVertex3f(max.x, min.y, min.z);
		glVertex3f(max.x, max.y, min.z);
		glVertex3f(min.x, max.y, min.z);
		// TOP
		glVertex3f(min.x, min.y, min.z);
		glVertex3f(min.x, min.y, max.z);
		glVertex3f(max.x, min.y, max.z);
		glVertex3f(max.x, min.y, min.z);
		// BOTTOM
		glVertex3f(min.x, max.y, min.z);
		glVertex3f(min.x, max.y, max.z);
		glVertex3f(max.x, max.y, max.z);
		glVertex3f(max.x, max.y, min.z);
		// LEFT
		glVertex3f(min.x, min.y, min.z);
		glVertex3f(min.x, max.y, min.z);
		glVertex3f(min.x, max.y, max.z);
		glVertex3f(min.x, min.y, max.z);
		// RIGHT
		glVertex3f(max.x, min.y, min.z);
		glVertex3f(max.x, max.y, min.z);
		glVertex3f(max.x, max.y, max.z);
		glVertex3f(max.x, min.y, max.z);
		glEnd();
	}
};

/** The bounding sphere implementation of the BVH node. */
class BSV : public BVH
{

private:

	/** The center of the bounding sphere. */
	Tuple3f center;
	/** The radius of the bounding sphere. */
	float radius;

public:

	/** Constructs a new SBB node from the specified values. */
	BSV(float x, float y, float z, float radius, unordered_set<Triangle*> triangles) : BSV(Tuple3f(x, y, z), radius, triangles)
	{
	}

	/** Constructs a new SBB node from the specified values. */
	BSV(Tuple3f center, float radius, unordered_set<Triangle*> triangles) : center(center), radius(radius), BVH(triangles)
	{
	}

	/** Returns the center of the bounding sphere. */
	Tuple3f getCenter() const
	{
		return center;
	}

	/** Returns the radius of the bounding sphere. */
	float getRadius() const
	{
		return radius;
	}

	/**
	 * Sets a new center for this node.
	 */
	void setCenter(Tuple3f center) {
		this->center = center;
	}

	/** Sets a new radius for this node. */
	void setRadius(float radius) {
		this->radius = radius;
	}

	/** Renders the node with a specified color. */
	void render(Color color, GLfloat matrix[4][4]) {
		glColor3f(color.r, color.g, color.b);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		int num_segments = 50;

		glPushMatrix();
		GLfloat transpose[4][4];
		transpose[0][0] = matrix[0][0];
		transpose[1][0] = matrix[0][1];
		transpose[2][0] = matrix[0][2];
		transpose[3][0] = matrix[0][3];

		transpose[0][1] = matrix[1][0];
		transpose[1][1] = matrix[1][1];
		transpose[2][1] = matrix[1][2];
		transpose[3][1] = matrix[1][3];

		transpose[0][2] = matrix[2][0];
		transpose[1][2] = matrix[2][1];
		transpose[2][2] = matrix[2][2];
		transpose[3][2] = matrix[2][3];

		transpose[0][3] = matrix[3][0];
		transpose[1][3] = matrix[3][1];
		transpose[2][3] = matrix[3][2];
		transpose[3][3] = matrix[3][3];

		glTranslatef(center.x, center.y, center.z);

		/*glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments + 1; ii++) {
			float theta = 2.0f * 3.1415926f * (float)ii / (float)num_segments;

			float u = radius * (float)cos(theta);
			float v = radius * (float)sin(theta);
			glVertex3f(u, v, 0);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments + 1; ii++) {
			float theta = 2.0f * 3.1415926f * (float)ii / (float)num_segments;

			float u = radius * (float)cos(theta);
			float v = radius * (float)sin(theta);
			glVertex3f(u, 0, v);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments + 1; ii++) {
			float theta = 2.0f * 3.1415926f * (float)ii / (float)num_segments;

			float u = radius * (float)cos(theta);
			float v = radius * (float)sin(theta);
			glVertex3f(0, u, v);
		}
		glEnd();*/

		glMultMatrixf(&transpose[0][0]);

		glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments + 1; ii++) {
			float theta = 2.0f * 3.1415926f * (float)ii / (float)num_segments;

			float u = radius * (float)cos(theta);
			float v = radius * (float)sin(theta);
			glVertex3f(u, v, 0);
		}
		glEnd();

		glPopMatrix();
	}
};

enum VolumeType
{
	AxisAlignedBoundingBox, Sphere
};

/**
 * The example for experimenting with BVH.
 * Code for handling interactions and rendering of the window is in this header.
 * The *.cpp file is designated for the implementation of the actual algorithms by students.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class BVHExample : public BaseWindow
{

private:

	/** The set of triangles defining the geometry. */
	unordered_set<Triangle*> geometry;
	/** The path to the raw file from which the geometry will be loaded. */
	static const string PATH;
	/** The flag determining whether to use AxisAlignedBoundBox or Spheres for building the BVH tree. */
	VolumeType volumeType = VolumeType::AxisAlignedBoundingBox;

	/** The set of volumes that were tested during the tracing of the BVH tree. */
	unordered_set<BVH*> visibleVolumes;
	/** The set of visible triangles that were. */
	unordered_set<Triangle*> visibleTriangles;
	/** The number of triangles in visible bounding boxes (at the lowest level). */
	int trianglesInVolumes = 0;
	/** The actual number of triangles student returned. */
	int testedTriangles = 0;

	/** The root of the BVH tree. */
	BVH* root;
	/** The currently selected node in the BVH tree. */
	BVH* current;
	/** The currently displayed level of the hierarchy */
	int displayLevel = 0;
	/** The maximum depth of the tree. */
	int maxDepth = 4;

	/** If true the visible triangles will be highlighted. */
	bool highlightVisible = true;
	/** The flag determining if the visible triangles needs to be recomputed. */
	bool dirty = true;

	/** The current camera position. */
	Tuple3f cameraPosition = Vector3f(0, 0, 0);
	/** The current left vector of the camera. */
	Vector3f cameraX = Vector3f(1, 0, 0);
	/** The current up vector of the camera. */
	Vector3f cameraY = Vector3f(0, 1, 0);
	/** The current direction vector of the camera. */
	Vector3f cameraZ = Vector3f(0, 0, 1);
public:

	/** Creates the new example window and loads the geometry.  */
	BVHExample()
	{
		geometry = load();
		init();
	}

	/** Releases allocated memory on example destruction. */
	~BVHExample()
	{
		for (unordered_set<Triangle*>::iterator it = geometry.begin(); it != geometry.end(); it++)
		{
			delete* it;
		}
		geometry.clear();
		deleteTree(root);
	}

	/** Deletes the allocated memory for the tree. */
	static void deleteTree(BVH* node)
	{
		if (node != nullptr) {
			if (node->getLeft() != nullptr)
				deleteTree(node->getLeft());
			if (node->getRight() != nullptr)
				deleteTree(node->getRight());
			delete node;
		}
	}

private:

	/** The method initializes the visualization and constructs the BVH tree. */
	void init()
	{
		root = construct(geometry, maxDepth, volumeType);
		current = root;
		displayLevel = 0;
		highlightVisible = false;
		dirty = true;
	}

protected:
	/**
	 * Invoked when a key is pressed.
	 *
	 * @param	key	The key in ASCII.
	 * @param	x  	The X coordinate of mouse location in window relative coordinates when the key was pressed.
	 * @param	y  	The Y coordinate of mouse location in window relative coordinates when the key was pressed.
	 */
	void keyboard(unsigned char key, int x, int y) override
	{
		BaseWindow::keyboard(key, x, y);
		const float step = 0.1f;
		switch (key)
		{
		case 'v':
			highlightVisible = !highlightVisible;
			break;
		case 'g':
			if (volumeType == VolumeType::AxisAlignedBoundingBox) {
				volumeType = VolumeType::Sphere;
			}
			else
			{
				volumeType = VolumeType::AxisAlignedBoundingBox;
			}
			init();
		case 'r':
			cameraX = Vector3f(1, 0, 0);
			cameraY = Vector3f(0, 1, 0);
			cameraZ = Vector3f(0, 0, 1);
			cameraPosition = Vector3f(0, 0, 0);
			dirty = true;
			break;
		case 'w':
			cameraX = cameraX.Rotate(step, cameraY);
			cameraX.Normalize();
			cameraZ = cameraX.Cross(cameraY);
			cameraZ.Normalize();
			dirty = true;
			break;
		case 's':
			cameraX = cameraX.Rotate(-step, cameraY);
			cameraX.Normalize();
			cameraZ = cameraX.Cross(cameraY);
			cameraZ.Normalize();
			dirty = true;
			break;
		case 'a':
			cameraY = cameraY.Rotate(step, cameraX);
			cameraY.Normalize();
			cameraZ = cameraX.Cross(cameraY);
			cameraZ.Normalize();
			dirty = true;
			break;
		case 'd':
			cameraY = cameraY.Rotate(-step, cameraX);
			cameraY.Normalize();
			cameraZ = cameraX.Cross(cameraY);
			cameraZ.Normalize();
			dirty = true;
			break;
		case 'q':
			cameraPosition.x += cameraZ.x * step;
			cameraPosition.y += cameraZ.y * step;
			cameraPosition.z += cameraZ.z * step;
			dirty = true;
			break;
		case 'e':
			cameraPosition.x -= cameraZ.x * step;
			cameraPosition.y -= cameraZ.y * step;
			cameraPosition.z -= cameraZ.z * step;
			dirty = true;
			break;
		}
	}

	/**
	 * Invoked when a special input (i.e., pressing arrows) was passed to the GLUT.
	 *
	 * @param	key	The special key that was pressed.
	 * @param	x	The X coordinate of the current mouse location.
	 * @param	y	The Y coordinate of the current mouse location.
	 */
	void specialInput(int key, int x, int y) override
	{
		BaseWindow::specialInput(key, x, y);
		switch (key)
		{
		case GLUT_KEY_DOWN:

			if (current->getLeft() != nullptr)
			{
				current = current->getLeft();
				displayLevel++;
			}
			highlightVisible = false;
			break;
		case GLUT_KEY_UP:
			if (current->getParent() != nullptr)
			{
				current = current->getParent();
				displayLevel--;
			}
			highlightVisible = false;
			break;
		case GLUT_KEY_LEFT:
			if (current->getParent() != nullptr)
			{
				current = current->getParent()->getLeft();
			}
			break;
		case GLUT_KEY_RIGHT:
			if (current->getParent() != nullptr)
			{
				current = current->getParent()->getRight();
			}
			break;
		}
	}

	/** Loads the geometry from the raw file defined by PATH. */
	unordered_set<Triangle*> load()
	{
		ifstream file(PATH);
		vector<float> raw;
		if (file.is_open())
		{
			float vertex;
			while (file >> vertex) {
				raw.push_back(vertex);
			}
		}

		if (raw.size() % 3 != 0)
		{
			cout << "WARNING: some vertices are missing.";
		}

		Tuple3f boundingMin = Tuple3f(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
		Tuple3f boundingMax = Tuple3f(numeric_limits<float>::min(), numeric_limits<float>::min(), numeric_limits<float>::min());
		for (size_t i = 0; i < raw.size(); i += 3) {
			boundingMin.x = std::min(boundingMin.x, raw[i]);
			boundingMin.y = std::min(boundingMin.y, raw[i + 1]);
			boundingMin.z = std::min(boundingMin.z, raw[i + 2]);

			boundingMax.x = std::max(boundingMax.x, raw[i]);
			boundingMax.y = std::max(boundingMax.y, raw[i + 1]);
			boundingMax.z = std::max(boundingMax.z, raw[i + 2]);
		}
		float dist = std::max({ boundingMax.x - boundingMin.x, boundingMax.y - boundingMin.y, boundingMax.z - boundingMin.z });

		const float scale = 2.f / dist;
		const Tuple3f center = (boundingMin + boundingMax) / 2;

		unordered_set<Triangle*> triangles;

		for (size_t i = 0; i < raw.size(); i += 9) {
			const Tuple3f v1 = (Tuple3f(raw[i], raw[i + 1], raw[i + 2]) - center) * scale;
			const Tuple3f v2 = (Tuple3f(raw[i + 3], raw[i + 4], raw[i + 5]) - center) * scale;
			const Tuple3f v3 = (Tuple3f(raw[i + 6], raw[i + 7], raw[i + 8]) - center) * scale;

			triangles.insert(new Triangle(v1, v2, v3));
		}

		// center the min and max for display
		boundingMin = (boundingMin - center) * scale;
		boundingMax = (boundingMax - center) * scale;

		return triangles;
	}

	/** Helper method that renders a given text on the screen. */
	void displayText(const float x, const float y, const int r, const int g, const int b, const char* string) {
		int j = strlen(string);

		glColor3f(r, g, b);
		glRasterPos2f(x, y);
		for (int i = 0; i < j; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	}

	/** The main render loop. */
	void render() override
	{
		BaseWindow::render();

		////////////// LEFT //////////////
		glViewport(0, 0, this->width / 2.0f, this->height);
		glPushMatrix();
		// obtains the visible triangles and volumes
		if (dirty) {
			visibleVolumes.clear();
			trianglesInVolumes = 0;
			testedTriangles = 0;
			visibleTriangles = pvs(root, cameraPosition, cameraZ, cameraX, cameraY, testedTriangles, visibleVolumes);
			dirty = false;

			for (auto volume : visibleVolumes)
			{
				if (volume->isLeaf())
				{
					trianglesInVolumes += volume->getTriangles().size();
				}
			}
		}

		renderCurrentLevel();
		renderCamera();
		if (highlightVisible)
		{
			renderVisibleTriangles();
		}
		glPopMatrix();

		////////////// RIGHT //////////////
		glViewport(this->width / 2.0f, 0, this->width / 2.0f, this->height);

		glPushMatrix();
		glLoadIdentity();

		float inf = 9999999;
		gluLookAt(
			cameraPosition.x /*- cameraZ.x*/, cameraPosition.y /*- cameraZ.y*/, cameraPosition.z /*- cameraZ.z*/, // -cameraZ for the frustum clipping plane
			cameraZ.x * inf, cameraZ.y * inf, cameraZ.z * inf,
			cameraY.x, cameraY.y, cameraY.z
		);

		renderNode(root, Color::DEFAULT_COLOR, false);
		glPopMatrix();

		////////////// LABELS //////////////
		glViewport(0, 0, this->width, this->height);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		stringstream ss;
		ss << "Depth: " << displayLevel;
		displayText(-0.99, -0.7, 1, 1, 0, ss.str().c_str());

		ss.str(std::string());
		ss << "Triangles: " << geometry.size() << ", In Node: " << current->getTriangles().size() << " In Parent: " << (current->getParent() != nullptr ? current->getParent()->getTriangles().size() : 0);
		displayText(-0.99, -0.8, 1, 1, 0, ss.str().c_str());

		ss.str(std::string());
		ss << "Max to Test: " << trianglesInVolumes << ", Actually Tested: " << testedTriangles << ", PVS: " << visibleTriangles.size();
		displayText(-0.99, -0.9, 1, 1, 0, ss.str().c_str());

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	/** Renders the current level of hierarchy. */
	void renderCurrentLevel()
	{
		if (current->getParent() != nullptr)
		{
			// NOT ROOT
			BVH* left = current->getParent()->getLeft();
			BVH* right = current->getParent()->getRight();

			renderNode(left, Color::DEFAULT_COLOR, true);
			renderNode(right, Color::DEFAULT_COLOR, true);
		}
		else
		{
			// ROOT
			current->render(highlightVisible && visibleVolumes.find(current) != visibleVolumes.end() ? Color::ORANGE : Color::GREEN, matrix);
			renderNode(current, Color::GREEN, true);
		}
	}

	/** Renders triangles in the given node with the specified color. */
	void renderNode(BVH* node, Color color, bool renderVolume)
	{
		if (renderVolume) {
			if (highlightVisible && visibleVolumes.find(node) != visibleVolumes.end())
			{
				node->render(Color::ORANGE, matrix);
			}
			else if (current == node) {
				node->render(Color::GREEN, matrix);
			}
			else
			{
				node->render(color, matrix);
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (Triangle* triangle : node->getTriangles())
		{
			if (highlightVisible && visibleTriangles.find(triangle) != visibleTriangles.end()) // visible triangles
			{
				triangle->render(Color::ORANGE);
			}
			else if (current->getTriangles().find(triangle) != current->getTriangles().end()) // current triangles
			{
				triangle->render(Color::GREEN);
			}
			else // other
			{
				triangle->render(color);
			}
		}
	}

	/** Renders all visible triangles. */
	void renderVisibleTriangles()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (Triangle* triangle : visibleTriangles)
		{
			triangle->render(Color::ORANGE);
		}
	}

	/** Renders the camera. */
	void renderCamera()
	{
		// LINE
		float arrowHead = 0.2;
		float heightScale = 0.5;

		glLineWidth(3);
		glColor3f(1, 0, 1);
		glBegin(GL_LINES);

		Tuple3f end = cameraPosition + cameraZ * heightScale;
		glVertex3f(cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glVertex3f(end.x, end.y, end.z);

		Tuple3f arrow = end - cameraZ * arrowHead + cameraX * arrowHead;
		glVertex3f(end.x, end.y, end.z);
		glVertex3f(arrow.x, arrow.y, arrow.z);

		arrow = end - cameraZ * arrowHead - cameraX * arrowHead;
		glVertex3f(end.x, end.y, end.z);
		glVertex3f(arrow.x, arrow.y, arrow.z);

		arrow = end - cameraZ * arrowHead + cameraY * arrowHead;
		glVertex3f(end.x, end.y, end.z);
		glVertex3f(arrow.x, arrow.y, arrow.z);

		arrow = end - cameraZ * arrowHead - cameraY * arrowHead;
		glVertex3f(end.x, end.y, end.z);
		glVertex3f(arrow.x, arrow.y, arrow.z);

		glEnd();
		glLineWidth(1);

		// QUAD
		const Vector3f want(0.0, 0.0, 1.0);
		const double angle = (acos(cameraZ.Dot(want)) * 180.0) / M_PI;
		Vector3f plane = want.Cross(cameraZ);
		plane.Normalize();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glRotatef(angle, plane.x, plane.y, plane.z);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(1, 1, 1, 0.5);
		glBegin(GL_QUADS);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
		glEnd();

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	/******************************************************************************************************************/
	/*************************************************** ASSIGNMENT ***************************************************/
	/******************************************************************************************************************/

	// For the detailed documentation of this method see BVHExample.cpp
	BVH* construct(unordered_set<Triangle*> triangles, int depth, VolumeType volumeType) const;

	// For the detailed documentation of this method see BVHExample.cpp
	unordered_set<Triangle*> pvs(BVH* node, const Tuple3f cameraPosition, const Vector3f cameraNormal, const Vector3f cameraRightVector, const Vector3f cameraUpVector, int& testedTriangles, unordered_set<BVH*>& visibleVolumes) const;

};



