#include "MainMenu.h"

bool cook_toggle = 0;
bool foodie_toggle = 0;
SAConnection conn;

User* user_ptr;
MainMenu::MainMenu() {
	cook_toggle = 0;
	foodie_toggle = 0;
}
MainMenu::~MainMenu() {
	//dtor
}

void MainMenu::highlights() {
	cout << "showing highlights\n\n";
	byTitle(conn, " WHERE RecipeID IN (SELECT RecipeID FROM (SELECT * FROM Rating_chart ORDER BY Rating DESC) WHERE ROWNUM <= 5)");

}
void MainMenu::browse() {
	browse_interface(conn);
}
void MainMenu::tips() {

	cout << "\n**Tips & Tricks**\n\n";

	while (1) {

		srand(time(0));
		int random_num = rand() % 30;
		ifstream tips_text("tnt.txt");
		string line;
		while (random_num--)
			getline(tips_text, line);
		cout << line << endl;

		cout << "\n\nOne more? (y/n)...";
		string t;
		cin >> t;
		if (t == "N" || t == "n") {
			tips_text.close();
			break;
		}

		cout << "\n\n";
	}
}

void MainMenu::login() {

	if (cook_toggle == 0 && foodie_toggle == 0) {
		user_ptr = login_interface(conn);
	}
	else {
		cout << "redirecting to profile page\n\n";
		Sleep(50);
		system("CLS");
		//user_ptr->get_type();
		cout << "\n\n";
		user_ptr->profile(conn);
	}
}

void main_menu()
{
	conn.Connect(_TSA("localhost:1521/xepdb1"), _TSA("BFadmin"), _TSA("foodislove"), SA_Oracle_Client); //cout << "We are connected!\n";

	SACommand count(&conn);
	count.setCommandText(_TSA("SELECT UserID FROM Users"));
	count.Execute();
	while (count.FetchNext()) {
		User::UserID = count.Field(_TSA("UserID")).asUShort();
	}
	count.setCommandText(_TSA("SELECT RecipeID FROM Recipes"));
	count.Execute();
	while (count.FetchNext()) {
		Recipe::RecipeID = count.Field(_TSA("RecipeID")).asUShort();
	}
	/*count.setCommandText(_TSA("SELECT OrderID FROM Orders"));
	count.Execute();
	while (count.FetchNext()) {
		Order::OrderID = count.Field(_TSA("OrderID")).asUShort();
	}*/


	MainMenu menu;

	while (1) {
		system("PAUSE");
		system("CLS");
		cout << "\n\n\n\t\t\t\t\t ========== BongoFoodies ==========\n\n";

		cout << "1.Highlights\t\t2.Browse\t\t3.Tips & Tricks\t\t";
		if (cook_toggle == 0 && foodie_toggle == 0)
			cout << "4.Register/Login\t\t";
		else
			cout << "4." << user_ptr->getUserName() << "\t\t";
		cout << "5.Exit\n\n";

		cout << "\nEnter choice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			menu.highlights();
			break;
		case 2:
			menu.browse();
			break;
		case 3:
			menu.tips();
			break;
		case 4:
			menu.login();
			break;
		case 5: {
			cout << "Come again :')\n\n";
			conn.Disconnect();	//cout << "We are disconnected!\n";
		}
			  return;
		}
	}
}


HomeCook hc;
Foodie f;
User* login_interface(SAConnection& conn)
{
	cout << "\n\n1.New User\n2.Existing User\n\n";
	int choice;
	string t;
	cout << "Choice: ";
	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "\n\nDo you wanna publish your recipes? (y/n)... ";
		cin >> t;

		if (t == "y" || t == "Y") {
			return hc.Register(conn);
		}
		else {
			return f.Register(conn);
		}

		break;


	case 2:
		cout << "\n\nAre you a HomeCook? (y/n)... ";
		cin >> t;

		if (t == "y" || t == "Y") {
			return hc.Login(conn);
		}
		else {
			return f.Login(conn);
		}

		break;
	}
}


void User::get_type()
{
	if (typeid(*this) == typeid(HomeCook))
		cout << "hc";
	else if (typeid(*this) == typeid(Foodie))
		cout << "foo";
	else
	{
		cerr << "\nBad employee type";
		exit(1);
	}
}