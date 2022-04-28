#include "Recipe.h"
extern bool cook_toggle;
extern bool foodie_toggle;
int Recipe::RecipeID = 0;

Recipe::Recipe()
{

}

Recipe::~Recipe()
{
	//dtor
}


void Recipe::upload_recipe(SAConnection& conn, int cook_ID)
{
	Sleep(100);
	system("CLS");
	cout << "****Recipe Info****\n\n";

	RecipeID++;

	cout << "\n\nTitle: ";
	cin >> title;

	cout << "\nRegion: ";
	cin >> region;

	cout << "\nCooking time (in minutes): ";
	cin >> time;

	cout << "\nNo. of Servings: ";
	cin >> servings;
	cout << "\n";

	string ch;
	do
	{
		cout << "Ingredient: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		string ingre;
		getline(cin, ingre);
		ingredients.push_back(ingre);
		cout << "Add more? (y/n)...";
		cin >> ch;
	} while (ch == "y" || ch == "Y");

	cout << "\n\nDirections: ";
	cin.ignore(256, '\n');
	getline(cin, procedure);

	cout << "\nAvailable for delivery? (y/n)...";
	cin >> ch;
	cout << "\n";
	if (ch == "y" || ch == "Y") {

		do
		{
			cout << "Delivery area: ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			string area;
			getline(cin, area);
			delivery_area.push_back(area);
			cout << "Add more? (y/n)...";
			cin >> ch;
		} while (ch == "y" || ch == "Y");

		cout << "\nPrice (in taka): ";
		cin >> price;
	}

	SACommand insert(&conn);
	try
	{
		insert.setCommandText(_TSA("INSERT INTO Recipes (RecipeID, Title, CookID, Region, Time, Servings, Price) VALUES (:1, :2, :3, :4, :5, :6, :7)"));
		insert << (unsigned short)RecipeID << title.c_str() << (unsigned short)cook_ID << region.c_str() << (double)time << (unsigned short)servings << (double)price;
		insert.Execute();

		insert.setCommandText(_TSA("INSERT INTO Rating_chart VALUES (:1, 0, 0, 0, 0, 0, 0.0)"));
		insert << (unsigned short)RecipeID;
		insert.Execute();

		for (int i = 0; i < delivery_area.size(); i++)
		{
			insert.setCommandText(_TSA("INSERT INTO Delivery_Area (RecipeID, Area) VALUES (:1, :2)"));
			insert << (unsigned short)RecipeID << delivery_area[i].c_str();
			insert.Execute();
		}

		conn.Commit();
	}
	catch (SAException& e)
	{
		(void)e;
		//conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nProblem uploading!\n";
	}

	ofstream recipe_text(title + ".txt");

	for (auto it = ingredients.begin(); it != ingredients.end(); it++)
		recipe_text << *it << " ";
	recipe_text << "\n";

	//for (auto it = procedure.begin(); it != procedure.end(); it++)
	recipe_text << procedure;
	recipe_text << "\n";

	recipe_text.close();


	cout << "\nNew recipe uploaded!\n" << endl;
}


void Recipe::show_recipe_details(SAConnection& conn, string filename)
{
	Sleep(100);
	system("CLS");

	SACommand select1(&conn), select2(&conn), select3(&conn), select4(&conn);
	try
	{
		select1.setCommandText(_TSA("SELECT * FROM Recipes WHERE Title = :1"));
		select1 << filename.c_str();
		select1.Execute();
		select2.setCommandText(_TSA("SELECT UserID, Name FROM Users WHERE UserID = (SELECT CookID FROM Recipes WHERE Title = :1)"));
		select2 << filename.c_str();
		select2.Execute();
		select3.setCommandText(_TSA("SELECT * FROM Rating_chart WHERE RecipeID = (SELECT RecipeID FROM Recipes WHERE Title = :1)"));
		select3 << filename.c_str();
		select3.Execute();
		select4.setCommandText(_TSA("SELECT Area FROM Delivery_area WHERE RecipeID = (SELECT RecipeID FROM Recipes WHERE Title = :1)"));
		select4 << filename.c_str();
		select4.Execute();


		while (select1.FetchNext())
		{

			RecipeID = select1.Field(_TSA("RecipeID")).asUShort();
			title = select1.Field(_TSA("Title")).asString().GetMultiByteChars();
			region = select1.Field(_TSA("Region")).asString().GetMultiByteChars();
			time = select1.Field(_TSA("Time")).asDouble();
			servings = select1.Field(_TSA("Servings")).asUShort();
			price = select1.Field(_TSA("Price")).asDouble();


			while (select2.FetchNext())
			{
				cook_ID = select2.Field(_TSA("UserID")).asUShort();
				cook_name = select2.Field(_TSA("Name")).asString().GetMultiByteChars();
			}
			while (select3.FetchNext())
			{
				star_count.push_back(select3.Field(_TSA("star5")).asUShort());
				star_count.push_back(select3.Field(_TSA("star4")).asUShort());
				star_count.push_back(select3.Field(_TSA("star3")).asUShort());
				star_count.push_back(select3.Field(_TSA("star2")).asUShort());
				star_count.push_back(select3.Field(_TSA("star1")).asUShort());
				rating = select3.Field(_TSA("Rating")).asDouble();
			}
			while (select4.FetchNext())
			{
				delivery_area.push_back(select4.Field(_TSA("Area")).asString().GetMultiByteChars());
			}


			ifstream recipe_text(title + ".txt");

			string line;
			getline(recipe_text, line);
			std::istringstream iss(line);

			for (string ing; iss >> ing;)
				ingredients.push_back(ing);

			getline(recipe_text, line);
			procedure = line;

			recipe_text.close();


			cout << "****Recipe Info****\n\n";
			cout << "\n\nTitle: " << title;
			cout << "\nCook name: " << cook_name;
			cout << "\nRegion: " << region;
			cout << "\nCooking time (in minutes): " << time;
			cout << "\nNo. of Servings: " << servings;

			cout << "\nIngredients: ";
			int s = ingredients.size();
			for (auto it = ingredients.begin(); it != ingredients.end(); it++) {
				if (s > 1)
					cout << *it << ", ";
				else
					cout << *it;
				s--;
			}

			cout << "\nDirections: " << procedure;
			cout << "\nRating: " << rating;

			if (delivery_area.empty())
				cout << "\nDelivery service not available!";
			else {
				cout << "\nDelivery areas: ";
				int s = delivery_area.size();
				for (auto it = delivery_area.begin(); it != delivery_area.end(); it++) {
					if (s > 1)
						cout << *it << ", ";
					else
						cout << *it;
					s--;
				}
				cout << "\nPrice (in taka): " << price << endl;
			}

			cout << "\n\n";
		}
	}
	catch (SAException& e)
	{
		(void)e;
		//conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nProblem viewing!\n";
	}

	if (cook_toggle > 0)
		this->recipe_options_interface(conn);
	else if (foodie_toggle > 0)
		this->recipe_options_interface(conn, User::UserID);

	system("pause");
}



void Recipe::recipe_options_interface(SAConnection& conn)
{
	cout << "\n\n\n";
	if (cook_toggle == cook_ID) {
		cout << "1.Edit recipe\n" << "2.Download recipe\n" << "3.Back\n";
		cout << "\nEnter choice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			this->edit_recipe(conn);
			break;
		case 2:
			this->download_recipe(conn);
			break;
		case 3:
			break;
		}
	}
	else {
		cout << "1.Download recipe\n" << "2.Back\n";
		cout << "\nEnter choice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			this->download_recipe(conn);
			break;
		case 2:
			break;
		}
	}
}

void Recipe::recipe_options_interface(SAConnection& conn, int foodie_ID)
{
	cout << "\n\n\n";
	cout << "1.Give feedback\n" << "2.Download recipe\n" << "3.Order recipe\n" << "4.Visit cook profile\n" << "5.Back\n";
	cout << "\nEnter choice: ";
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
		this->give_feedback(conn);
		break;
	case 2:
		this->download_recipe(conn);
		break;
	case 3: {
		this->order_recipe(conn, foodie_ID);
		break; }
	case 4:
		this->show_cook_profile(conn);
		break;
	case 5:
		break;
	}
}

void Recipe::order_recipe(SAConnection& conn, int foodie_ID)
{
	Order order;
	order.order_details(conn, foodie_ID, RecipeID, price, title);
}


void Recipe::edit_recipe(SAConnection& conn)
{
	SACommand update(&conn);
	ofstream recipe_text;
	cout << "\n\nWhat do you want to edit?\n";
	try
	{
		while (1) {
			cout << "\n1.Title  2.Region  3.Cooking time  4.Servings  5.Ingredients  6.Procedure  7.Delivery area  8.Price\n";
			cout << "\nEnter choice: ";
			int choice;
			cin >> choice;
			if (choice == 1) {
				cout << "Enter new title: ";
				cin >> title;
				update.setCommandText(_TSA("UPDATE Recipes SET Title = :1 WHERE RecipeID = :2"));
				update << title.c_str() << (unsigned short)RecipeID;
				update.Execute();
			}
			else if (choice == 2) {
				cout << "Enter new region: ";
				cin >> region;
				update.setCommandText(_TSA("UPDATE Recipes SET Region= :1 WHERE RecipeID = :2"));
				update << region.c_str() << (unsigned short)RecipeID;
				update.Execute();
			}
			else if (choice == 3) {
				cout << "Enter new cooking time (in minutes): ";
				cin >> time;
				update.setCommandText(_TSA("UPDATE Recipes SET Time = :1 WHERE RecipeID = :2"));
				update << (double)time << (unsigned short)RecipeID;
				update.Execute();
			}
			else if (choice == 4) {
				cout << "Enter new no. of servings: ";
				cin >> servings;
				update.setCommandText(_TSA("UPDATE Recipes SET Servings = :1 WHERE RecipeID = :2"));
				update << (unsigned short)servings << (unsigned short)RecipeID;
				update.Execute();
			}
			else if (choice == 5) {
				string ch;
				ingredients.clear();
				do
				{
					cout << "Enter new ingredients: ";
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					string ingre;
					getline(cin, ingre);
					ingredients.push_back(ingre);
					cout << "Add more? (y/n)...";
					cin >> ch;
				} while (ch == "y" || ch == "Y");

				recipe_text.open(title + ".txt", ios::trunc);
				for (auto it = ingredients.begin(); it != ingredients.end(); it++)
					recipe_text << *it << " ";
				recipe_text << "\n";
				recipe_text << procedure;
				recipe_text << "\n";
				recipe_text.close();
			}
			else if (choice == 6) {
				procedure.clear();
				cout << "Enter new procedure: ";
				cin.ignore(256, '\n');
				getline(cin, procedure);
				recipe_text.open(title + ".txt", ios::trunc);
				for (auto it = ingredients.begin(); it != ingredients.end(); it++)
					recipe_text << *it << " ";
				recipe_text << "\n";
				recipe_text << procedure;
				recipe_text << "\n";
				recipe_text.close();
			}
			else if (choice == 7) {
				string ch;
				delivery_area.clear();
				do
				{
					cout << "Delivery area: ";
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					string area;
					getline(cin, area);
					delivery_area.push_back(area);
					cout << "Add more? (y/n)...";
					cin >> ch;
				} while (ch == "y" || ch == "Y");

				for (int i = 0; i < delivery_area.size(); i++)
				{
					update.setCommandText(_TSA("UPDATE Delivery_Area SET Area = :1  WHERE RecipeID = :2"));
					update << delivery_area[i].c_str() << (unsigned short)RecipeID;
					update.Execute();
				}
			}
			else if (choice == 8) {
				if (delivery_area.empty())
					cout << "\nNo delivery areas found!";
				else {
					cout << "Enter new price: ";
					cin >> price;
					update.setCommandText(_TSA("UPDATE Recipes SET Price = :1 WHERE RecipeID = :2"));
					update << (double)price << (unsigned short)RecipeID;
					update.Execute();
				}
			}

			cout << "\nEdit more?? (y/n)...";
			char t;
			cin >> t;
			if (t == 'N' || t == 'n')
				break;

		}

		conn.Commit();
	}
	catch (SAException& e)
	{
		(void)e;
		//conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nProblem editing!\n";
	}

	cout << "\nRecipe edited successfully!\n" << endl;
	system("pause");
}


void Recipe::give_feedback(SAConnection& conn)
{
	cout << "\n\n\nWanna show this recipe some love?";
	cout << "\n\n1. *\n" << "2. **\n" << "3. ***\n" << "4. ****\n" << "5. *****\n";
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	switch (ch)
	{
	case 1:
		star_count[4]++;
		break;
	case 2:
		star_count[3]++;
		break;
	case 3:
		star_count[2]++;
		break;
	case 4:
		star_count[1]++;
		break;
	case 5:
		star_count[0]++;
		break;
	}

	rating = star_count[0] * 5 + star_count[1] * 4 + star_count[2] * 3 + star_count[3] * 2 + star_count[4] * 1;
	rating /= star_count[0] + star_count[1] + star_count[2] + star_count[3] + star_count[4];
	rating = std::round(rating * 100.0) / 100.0;


	SACommand update(&conn);
	try
	{
		update.setCommandText(_TSA("UPDATE Rating_chart SET star5 = :1, star4 = :2, star3 = :3, star2 = :4, star1 = :5, Rating = :6 WHERE RecipeID = :7"));
		update << (unsigned short)star_count[0] << (unsigned short)star_count[1] << (unsigned short)star_count[2] << (unsigned short)star_count[3] << (unsigned short)star_count[4] << (double)rating << (unsigned short)RecipeID;
		update.Execute();
		conn.Commit();
	}
	catch (SAException& e)
	{
		(void)e;
		//conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nProblem uploading!\n";
	}
	system("pause");
}

void Recipe::download_recipe(SAConnection& conn)
{
	//vs

	wchar_t* wc = NULL;
	SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &wc);
	wstring ws(wc);
	CoTaskMemFree(static_cast<void*>(wc));
	string dir(ws.begin(), ws.end());
	string path = dir + "\\" + title + ".txt";

	//codeblocks
	/*
	string path;
	path = getenv("USERPROFILE");
	path = path + "\\Downloads\\" + title + ".txt";
	*/

	ofstream recipe_text;
	recipe_text.open(path, ios::trunc);

	recipe_text << "****Recipe Info****\n\n";
	recipe_text << "\n\nTitle: " << title;
	recipe_text << "\nCook name: " << cook_name;
	recipe_text << "\nRegion: " << region;
	recipe_text << "\nCooking time (in minutes): " << time;
	recipe_text << "\nServings: " << servings;

	recipe_text << "\nIngredients: ";
	int s = ingredients.size();
	for (auto it = ingredients.begin(); it != ingredients.end(); it++) {
		if (s > 1)
			recipe_text << *it << ", ";
		else
			recipe_text << *it;
		s--;
	}

	recipe_text << "\nDirections: " << procedure;
	recipe_text << "\nRating: " << rating;

	if (delivery_area.empty())
		recipe_text << "\nDelivery service not available!";
	else {
		recipe_text << "\nDelivery areas: ";
		int s = delivery_area.size();
		for (auto it = delivery_area.begin(); it != delivery_area.end(); it++) {
			if (s > 1)
				recipe_text << *it << ", ";
			else
				recipe_text << *it;
			s--;
		}
	}

	recipe_text << "\nPrice (in taka): " << price << endl;
	recipe_text.close();

	cout << "\nYour download has been completed. Check downloads folder!\n\n";
	system("pause");
}

void Recipe::show_cook_profile(SAConnection& conn)
{
	cout << "Name: " << cook_name << endl;
	system("CLS");
	cout << "Cook name: " << cook_name;
	cout << "\n\n\nOther recipes from same cook: \n";
	//SELECT Title FROM Recipes WHERE CookID = this->USERID
	string query = "SELECT Title FROM Recipes WHERE CookID = ";
	query += cook_ID + '0';
	vector<string> file_list = show_recipe_list(conn, query, "Title");
	cout << "\nEnter choice: ";
	int ch;
	cin >> ch;
	if (ch != 0) {
		Recipe r;
		r.show_recipe_details(conn, file_list[(unsigned __int64)ch - 1]);
	}
}



vector<string> show_recipe_list(SAConnection& conn, string query, string field)
{
	cout << "\n\n";
	vector<string> file_list;

	SACommand select(&conn);
	try
	{
		select.setCommandText(_TSA(query.c_str()));
		select.Execute();

		int i = 1;
		while (select.FetchNext())
		{

			file_list.push_back(select.Field(_TSA(field.c_str())).asString().GetMultiByteChars());
			cout << i << ". " << file_list[(unsigned __int64)i - 1] << "\n";
			i++;
		}
		cout << "0. Back to categories\n\n";
		return file_list;
	}
	catch (SAException& e)
	{
		(void)e;
		//conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nProblem uploading!\n";
	}
}








