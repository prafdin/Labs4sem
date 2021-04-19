#include "main_window.h"

#include <sstream>

#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "protocol.h"
#include "server_request.h"

using namespace std;

QString MainWindow::toQString(const std::vector<string>& ref) {
	QString string;
	for (auto& message : ref) {
		string += QString(message.c_str()) + QString("\n");
	}
	return string;
}

void MainWindow::try_connect() {
	get_messages();

	_buttonConnect->setEnabled(false);
}

void MainWindow::get_messages() {
	const auto buffer = Protocol::GetMessages::makeRequest(_messages.size());
	QHostAddress address(_address->text());
	const auto serverRequest = new ServerRequest(address, _port->text().toUInt(), buffer, 5000);
	QObject::connect(serverRequest, &ServerRequest::responded, this, &MainWindow::responded);
	QObject::connect(serverRequest, &ServerRequest::failed, this, &MainWindow::failed);
}

void MainWindow::send_message() {
	const auto buffer = Protocol::SendMessage::makeRequest(_message->toPlainText().toStdString() );
	QHostAddress address(_address->text());
	const auto serverRequest = new ServerRequest(address, _port->text().toUInt(), buffer, 5000);

	QObject::connect(serverRequest, &ServerRequest::responded, this, &MainWindow::responded);
	_messages.push_back(_message->toPlainText().toStdString());
	_message->clear();
	_history->setText(_history->toPlainText()+ _message->toPlainText()+"\n");

}

void MainWindow::responded(Protocol::QueryType queryType, Protocol::Buffer& response) {
	switch (queryType) {
	case Protocol::QueryType::GetMessages:
	{
		if (_messages.size() == 0) { //first request messages
			_status->setText(u8"Вы подключены к серверу");
			_success_conect = true;
			_timer.start();
			connect(&_timer, &QTimer::timeout, this, &MainWindow::get_messages);
		}
		const auto messages = Protocol::GetMessages::parseResponse(response);
		QString response;
		for (auto& message : messages) {
			_messages.push_back(message);
		}

		_history->setText(toQString(_messages));
		break;
	}

	case Protocol::QueryType::SendMessage:
	{
		const auto index = Protocol::SendMessage::parseResponse(response);
		if (_messages.size() - 1 != index) {
			_messages.erase(_messages.end() - 1);

			QHostAddress address(_address->text());
			const auto buffer = Protocol::GetMessages::makeRequest(_messages.size());
			const auto serverRequest = new ServerRequest(address, _port->text().toUInt(), buffer, 5000);
			QObject::connect(serverRequest, &ServerRequest::responded, this, &MainWindow::responded);
			_status->setText(u8"На сервере появились новые сообщения");
		}
		else {
			_status->setText(u8"Доставлено");
		}
		break;
	}

	default:
	{
		_history->setText(u8"Неизвестный тип ответа!");
	}

	}

	_buttonSend->setEnabled(true);

	sender()->deleteLater();
}

void MainWindow::failed() {
	if (_success_conect)
		return;
	_status->setText(u8"Не удалось подключиться");
	_buttonConnect -> setEnabled(true);

	sender()->deleteLater();
}


MainWindow::MainWindow() : _history(new QTextEdit), _message(new QTextEdit), _buttonSend(new QPushButton), _status(new QLabel),
_address(new QLineEdit), _port(new QLineEdit), _buttonConnect(new QPushButton)
{
	_buttonSend->setText(u8"Отправить сообщение");
	_buttonConnect->setText(u8"Подключиться");
	_status->setText(u8"Соединение не установлено");

	auto adressLayout = new QHBoxLayout;
	_address->setFixedSize(100, 25);
	_port->setFixedSize(40, 25);
	adressLayout->addWidget(_address);
	adressLayout->addWidget(_port);
	adressLayout->addWidget(_buttonConnect);
	adressLayout->setAlignment(_port, Qt::AlignLeft );
	adressLayout->setAlignment(_buttonConnect, Qt::AlignRight);


	auto messageLayout = new QHBoxLayout;
	_message->setFixedSize(150, 25);
	messageLayout->addWidget(_message);
	messageLayout->addWidget(_buttonSend);
	messageLayout->setAlignment(_buttonSend, Qt::AlignRight);

	const auto rootLayout = new QVBoxLayout;
	rootLayout->addWidget(_status);
	_history->setReadOnly(true);
	rootLayout->addWidget(_history);
	rootLayout->addLayout(messageLayout);
	rootLayout->addLayout(adressLayout);
	setLayout(rootLayout);

	_timer.setInterval(1000);

	QObject::connect(_buttonConnect, &QAbstractButton::clicked, this, &MainWindow::try_connect);
	QObject::connect(_buttonSend, &QAbstractButton::clicked, this, &MainWindow::send_message );
}
