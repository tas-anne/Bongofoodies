#include "Foodie.h"
extern bool foodie_toggle;

Foodie::Foodie() {
	//cout << "fctor\n";
}

Foodie::~Foodie() {
	//cout<<"fdtor\n";
}

void Foodie::profile(SAConnection& conn)
{
	cout << "User ID: " << UserID << endl;
	cout << "Name: " << Name << endl;
	cout << "Email: " << Email << endl;
	cout << "Member since: " << DOR.tm_mday << "-" << DOR.tm_mon << "-" << DOR.tm_year+1900 << endl<<"\n\n";

	cout << "1. View order history\n";
	cout << "2. Logout\n";
	cout << "3. Return to Main Menu\n\n";
	cout << "\nEnter choice: ";
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
		this->order_history(conn);
		break;
	case 2:
		cout << "Logging Out\n\n";
		foodie_toggle = 0;
		break;
	default:
		break;
	}
}

Foodie* Foodie::Register(SAConnection& conn) {
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
		//cout << e.ErrText().GetMultiByteChars();
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

Foodie* Foodie::Login(SAConnection& conn)
{
	Sleep(100);
	system("CLS");
	cout << "\n\t **** Login ****\n\n";


	string name, email, pass;
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
			foodie_toggle = UserID;
			return this;
		}
	}

	return Login(conn);
}

void Foodie::order_history(SAConnection& conn) {
	int orderId, quantity, foodId;
	double amount;
	string title;
	string status;
	SACommand find(&conn), find2(&conn);
	find.setCommandText(_TSA("SELECT * FROM ORD WHERE CustomerID = :1"));
	find << (unsigned short)UserID;
	find.Execute();

	while (find.FetchNext())
	{
		orderId = find.Field(_TSA("OrderID")).asUShort();
		foodId = find.Field(_TSA("FoodID")).asUShort();
		quantity = find.Field(_TSA("Quantity")).asUShort();
		amount = find.Field(_TSA("Amount")).asDouble();
		status = find.Field(_TSA("Status")).asString().GetMultiByteChars();

		find2.setCommandText(_TSA("SELECT Title FROM Recipes WHERE RecipeID = :1"));
		find2 << (unsigned short) foodId;
		find2.Execute();
		while (find2.FetchNext())
		{
			title = find2.Field(_TSA("Title")).asString().GetMultiByteChars();
		}
		cout << "\n\nOrder Id: " << orderId << "\nTitle: " << title << "\nQuantity: " << quantity << "\nAmount: " << amount << "\nStatus: " << status << "\n\n";
	}
}