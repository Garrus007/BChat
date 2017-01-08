#pragma once

#include "IUserManager.h"

class UserManager : IUserManager
{
public:
	UserManager();
	~UserManager();

	// ������������ ����� IUserManager
	virtual vector<User> GetUsers() override;
	virtual User GetUser(uint32_t id) override;
private:
	vector<User> _users;
};

