#include "ContactsWindow.h"

ContactsWindow::ContactsWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	connect(ui.btnSettings, SIGNAL(clicked()), this, SLOT(Settings_Click()));
	connect(ui.btnHostChat, SIGNAL(clicked()), this, SLOT(HostChat_Click()));
	connect(ui.btnJoinChat, SIGNAL(clicked()), this, SLOT(JoinChat_Click()));

	connect(&_sessionManager, SIGNAL(SessionCreated()), this, SLOT(SessionCreated()));

	connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(RemoveUser()));
	//connect(&_sessionManager, SIGNAL(UserConnected()), this, SLOT(Host_UserConnected()));
	refreshUsersList();
}

ContactsWindow::~ContactsWindow()
{
}

void ContactsWindow::Settings_Click()
{
	auto settingsWindow = new SettingsWindow();
	settingsWindow->exec();
}

void ContactsWindow::JoinChat_Click()
{
	//��� Id �����, ��������� �� ������ �� �����
	_sessionManager.ConnectToUser(0);
}

void ContactsWindow::HostChat_Click()
{
	_sessionManager.CreateChat();
}

void ContactsWindow::SessionCreated()
{
	ui.btnHostChat->setEnabled(false);

	ChatWindow* chat = new ChatWindow(_sessionManager.GetSession());
	chat->show();
}

void ContactsWindow::refreshUsersList()
{
	// �������� ������
	ui.lstUsers->clear();

	// ��������� ������������� �� �����
	_um.LoadFromFile();

	// �������� ���� �������������
	auto users = _um.GetAllUsers();

	// ���������� ������������� � �������
	for (auto user : users)
	{		
		ui.lstUsers->addItem(user.second._name.c_str());		
	}
}

void ContactsWindow::RemoveUser()
{
	// �������� ��� ������������ - ����
	string userName = ui.lstUsers->currentItem()->text().toStdString();

	// ������� ������������
	_um.Remove(userName);

	// ������������� ������������� - ��������� � ���������
	_um.ReloadUsers();

	// �������� ������ �������������
	refreshUsersList();
}
