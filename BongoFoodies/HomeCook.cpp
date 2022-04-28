#include "HomeCook.h"

extern bool cook_toggle;

HomeCook::HomeCook() {
	//cout << "hcctor\n";
}

HomeCook::~HomeCook() {
	//cout << "hcdtor\n";
}

HomeCook* HomeCook::Register(SAConnection& conn) {
	system("CLS");
	cout << "\n\t **** Registration ****\n\n";

	UserID++;
	time_t now = time(0);
	struct tm* date = localtime(&now);
	string name, email, pass;
	cout << "Name:";
	cin >> name;
	cout << "Email:";
	cin >> email;
	cout << "Password:";
	cin >> pass;


	SACommand insert(&conn);
	try {
		insert.setCommandText(_TSA("INSERT INTO Users (UserID, name, email, pass, reg) VALUES (:1, :2, :3, :4, :5)"));
		insert << (unsigned short)UserID << name.c_str() << email.c_str() << pass.c_str() << SADateTime(1900 + date->tm_year, 1 + date->tm_mon, date->tm_mday);
		insert.Execute();
		conn.Commit();
		cout << "\nRegistration successful!\n" << endl;
		return Login(conn);
	}

	catch (SAException& e) {
		(void)e;
		conn.Rollback();
		cout << e.ErrText().GetMultiByteChars();
		cout << "\nEmail already registered!\n";
		cout << "1.Retry? 2.Login? ... ";
		int ch;
		cin >> ch;
		if (ch == 2)
			return Login(conn);
		else
			return Register(conn);
	}
}

HomeCook* HomeCook::Login(SAConnection& conn)
{
	Sleep(100);
	system("CLS");
	cout << "\n\t **** Login ****\n\n";


	string email, pass;
	cout << "Email:";
	cin >> email;
	cout << "Password:";
	cin >> pass;


	SACommand select(&conn);
	select.setCommandText(_TSA("SELECT UserID, name, pass, reg FROM Users WHERE email = :1"));
	select << email.c_str();
	select.Execute();

	if (!select.FetchNext()) {
		cout << "\nEmail not registered!\n";
		cout << "1.Retry? 2.Register? ... ";
		int ch;
		cin >> ch;
		if (ch == 2)
			return Register(conn);
	}
	else {
		if (pass != select.Field(_TSA("pass")).asString().GetMultiByteChars()) {
			cout << "\nIncorrect password!\n";
		}
		else {
			Name = select.Field(_TSA("name")).asString().GetMultiByteChars();
			UserID = select.Field(_TSA("UserID")).asLong();
			DOR = (struct tm)select.Field(_TSA("reg")).asDateTime();
			Email = email;
			Password = pass;
			cout << "\nLogin successful!\n" << endl;
			cook_toggle = UserID;
			return this;
		}
	}

	return Login(conn);
}


void HomeCook::profile(SAConnection& conn)
{
	cout << "User ID: " << UserID << endl;
	cout << "Name: " << Name << endl;
	cout << "Email: " << Email << endl;
	cout << "Member since: " << DOR.tm_mday << "-" << DOR.tm_mon << "-" << DOR.tm_year + 1900 << endl << "\n\n";


	cout << "1. Upload a new recipe\n";
	cout << "2. View all recipes\n";
	cout << "3. View pending orders\n";
	cout << "4. Logout\n";
	cout << "5. Return to Main Menu\n\n";
	cout << "\nEnter choice: ";
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
	{
		Recipe r;
		r.upload_recipe(conn, UserID);
	}
	break;
	case 2:
	{
		system("CLS");
		string query;
		query = "SELECT Title FROM Recipes WHERE CookID = ";
		query += UserID + '0';
		vector<string> file_list = show_recipe_list(conn, query, "Title");
		cout << "\nEnter choice: ";
		int ch;
		cin >> ch;
		if (ch != 0) {
			Recipe r;
			r.show_recipe_details(conn, file_list[(unsigned __int64)ch - 1]);
		}
	}
	break;
	case 3: {
		this->orders_to_be_fulfilled(conn);
	}
		  break;
	case 4:
		cook_toggle = 0;
		cout << "Logging Out\n\n";
		break;
	default:
		break;
	}
}




void HomeCook::orders_to_be_fulfilled(SAConnection& conn)
{
	int orderId, foodId, customerId, quantity;
	string title;
	string status = "In progress";
	SACommand find(&conn), find2(&conn);
	find.setCommandText(_TSA("SELECT * FROM ORD WHERE FoodID IN (SELECT RecipeID FROM Recipes WHERE CookID= :1) AND Status = :2"));
	find << (unsigned short)UserID << status.c_str();
	find.Execute();

	while (find.FetchNext())
	{
		orderId = find.Field(_TSA("OrderID")).asUShort();
		foodId = find.Field(_TSA("FoodID")).asUShort();
		customerId = find.Field(_TSA("CustomerID")).asUShort();
		quantity = find.Field(_TSA("Quantity")).asUShort();

		find2.setCommandText(_TSA("SELECT Title FROM Recipes WHERE RecipeID = :1"));
		find2 << (unsigned short)foodId;
		find2.Execute();
		while (find2.FetchNext())
		{
			title = find2.Field(_TSA("Title")).asString().GetMultiByteChars();
		}

		cout << "\n\nOrder Id: " << orderId << "\nCustomer Id: " << customerId << "\nTitle: " << title << "\nQuantity: " << quantity << "\n\n";
	}
}

