#include "Hand.h"

class Player {
private:
	double cX;
	double cY;
	double area;
	double velX;
	Hand leftHand;
	Hand rightHand;
public:
	Player() {}
	Player(double cX,double cY,double area);
	void printPlayer();

	// Gets
	double getCX();
	double getCY();
	double getArea();
	double getVelX();
	Hand getLeftHand();
	Hand getRightHand();
	
	// Sets
	void setCX(double cX);
	void setCY(double cY);
	void setArea(double area);
	void setVelX(double velX);
	void setLeftHand(Hand leftHand);
	void setRightHand(Hand rightHand);
};