#pragma once

#include <string>
#include <list>

using std::string;
using std::list;

class Person;
typedef list<Person> persons;
typedef persons::iterator persons_iter;

class Person
{
public:
	Person() {}
	Person(int id, string lastName, string firstName);
	string getFirstName() const;
	void setFirstName(const string& name);
	string getLastName() const;
	void setLastName(const string& name);
private:
	string m_firstName;
	string m_lastName;
};