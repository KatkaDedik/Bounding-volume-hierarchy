#include "../core/BaseWindow.h"

void BaseWindow::show(int argc, char** argv)
{
	glutInit(&argc, argv);

	g_CurrentWindowInstance = this;

	// INITS GLUT 
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
	glutInitWindowPosition(INIT_POS_X, INIT_POS_Y);
	glutCreateWindow(WIN_NAME);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// INIT TRACKBALL
	trackball(curquat, 0.0, 0.0, 0.0, 0.0);
	build_rotmatrix(matrix, curquat);

	// PROJECTION 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-this->width/2.0, this->width / 2.0, -this->width / 2.0, this->width / 2.0, 1.5, 20.0);
	//glFrustum(-1, 1, -1, 1, 1, 20.0);
	scalefactor = 0.5;
	glOrtho(-1, 1, -1, 1, 0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (this->centerOrigin || this->RenderMode3D)
	{
		gluLookAt(
			0.0, 0.0, 3.0,   // eye
			0.0, 0.0, 0.0,   // target 
			0.0, 1.0, 0.0);  // up-vector
	}
	else
	{
		gluLookAt(
			this->width / 2.0f, this->height / 2.0f, 1.0,   // eye
			this->width / 2.0f, this->height / 2.0f, 0.0,   // target 
			0.0, 1.0, 0.0);  // up-vector
	}

	// SETS CALLBACKS
	glutDisplayFunc(BaseWindow::displayCallback);
	glutKeyboardFunc(BaseWindow::keyboardCallback);
	glutMotionFunc(BaseWindow::motionCallback);
	glutPassiveMotionFunc(BaseWindow::passiveMotionCallback);
	glutMouseFunc(BaseWindow::mouseCallback);
	glutReshapeFunc(BaseWindow::reshapeCallback);
	glutSpecialFunc(BaseWindow::spectialInputCallback);
}

void BaseWindow::render() {
	if (updateModelViewMatrix)
	{
		glPopMatrix();
		glPushMatrix();
		glMultMatrixf(&matrix[0][0]);
		glScalef(scalefactor, scalefactor, scalefactor);
		updateModelViewMatrix = false;
	}

	// Renders all the components.
	for (auto* component : this->components)
	{
		component->render();
	}
}

void BaseWindow::keyboard(unsigned char key, int x, int y) {
	for (auto* component : this->components)
	{
		component->keyboardEvent(key, x, y);
	}
	glutPostRedisplay();
}

void BaseWindow::specialInput(int key, int x, int y)
{
	glutPostRedisplay();
}

void BaseWindow::mouse(int button, int state, int x, int y) {

	if (this->RenderMode3D)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			mouseX = x;
			mouseY = y;
			if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
			{
				scaling = true;
				return;
			}
			else
			{
				moving = true;
				return;
			}

		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			moving = false;
			scaling = false;
		}
	}


	for (auto* component : this->components)
	{
		component->mouseEvent(button, state, x, y);
	}
	glutPostRedisplay();
}


void BaseWindow::motion(int x, int y) {
	if (scaling) {
		scalefactor = scalefactor * (1.0 + (((float)(mouseY - y)) / this->height));
		mouseX = x;
		mouseY = y;
		updateModelViewMatrix = true;
	}
	if (moving) {
		float halfW = this->width / 2.f;
		trackball(lastquat,
			(2.0 * mouseX - halfW) / this->width,
			(this->height - 2.0 * mouseY) / this->height,
			(2.0 * x - halfW) / this->width,
			(this->height - 2.0 * y) / this->height
		);
		mouseX = x;
		mouseY = y;

		add_quats(lastquat, curquat, curquat);
		build_rotmatrix(matrix, curquat);
		updateModelViewMatrix = true;
	}

	for (auto component : this->components)
	{
		component->mouseMotionEvent(x, y);
	}

	glutPostRedisplay();
}

void BaseWindow::passiveMotion(int x, int y) {

}

void BaseWindow::reshape(int w, int h) {

	glViewport(0, 0, w, h);

	// ignore the values for now
	//this->width = w;
	//this->height = h;

	// snap back to the width we want
	glutReshapeWindow(this->width, this->height);
}

void BaseWindow::addComponent(Component* component)
{
	components.push_back(component);
	for (auto& subcomponent : component->getComponents()) // TODO: Make this recursive.
	{
		components.push_back(subcomponent);
	}
}

void BaseWindow::removeComponent(Component* component)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if (*it == component) {
			it = components.erase(it);
		}
		else
		{
			++it;
		}
	}
	//for (auto& subcomponent : component->getComponents()) // TODO: also remove subcomponents
	//{
	//	components.push_back(subcomponent);
	//}
}