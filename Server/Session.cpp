#include "Session.h"



s::Session::Session(): socket(nullptr)
{
}


s::Session::~Session()
{
}

bool s::Session::isUserLoggedIn() const
{
	return account != nullptr;
}

s::Account* s::Session::getAccount() const
{
	return account;
}

void s::Session::setAccount(s::Account * account)
{
	delete this->account;
	this->account = account;
}
