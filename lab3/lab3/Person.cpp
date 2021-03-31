#include "Person.h"

string Person::getFirstName() const
{
	return m_firstName;
}
void Person::setFirstName(const string& firstName)
{
	m_firstName = firstName;
}
string Person::getLastName() const
{
	return m_lastName;
}
void Person::setLastName(const string& lastName)
{
	m_lastName = lastName;
}
