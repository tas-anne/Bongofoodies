#pragma once
#include "User.h"

class Foodie :
	public User
{
public:
	Foodie();
	virtual ~Foodie();
	virtual Foodie* Register(SAConnection& conn);
	virtual Foodie* Login(SAConnection& conn);
	void profile(SAConnection& conn);

private:
	void order_history(SAConnection& conn);
};

