#include "server.h"

#include <stdexcept>

#include "protocol.h"

using namespace std;

void Server::newConnection() {
	auto socket = _server.nextPendingConnection();
	if (socket == nullptr) {
		return;
	}

	_buffers[socket] = Protocol::Buffer();

	connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
	connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
}

void Server::readyRead() {
	auto socket = qobject_cast<QTcpSocket*>(sender());
	auto& buffer = _buffers[socket];
	if (socket->bytesAvailable() == 0) {
		return;
	}

	const auto data = socket->readAll();
	buffer.insert(buffer.end(), data.begin(), data.end());

	if (!Protocol::canParse(buffer)) {
		return;
	}

	// �������� ������ �������

	Protocol::readUnsigned(buffer); // ��������� ������ ���� ������� �� ������
	const auto queryType = (Protocol::QueryType)Protocol::readUnsigned(buffer);

	switch (queryType) {

	case Protocol::QueryType::GetMessages:
	{
		// ���������� ������� �� ������� ������
		const auto fromIndex = Protocol::GetMessages::parseRequest(buffer);

		// ������������ ������
		vector<string> messages;
		if (fromIndex < _messages.size()) {
			messages.insert(messages.end(), _messages.begin() + fromIndex, _messages.end());
		}

		// �������� ������ � ������ ������
		const auto responseBuffer = Protocol::GetMessages::makeResponse(messages);
		string responseData(responseBuffer.begin(), responseBuffer.end());

		// �������� ������
		socket->write(responseData.c_str(), responseData.size());

		break;
	}

	case Protocol::QueryType::SendMessage:
	{
		auto message = Protocol::SendMessage::parseRequest(buffer);

		_messages.push_back(message);
		auto index = _messages.size() - 1;

		const auto responseBuffer = Protocol::SendMessage::makeResponse(index);
		string responseData(responseBuffer.begin(), responseBuffer.end());

		socket->write(responseData.c_str(), responseData.size());

		break;
	}

	}

	socket->close();
}

void Server::disconnected() {
	auto socket = qobject_cast<QTcpSocket*>(QObject::sender());
	socket->deleteLater();
	_buffers.erase(socket);
}

Server::Server(const QHostAddress& address, quint16 port) {
	connect(&_server, &QTcpServer::newConnection, this, &Server::newConnection);

	if (!_server.listen(address, port)) {
		throw runtime_error(_server.errorString().toStdString());
	}
}

QHostAddress Server::address() const {
	return _server.serverAddress();
}

quint16 Server::port() const {
	return _server.serverPort();
}