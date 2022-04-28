#include "Search.h"

void browse_interface(SAConnection& conn) {
	while (1) {
		cout << "\n\n";
		cout << "1.All\n" << "2.by Region\n" << "3.by Review\n" << "4.by Price\n" << "5.by Delivery Area\n" << "6.Return to main menu\n";
		cout << "\nEnter choice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			byTitle(conn, "");
			break;
		case 2:
			byRegion(conn);
			break;
		case 3:
			byReview(conn);
			break;
		case 4:
			byPrice(conn);
			break;
		case 5:
			byArea(conn);
			break;
		case 6:
			return;
		}
		system("CLS");
	}
}

void byTitle(SAConnection& conn, string str)
{
	system("CLS");
	string query = "SELECT Title FROM Recipes" + str;
	vector<string> file_list = show_recipe_list(conn, query, "Title");
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	if (ch == 0)
		return;
	else {
		Recipe r;
		r.show_recipe_details(conn, file_list[(unsigned __int64)ch - 1]);
	}
}

void byRegion(SAConnection& conn)
{
	system("CLS");
	string query = "SELECT DISTINCT Region FROM Recipes";
	vector<string> file_list = show_recipe_list(conn, query, "Region");
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	if (ch == 0)
		return;
	else {
		//SELECT Title FROM Recipes WHERE Region = 'input'
		string str = " WHERE Region = '" + file_list[(unsigned __int64)ch - 1] + "'";
		byTitle(conn, str);
	}
}

void byReview(SAConnection& conn)
{
	system("CLS");
	cout << "\n\n1. *\n" << "2. **\n" << "3. ***\n" << "4. ****\n" << "5. *****\n" << "0. Back to categories\n";
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	switch (ch)
	{
		//SELECT Title FROM Recipes WHERE RecipeID = (SELECT RecipeId FROM Rating_chart WHERE Rating >= input)
	case 1:
		byTitle(conn, " WHERE RecipeID IN (SELECT RecipeId FROM Rating_chart WHERE Rating >= 1.0)");
		break;
	case 2:
		byTitle(conn, " WHERE RecipeID IN (SELECT RecipeId FROM Rating_chart WHERE Rating >= 2.0)");
		break;
	case 3:
		byTitle(conn, " WHERE RecipeID IN (SELECT RecipeId FROM Rating_chart WHERE Rating >= 3.0)");
		break;
	case 4:
		byTitle(conn, " WHERE RecipeID IN (SELECT RecipeId FROM Rating_chart WHERE Rating >= 4.0)");
		break;
	case 5:
		byTitle(conn, " WHERE RecipeID IN (SELECT RecipeId FROM Rating_chart WHERE Rating = 5.0)");
		break;
	case 6:
		break;
	}
}

void byPrice(SAConnection& conn)
{
	system("CLS");
	cout << "\n\n1. Under 100 taka\n" << "2. 100-500 taka\n" << "3. 500-1000 taka\n" << "0. Back to categories\n";
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	switch (ch)
	{
		//SELECT Title FROM Recipes WHERE Price >= input
	case 1:
		byTitle(conn, " WHERE Price < 100");
		break;
	case 2:
		byTitle(conn, " WHERE Price BETWEEN 100 AND 500");
		break;
	case 3:
		byTitle(conn, " WHERE Price BETWEEN 500 AND 1000");
		break;
	case 4:
		break;
	}
}

void byArea(SAConnection& conn)
{
	system("CLS");
	string query = "SELECT DISTINCT Area FROM Delivery_area";
	vector<string> file_list = show_recipe_list(conn, query, "Area");
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	if (ch == 0)
		return;
	else {

		//SELECT Title FROM Recipes WHERE RecipeID = (SELECT RecipeId FROM Delivery_area WHERE Area = 'input')
		string str = " WHERE RecipeID IN (SELECT RecipeId FROM Delivery_area WHERE Area = '" + file_list[(unsigned __int64)ch - 1] + "')";
		byTitle(conn, str);
	}
}
