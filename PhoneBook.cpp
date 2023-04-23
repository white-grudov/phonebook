#include "PhoneBook.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::left;
using std::setw;

const map<string, int> PhoneBook::_commands = {
	{ "help",           0 },
	{ "add",            1 },
	{ "remove",         2 },
	{ "update",         3 },
	{ "remove_number",  4 },
	{ "find_by_name",   5 },
	{ "find_by_number", 6 },
	{ "show",           7 },
	{ "save",           8 },
	{ "load",           9 },
	{ "clear",          10 },
	{ "exit",           11 }
};

void PhoneBook::run()
{
	cout << "Welcome to the Phonebook application!\nType \"help\" to see available commands.\n";
	while (_isRunning)
	{
		cout << "\nType a command:";
		cout << "\n> ";
		string input;
		cin >> input;

		int command;
		try
		{
			command = _commands.at(input);
		}
		catch (std::out_of_range)
		{
			command = -1;
		}

		switch (command)
		{
		case 0:
			help();
			break;
		case 1:
			add();
			break;
		case 2:
			remove();
			break;
		case 3:
			update();
			break;
		case 4:
			remove_number();
			break;
		case 5:
			find_by_name();
			break;
		case 6:
			find_by_number();
			break;
		case 7:
			show();
			break;
		case 8:
			save();
			break;
		case 9:
			load();
			break;
		case 10:
			clear();
			break;
		case 11:
			exitPhoneBook();
			break;
		default:
			cout << "Invalid command! Please try again.\n";
			break;
		}
	}
}

void PhoneBook::help()
{
	cout << "\nTo use phonebook, type a number that corresponds to a command:\n\n";

	cout << "add            - Add contact\n";
	cout << "remove         - Remove contact\n";
	cout << "update         - Add number to the existing contact\n";
	cout << "remove_number  - Remove number from the existing contact\n";
	cout << "find_by_name   - Find contact by name\n";
	cout << "find_by_number - Find contact by number\n";
	cout << "show           - Show numbers in the phonebook\n";
	cout << "save           - Save phonebook to file\n";
	cout << "load           - Load phonebook from file\n";
	cout << "clear          - Delete all contacts from phonebook\n";
	cout << "exit           - Exit the program\n\n";

	cout << "Type \"cancel\" to cancel current operation\n";
}

void PhoneBook::add()
{
	cout << "\nEnter the name of a contact.";
	cout << "\n> ";
	string name;
	cin >> name;
	if (name == "cancel") return;

	vector<int> contact = doFindByName(name);
	if (!contact.empty())
	{
		cout << "Contact already exists!\n";
		return;
	}

	vector<PhoneNumber> currentNumbers;
	bool enteringNumbers = true;
	while (enteringNumbers)
	{
		cout << "Enter the 12-digit number.";
		cout << "\n> ";
		string number;
		cin >> number;
		if (number == "cancel") return;

		PhoneNumber validatedNumber(validate(number));
		if (validatedNumber[0] == 0)
		{
			cout << "Invalid number!\n";
			continue;
		}
		else
		{
			currentNumbers.push_back(validatedNumber);
			cout << "Number was added successfully.\nDo you want to add one more number? (y/n):";
			cout << "\n> ";
			string answer;
			cin >> answer;
			if (tolower(answer[0]) == 'y') continue;
			else enteringNumbers = false;
		}
		_data.push_back(Record{ name, currentNumbers });
		cout << "Contact was added to the phonebook.\n";
	}
}

void PhoneBook::remove()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	showNoNumbers();

	unsigned contact = 0;
	bool enteringNumber = true;
	while (enteringNumber)
	{
		cout << "Enter the number of contact you want to remove:";
		cout << "\n> ";
		try
		{
			string temp;
			cin >> temp;
			if (temp == "cancel") return;
			contact = std::stoi(temp);
		}
		catch (std::exception)
		{
			cout << "Invalid number!\n";
			continue;
		}
		enteringNumber = false;
	}
	if (contact > _data.size())
	{
		cout << "Such contact does not exist!\n";
		return;
	}
	try
	{
		_data.erase(_data.begin() + contact - 1);
	}
	catch (std::exception)
	{
		cout << "Removing contact was unsuccessful.\n";
		return;
	}
	cout << "Contact was removed successfully\n";
}

void PhoneBook::update()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	showNoNumbers();

	unsigned contact = 0;
	bool enteringNumbers = true;
	while (enteringNumbers)
	{
		cout << "Enter the number of contact you want to add new phone number to:";
		cout << "\n> ";
		try
		{
			string temp;
			cin >> temp;
			if (temp == "cancel") return;
			contact = std::stoi(temp);
		}
		catch (std::exception)
		{
			cout << "Invalid number!\n";
			continue;
		}
		enteringNumbers = false;
	}
	if (contact > _data.size())
	{
		cout << "Such contact does not exist!\n";
		return;
	}

	vector<PhoneNumber> currentNumbers;
	enteringNumbers = true;
	while (enteringNumbers)
	{
		cout << "Enter the 12-digit number.";
		cout << "\n> ";
		string number;
		cin >> number;
		if (number == "cancel") return;

		PhoneNumber validatedNumber(validate(number));
		if (validatedNumber[0] == 0)
		{
			cout << "Invalid number!\n";
			continue;
		}
		else
		{
			currentNumbers.push_back(validatedNumber);
			cout << "Number was added successfully.\nDo you want to add one more number? (y/n):";
			cout << "\n> ";
			string answer;
			cin >> answer;
			if (tolower(answer[0]) == 'y') continue;
			else enteringNumbers = false;
		}
		_data[contact - 1].numbers.insert(_data[contact - 1].numbers.end(), currentNumbers.begin(), currentNumbers.end());
		cout << "Contact was updated successfully.\n";
	}
}

void PhoneBook::remove_number()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	showNoNumbers();

	unsigned contact = 0;
	bool enteringNumbers = true;
	while (enteringNumbers)
	{
		cout << "Enter the number of contact you want to delete new phone number from:";
		cout << "\n> ";
		try
		{
			string temp;
			cin >> temp;
			if (temp == "cancel") return;
			contact = std::stoi(temp);
		}
		catch (std::exception)
		{
			cout << "Invalid number!\n";
			continue;
		}
		enteringNumbers = false;
	}
	if (contact > _data.size())
	{
		cout << "Such contact does not exist!\n";
		return;
	}
	if (_data[contact - 1].numbers.size() == 1)
	{
		cout << "Contact has only one number! Use \"remove\" instead.\n";
		return;
	}

	showContact(contact - 1);
	unsigned phoneNo = 0;
	enteringNumbers = true;
	while (enteringNumbers)
	{
		cout << "Enter the number of phone number you want to delete:";
		cout << "\n> ";
		try
		{
			string temp;
			cin >> temp;
			if (temp == "cancel") return;
			phoneNo = std::stoi(temp);
		}
		catch (std::exception)
		{
			cout << "Invalid number!\n";
			continue;
		}
		enteringNumbers = false;
	}
	if (phoneNo > _data[contact - 1].numbers.size())
	{
		cout << "Such phone number does not exist!\n";
		return;
	}
	try
	{
		_data[contact - 1].numbers.erase(_data[contact - 1].numbers.begin() + phoneNo - 1);
	}
	catch (std::exception)
	{
		cout << "Removing phone number was unsuccessful.\n";
		return;
	}
	cout << "Phone number was removed successfully\n";
}

void PhoneBook::find_by_name()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	cout << "\nEnter the name you want to find:";
	cout << "\n> ";
	string name;
	cin >> name;
	if (name == "cancel") return;

	vector<int> contact = doFindByName(name);
	if (contact.empty())
	{
		cout << "Contact is not found.\n";
		return;
	}
	for (int con : contact)
		showContact(con);
}

void PhoneBook::find_by_number()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	bool enteringNumbers = true;

	test:
	while (enteringNumbers)
	{
		cout << "Enter the number.";
		cout << "\n> ";
		string number;
		cin >> number;
		if (number == "cancel") return;

		string chars = " +()-";
		for (char c : chars)
			number.erase(std::remove(number.begin(), number.end(), c), number.end());

		for (int i = 0; i < number.length(); ++i)
		{
			if (!(number[i] >= 48 && number[i] <= 57))
			{
				cout << "Invalid number!\n";
				goto test;
			}
		}
		vector<int> contact = doFindByNumber(number);
		if (contact.empty())
		{
			cout << "Contact is not found.\n";
			return;
		}
		for (int con : contact)
			showContact(con);
		enteringNumbers = false;

	}
}

void PhoneBook::show() const
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	int numberWidth = std::to_string(_data.size()).length() > 3 ? std::to_string(_data.size()).length() + 2 : 5;
	int nameWidth = 7;
	for (const auto& record : _data)
		nameWidth = record.name.length() >= (nameWidth - 3) ? record.name.length() + 3: nameWidth;

	cout << endl << left << setw(numberWidth) << " No. " << left << setw(nameWidth) << "| Name "
	     << left << "| Number" << endl;
	for (int i = 0; i < _data.size(); ++i)
	{
		cout << string(numberWidth + nameWidth + 20, '-') << endl;
		cout << left << setw(numberWidth) << ' ' + std::to_string(i + 1) + ' '
		     << left << setw(nameWidth) << "| " + _data[i].name
		     << left << "| " << _data[i].numbers[0] << endl;
		if (_data[i].numbers.size() > 1)
		{
			for (int j = 1; j < _data[i].numbers.size(); ++j)
			{
				cout << string(numberWidth, ' ') << '|' << string(nameWidth - 1, ' ') << '|'
				     << ' ' << _data[i].numbers[j] << endl;
			}
		}
	}
	cout << endl;
}

void PhoneBook::save()
{
	cout << "\nEnter the filename where you want to save the phonebook (format - example.txt):";
	cout << "\n> ";
	string name;
	cin >> name;
	if (name == "cancel") return;

	std::ofstream os;
	os.open(name);

	for (const auto& record : _data)
	{
		os << record.name << ':' << record.numbers[0];
		for (int i = 1; i < record.numbers.size(); ++i)
			os << ',' << record.numbers[i];
		os << endl;
	}
	os.close();

	cout << "File was saved successfully\n";
}

void PhoneBook::load()
{
	cout << "\nDo you want to save current phonebook first? (y/n):";
	cout << "\n> ";
	string answer;
	cin >> answer;
	if (tolower(answer[0]) == 'y') save();

	_data.clear();

	cout << "\nEnter the filename where you want to load phonebook from (format - example.txt):";
	cout << "\n> ";
	string name;
	cin >> name;
	if (name == "cancel") return;

	std::ifstream is;
	is.open(name);

	string data;
	while (std::getline(is, data))
	{
		vector<string> contact = split(data, ":");
		vector<PhoneNumber> numbers;
		vector<string> unvalidated = split(contact[1], ",");
		for (string number : unvalidated)
			numbers.push_back(validate(number));
		_data.push_back(Record{ contact[0], numbers });
	}
	cout << "Phonebook was loaded successfully\n";
}

void PhoneBook::clear()
{
	if (_data.empty())
	{
		cout << "\nPhonebook is empty.\n";
		return;
	}
	cout << "\nAre you sure you want to delete all contacts? (y/n):";
	cout << "\n> ";
	string answer;
	cin >> answer;
	if (tolower(answer[0]) == 'y') _data.clear();

	cout << "Phonebook was cleared successfully\n";
}

void PhoneBook::exitPhoneBook()
{
	cout << "\nDo you want to save current phonebook first? (y/n):";
	cout << "\n> ";
	string answer;
	cin >> answer;
	if (tolower(answer[0]) == 'y') save();

	_isRunning = false;
}

PhoneNumber PhoneBook::validate(string& number)
{
	PhoneNumber result = { 0 };

	string chars = " +()-";
	for (char c : chars)
		number.erase(std::remove(number.begin(), number.end(), c), number.end());
	if (number.length() == 12)
	{
		for (int i = 0; i < 12; ++i)
		{
			if (number[i] >= 48 && number[i] <= 57)
				result[i] = number[i];
			else return PhoneNumber({ 0 });
		}
	}
	return result;
}

void PhoneBook::showNoNumbers() const
{
	int numberWidth = std::to_string(_data.size()).length() > 3 ? std::to_string(_data.size()).length() + 2 : 5;
	int nameWidth = 7;
	for (const auto& record : _data)
		nameWidth = record.name.length() >= (nameWidth - 3) ? record.name.length() + 3 : nameWidth;

	cout << endl << left << setw(numberWidth) << " No. " << left << "| Name " << endl;
	for (int i = 0; i < _data.size(); ++i)
	{
		cout << string(numberWidth + nameWidth, '-') << endl;
		cout << left << setw(numberWidth) << ' ' + std::to_string(i + 1) + ' '
			<< left << setw(nameWidth) << "| " + _data[i].name << endl;
	}
	cout << endl;
}

void PhoneBook::showContact(unsigned num) const
{
	int numberWidth = std::to_string(_data.size()).length() > 3 ? std::to_string(_data.size()).length() + 2 : 5;
	int nameWidth = 7;
	for (const auto& record : _data)
		nameWidth = record.name.length() >= (nameWidth - 3) ? record.name.length() + 3 : nameWidth;

	cout << endl << left << setw(numberWidth) << " No. " << left << setw(nameWidth) << "| Name "
		<< left << "| Number" << endl;

	cout << string(numberWidth + nameWidth + 24, '-') << endl;
	cout << left << setw(numberWidth) << ' ' + std::to_string(num + 1) + ' '
		<< left << setw(nameWidth) << "| " + _data[num].name
		<< left << "| (1) " << _data[num].numbers[0] << endl;
	if (_data[num].numbers.size() > 1)
	{
		for (int i = 1; i < _data[num].numbers.size(); ++i)
		{
			cout << string(numberWidth, ' ') << '|' << string(nameWidth - 1, ' ') << '|'
				<< " (" << i + 1 << ") " << _data[num].numbers[i] << endl;
		}
	}
	cout << endl;
}

vector<int> PhoneBook::doFindByName(string& name) const
{
	vector<int> result;
	for (int i = 0; i < _data.size(); ++i)
		if (toLowerCase(_data[i].name).find(toLowerCase(name)) != string::npos)
			result.push_back(i);

	return result;
}

vector<int> PhoneBook::doFindByNumber(const string& number) const
{
	vector<int> result;
	for (int i = 0; i < _data.size(); ++i)
		for (const auto& num : _data[i].numbers)
		{
			string temp;
			for (const auto& digit : num)
				temp += digit;
			if (toLowerCase(temp).find(toLowerCase(number)) != string::npos)
				result.push_back(i);
		}

	return result;
}

string PhoneBook::toLowerCase(const string& str) const
{
	string result(str);
	std::transform(result.begin(), result.end(), result.begin(),
			[] (unsigned char c) { return std::tolower(c); });
	return result;
}

vector<string> PhoneBook::split(string text, string delimiter)
{
	vector<string> result;
	int pos = 0;
	string token;
	while ((pos = text.find(delimiter)) != std::string::npos) {
		token = text.substr(0, pos);
		result.push_back(token);
		text.erase(0, pos + delimiter.length());
	}
	result.push_back(text);
	return result;
}
