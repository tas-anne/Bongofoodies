// FoodTE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "MainMenu.h"
using namespace std;

int main()
{
	cout << "\n\n\n\t\t ========== WELCOME TO BongoFoodies!! ==========\n\n" << flush;
	main_menu();
	return 0;
}


//bfadmin/foodislove@localhost/xepdb1
//DELETE FROM table_name;
/*
	CREATE TABLE Users 
	(
		UserID INT,
		Name VARCHAR2(20),
		Email VARCHAR2(20) UNIQUE,
		Password VARCHAR2(20),
		DOR DATE,
		CONSTRAINT u_pk PRIMARY KEY(UserID)
	);
*/
/*
	CREATE TABLE Recipes
	(
		RecipeID INT,
		Title VARCHAR2(20),
		CookID INT,
		Region VARCHAR2(20), 
		Time NUMBER, 
		Servings INT, 
		Price NUMBER,
		CONSTRAINT r_pk PRIMARY KEY (RecipeID),
		CONSTRAINT r_u_fk FOREIGN KEY (CookID) references Users (UserID)
	);
*/
/*
	CREATE TABLE Delivery_Area
	(
		RecipeID INT,
		Area VARCHAR2(20),
		CONSTRAINT d_r_fk FOREIGN KEY (RecipeID) REFERENCES Recipes (RecipeID)
	);
*/
/*
	CREATE TABLE Rating_chart
	(
		RecipeID INT,
		star5 INT,
		star4 INT,
		star3 INT,
		star2 INT,
		star1 INT,
		Rating NUMBER,
		CONSTRAINT s_pk PRIMARY KEY (RecipeID),
		CONSTRAINT s_r_fk FOREIGN KEY (RecipeID) REFERENCES Recipes (RecipeID)
	);
*/
/*
	CREATE TABLE ORD
	(
		OrderID INT,
		foodID INT,
		customerID INT,
		quantity INT,
		amount NUMBER(4, 2),
		status VARCHAR2(20),
		CONSTRAINT o_fk1 FOREIGN KEY(foodID) references Recipes(RecipeID),
		CONSTRAINT o_fk2 FOREIGN KEY(CustomerID) references Users(UserID)
	);
*/












































/*
	CREATE TABLE Orders
	(
		OrderID INT,
		CustomerID INT,
		RecipeID INT,
		DOO DATE,
		Quantity INT,
		Amount NUMBER,
		is_Pending NUMBER(1),
		CONSTRAINT o_pk PRIMARY KEY (OrderID),
		CONSTRAINT o_u_fk FOREIGN KEY (CustomerID) references Users (UserID),
		CONSTRAINT o_r_fk FOREIGN KEY (RecipeID) references Recipes (RecipeID)
	);
*/