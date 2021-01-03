#include "Component.h"

void Component::setLocation(int x, int y) {
	this->x = x;
	this->y = y;
}

void Component::render() const
{
	Rectangle2D bounds = this->getBounds();
	bounds.render(color);
}


void Component::keyboardEvent(unsigned char key, int x, int y)
{
}

void Component::mouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && this->contains(x, y))
	{
		this->active = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		this->active = false;
	}
}

Rectangle2D Component::getBounds() const
{
	return Rectangle2D(this->x, this->y, this->width, this->height);
}

void Component::addComponent(Component* component)
{
	components.push_back(component);
}

const vector<Component*>& Component::getComponents() const
{
	return this->components;
}