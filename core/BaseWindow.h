#pragma once
#include "../core/Trackball.h"
#include "../core/Component.h"


#define INIT_WIDTH 1600
#define INIT_HEIGHT 800
#define INIT_POS_X 0
#define INIT_POS_Y 0
#define WIN_NAME "OpenGL Assignment"

/** The static global instance of this class for GLUT callbacks. */
static class BaseWindow* g_CurrentWindowInstance;

/**
* The base class for all examples. It handles the window creation and manages the mouse and keyboard events.
*
* @author Jan Byška <jan.byska@gmail.com>
*/
class BaseWindow
{
protected:
	/** The current window width. */
	int width = INIT_WIDTH;

	/** The current window height. */
	int height = INIT_HEIGHT;

	/** Determines whether the window will be used as 3D or 2D. */
	bool RenderMode3D = true;

	/** The list of components that should be rendered and react to events. */
	vector<Component*> components;

	/** The center origin flag; if {@p true} the point [0,0,0] will be in the window center; if {@p false} the point [0,0,0] will be in the top left corner. */
	bool centerOrigin = false;

	/** The flag determining if the camera is moving. */
	bool moving = false;

	/** The flag determining if the camera is scaling. */
	bool scaling = false;

	/** The current scale factor for camera. */
	float scalefactor = 1.0;

	/** The X coordinate of the last known mouse position during the scene rotation. */
	int mouseX = 0;

	/** The Y coordinate of the last know mouse position during the scene rotation. */
	int mouseY = 0;

	/** The matrix determining the current orientation of the camera. */
	float curquat[4];

	/** The  matrix determining the last orientation of the camera. */
	float lastquat[4];

	/** Rotation matrix. */
	GLfloat matrix[4][4];

	/** The update flag. */
	int updateModelViewMatrix = true;

public:
	/**
	* Initializes the GLUT and shows the window.
	* @param argc			The number of command line arguments are passed to main().
	* @param argv			The list of command line arguments are passed to main().
	*/
	void show(int argc, char **argv);

protected:
	/** This method forwards the 'reshape' callback from GLUT to 'reshape(int,int)' method. */
	static void reshapeCallback(int w, int h) { g_CurrentWindowInstance->reshape(w, h); }
	/** This method forwards the 'display' callback from GLUT to 'render()' method. */
	static void displayCallback() 
	{ 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_CurrentWindowInstance->render(); 
		
		glutSwapBuffers();
	}
	/** This method forwards the 'keyboard' callback from GLUT to 'keyboard()' method. */
	static void keyboardCallback(unsigned char key, int x, int y) { g_CurrentWindowInstance->keyboard(key, x, y); }
	/** This method forwards the 'mouse' callback from GLUT to 'mouse(int,int,int,int)' method. */
	static void mouseCallback(int button, int state, int x, int y) {
		// Updates mouse position if the window is centered.
		if (g_CurrentWindowInstance->centerOrigin)
		{
			x -= g_CurrentWindowInstance->width / 2.0f;
			y -= g_CurrentWindowInstance->height / 2.0f;
		}
		g_CurrentWindowInstance->mouse(button, state, x, y);
	}
	/** This method forwards the 'motion' callback from GLUT to 'motion(int,int)' method. */
	static void motionCallback(int x, int y) {
		// Updates mouse position if the window is centered.
		if (g_CurrentWindowInstance->centerOrigin)
		{
			x -= g_CurrentWindowInstance->width / 2.0f;
			y -= g_CurrentWindowInstance->height / 2.0f;
		}
		g_CurrentWindowInstance->motion(x, y);
	}
	/** This method forwards the 'passive motion' callback from GLUT to 'pasiveMotion(int,int)' method. */
	static void passiveMotionCallback(int x, int y) {
		g_CurrentWindowInstance->passiveMotion(x, y);
	}
	/** This method forwards the 'specialFunc' callback from GLUT to 'specialInput(int, int,int)' method. */
	static void spectialInputCallback(int key, int x, int y) {
		g_CurrentWindowInstance->specialInput(key, x, y);
	}

	/** Invoked when the window is changed. */
	virtual void reshape(int w, int h);

	/** Invoked every frame. */
	virtual void render();

	/**
	 * Invoked when a key is pressed.
	 *
	 * @param	key	The key in ASCII.
	 * @param	x  	The X coordinate of mouse location in window relative coordinates when the key was pressed.
	 * @param	y  	The Y coordinate of mouse location in window relative coordinates when the key was pressed.
	 */
	virtual void keyboard(unsigned char key, int x, int y);

	/** Invoked when a mouse button has been pressed or released. */
	virtual void mouse(int button, int state, int x, int y);

	/**
	 * Invoked when the mouse has been moved while a mouse button was pressed.
	 *
	 * @param	x	The X coordinate of the current mouse location.
	 * @param	y	The Y coordinate of the current mouse location.
	 */
	virtual void motion(int x, int y);

	/**
	 * Invoked when the mouse has been moved.
	 *
	 * @param	x	The X coordinate of the current mouse location.
	 * @param	y	The Y coordinate of the current mouse location.
	 */
	virtual void passiveMotion(int x, int y);

	/**   
	 * Invoked when a special input was passed to the GLUT.
	 *
	 * @param	key	The special key that was pressed.
	 * @param	x	The X coordinate of the current mouse location.
	 * @param	y	The Y coordinate of the current mouse location.
	 */
	virtual void specialInput(int key,int x, int y);

	/**
	* Adds the specified component to this window.
	* @param component		The component to be added.
	*/
	void addComponent(Component* component);

	/**
	* Removes the specified component from this window.
	* @param component		The component to be removed.
	*/
	void removeComponent(Component* component);
};
