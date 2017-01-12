#pragma once

#include "util\FactoryMethod\FactoryMethod.h"
#include "UserManager.h"
using namespace Util;

class UserManagerCreator: FactoryMethod<UserManager>
{
public:
	UserManagerCreator();
	~UserManagerCreator();

	// ������������ ����� FactoryMethod
	virtual UserManager * Create() override;
};

