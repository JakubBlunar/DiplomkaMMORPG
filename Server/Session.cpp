#include "Session.h"



Session::Session(): socket(nullptr)
{
}


Session::~Session()
{
}

bool Session::isUserLoggedIn() const
{
	return account != nullptr;
}

s::Account* Session::getAccount() const
{
	return account;
}

void Session::setAccount(s::Account * account)
{
	delete this->account;
	this->account = account;
}
