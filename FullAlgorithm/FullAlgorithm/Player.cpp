#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(double cX, double cY, double area)
{
	this->cX = cX;
	this->cY = cY;
	this->area = area;
}

void Player::printPlayer()
{
	cout << "Player:\n cX = " << this->cX << "\ncY = " << this->cY << "\Area = " << this->area << endl;
}

// Gets
double Player::getCX()
{
	return this->cX;
}

double Player::getCY()
{
	return this->cY;
}

double Player::getArea()
{
	return this->area;
}

double Player::getVelX()
{
	return this->velX;
}

Hand Player::getLeftHand()
{
	return this->leftHand;
}

Hand Player::getRightHand()
{
	return this->rightHand;
}
// Sets

void Player::setCX(double cX)
{
	this->cX = cX;
}

void Player::setCY(double cY)
{
	this->cY = cY;
}

void Player::setArea(double area)
{
	this->area = area;
}

void Player::setVelX(double velX)
{
	this->velX = velX;
}

void Player::setLeftHand(Hand leftHand)
{
	this->leftHand = leftHand;
}

void Player::setRightHand(Hand rightHand)
{
	this->rightHand = rightHand;
}