#pragma once

#include "util\FactoryMethod\FactoryMethod.h"
#include "SettingsManager.h"

using namespace Util;


class SettingsManagerCreator : FactoryMethod<SettingsManager>
{
public:
	SettingsManagerCreator();
	~SettingsManagerCreator();

	// ������������ ����� FactoryMethod
	virtual SettingsManager * Create() override;
};

