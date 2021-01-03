#pragma once

#include "Core.h"

/**
* The super-class containing the shared functionality for all components. 
* A component is an object that can be displayed on the screen and user can interact with it. 
*
* @author <a href="mailto:jan.byska@gmail.com">Jan Byška</a>
*/
class Component
{
protected:
	/** The component width. */
	float width = 100;
	/** The component height. */
	float height = 100;
	/** The X position of the component in the _parent's coordinate system (in real screen pixels). */
	int x = 0;
	/** The Y position of the component in the _parent's coordinate system (in real screen pixels). */
	int y = 0;
	/* The list of sub-components. */
	vector<Component*> components;
	/** The flag indicating whether the user is interacting with this component. */
	bool active = false;
	/** The default component color. */
	Color color;

public:

	/**
	* Constructs a new component with the given size (width, height) and color at the given location (x,y).
	* @param x				The X coordinate of the top-left corner of this component.
	* @param y				The Y coordinate of the top-left corner of this component.
	* @param width			The new width of this component in pixels.
	* @param height			The new height of this component in pixels.
	* @param color			The default component color.
	*/
	Component(int x, int y, float width, float height, Color color) : x(x), y(y), width(width), height(height), color(color) {}

	/** Returns the current X coordinate of the components origin. */
	inline int getX() const { return this->x; }
	/** Returns the current Y coordinate of the components origin. */
	inline int getY() const { return this->y; }
	/** Returns the current width of this component. */
	inline int getWidth() const { return this->width; }
	/** Returns the current height of this component. */
	inline int getHeight() const { return this->height; }

	/**
	 * Returns the center position of this component. WARNING: Rounding errors may appear.
	 *
	 * @returns	The current center.
	 */
	inline Point2D getCenter() const { return Point2D(this->x + this->width / 2.f, this->y + this->height / 2.f);  }

	/**
	* Moves this component to the location specified by the given (x,y) coordinates.
	* @param x			The X coordinate of the new handle position.
	* @param y			The Y coordinate of the new handle position.
	*/
	void setLocation(int x, int y);

	/**
	 * Return the current location of this component.
	 *
	 * @returns	The current location.
	 */
	Point2D getLocation() const { return Point2D(this->x, this->y); };

	/**
	* Renders this component on the screen.
	* The default implementation will render only the bounding box.
	*/
	virtual void render() const;

	/**
	 * Invoked when a user types into the window.
	 *
	 * @param	key	The key in ASCII.
	 * @param	x  	The x coordinate of mouse location in window relative coordinates when the key was pressed.
	 * @param	y  	The y coordinate of mouse location in window relative coordinates when the key was pressed.
	 */
	virtual void keyboardEvent(unsigned char key, int x, int y);

	/** 
	* Invoked when a mouse button has been pressed or released. 
	* By default on (left) mouse button press/release this method will activate/deactivate the component. 
	*
	* @param button			Can be one of the GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON indicating the mouse button responsible for state change.
	* @param state			Can be either GLUT_UP or GLUT_DOWN indicating whether the callback was due to a release or press respectively.
	* @param x				The window relative X coordinate when the mouse button state changed.
	* @param y				The window relative Y coordinate when the mouse button state changed.
	*/
	virtual void mouseEvent(int button, int state, int x, int y);

	/**
	 * Invoked when the mouse cursor has been moved.
	 *
	 * @param	x	The X coordinate of the current mouse location.
	 * @param	y	The Y coordinate of the current mouse location.
	 */
	virtual void mouseMotionEvent(int x, int y) {}

	/** Tests if the specified coordinates (x,y) are inside this component. */
	inline bool contains(int x, int y) const { return getBounds().contains(x, y); }

	/** Returns a bounding box of this component. */
	Rectangle2D getBounds() const;

	/** Adds a subcomponent to this component. */
	void addComponent(Component* component);

	/** Adds a subcomponent to this component. */
	const vector<Component*>& getComponents() const;

	
};

