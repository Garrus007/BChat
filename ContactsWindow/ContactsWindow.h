#pragma once

#include <QWidget>
#include "ui_ContactsWindow.h"

#include "Session\SessionManager\SimpleSessionManager.h"
#include "util\SettingsManager\SettingsManagerContainer.h"
#include "SettingsWindow\SettingsWindow.h"

#include "ChatWindow\ChatWindow.h"

class ContactsWindow : public QWidget
{
	Q_OBJECT

public:
	ContactsWindow(QWidget *parent = Q_NULLPTR);
	~ContactsWindow();

private:
	Ui::ContactsWindow ui;

	SimpleSessionManager _sessionManager;

public slots:
	void Settings_Click();
	void JoinChat_Click();
	void HostChat_Click();

	void SessionCreated();
};
