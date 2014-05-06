#include "Hand.h"

Hand::Hand(double cX, double cY, double area)
{
	this->cX = cX;
	this->cY = cY;
	this->area = area;
}

double Hand::getCX()
{
	return this->cX;
}

double Hand::getCY()
{
	return this->cY;
}

double Hand::getArea()
{
	return this->area;
}