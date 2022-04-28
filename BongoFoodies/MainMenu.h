#pragma once
#include <stdlib.h>
#include "Search.h"
#include "Order.h"
#include "Foodie.h"
#include "HomeCook.h"
using namespace std;

class MainMenu
{
public:
	MainMenu();
	virtual ~MainMenu();
	void highlights();
	void browse();
	void tips();
	void login();
};

User* login_interface(SAConnection& conn);
void main_menu();

