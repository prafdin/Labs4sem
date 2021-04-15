#pragma once

#include <QTextEdit>
#include <QString>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

#include <vector>
#include <string>

#include "protocol.h"

class MainWindow : public QWidget {
	Q_OBJECT

private:
	QTextEdit* _history;
	QTextEdit* _message;
	QPushButton* _buttonSend;
	QPushButton* _buttonConnect;
	QLabel* _status;
	QLineEdit* _address;
	QLineEdit* _port;
	QTimer _timer;

	std::vector<std::string> _messages;

private slots:
	void try_connect();
	void get_messages();
	void send_message();
	void responded(Protocol::QueryType queryType, Protocol::Buffer& response);
	void failed();
	
private:
	QString toQString(const std::vector<std::string>& ref);
public:
	MainWindow();

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
};