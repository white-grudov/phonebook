#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>

using std::string;
using std::vector;
using std::array;
using std::map;

using PhoneNumber = array<char, 12>;

inline std::ostream& operator<<(std::ostream& os, PhoneNumber number)
{
	string result;
	for (char ch : number)
		result += ch;
	result = "+" + result.substr(0, 3) + " (" + result.substr(3, 2) + ") " + result.substr(5);
	return os << result;
}

class PhoneBook
{
public:
	struct Record
	{
		string name;
		vector<PhoneNumber> numbers;
	};
	PhoneBook() : _isRunning(true) {}
	~PhoneBook() = default;

	void run();
private:
	static const map<string, int> _commands;

	vector<Record> _data;
	bool _isRunning;

	void help();
	void add();
	void remove();
	void update(); 
	void remove_number();
	void find_by_name();
	void find_by_number();
	void show() const;
	void save();
	void load();
	void clear();
	void exitPhoneBook();

	PhoneNumber validate(string&);
	void showNoNumbers() const;
	void showContact(unsigned) const;
	vector<int> doFindByName(string&) const;
	vector<int> doFindByNumber(const string&) const;
	string toLowerCase(const string&) const;
	vector<string> split(string, string);

	PhoneBook(const PhoneBook&) = delete;
	PhoneBook& operator=(const PhoneBook&) = delete;
};

#endif