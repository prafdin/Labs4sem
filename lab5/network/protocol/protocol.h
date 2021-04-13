#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <vector>

struct Protocol final {
	using Byte = unsigned char;
	using Buffer = std::deque<Byte>;

	static const std::size_t UNSIGNED_SIZE = 4;

	static void writeUnsigned(Buffer& buffer, std::size_t value);
	static std::size_t readUnsigned(Buffer& buffer, bool extractBytes = true);

	static void writeData(Buffer& buffer, const std::string& value);
	static std::string readData(Buffer& buffer);

	static bool canParse(Buffer& buffer);

	// ���� ��������-�������
	enum class QueryType : std::size_t {
		GetMessages,
		SendMessage
	};

	struct GetMessages final {
		static Buffer makeRequest(std::size_t fromIndex);
		static std::size_t parseRequest(Buffer& buffer);
		static Buffer makeResponse(const std::vector<std::string>& messages);
		static std::vector<std::string> parseResponse(Buffer& buffer);
	};

	struct SendMessage final {
		static Buffer makeRequest(const std::string& str);
		static std::string parseRequest(Buffer& buffer);
		static Buffer makeResponse(std::size_t index);
		static std::size_t parseResponse(Buffer& buffer);
	};
};