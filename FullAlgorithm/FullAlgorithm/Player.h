#include "Hand.h"

class Player {
private:
	double cX;
	double cY;
	double area;
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
	Hand getLeftHand();
	Hand getRightHand();
	
	// Sets
	void setCX(double cX);
	void setCY(double cY);
	void setArea(double area);
	void setLeftHand(Hand leftHand);
	void setRightHand(Hand rightHand);
};