#ifndef __SERVER_MESSAGE_HPP__
#define __SERVER_MESSAGE_HPP__
#include <iostream>
#include "util/buffer_manipulator.hpp"

#include "network.hpp"

namespace YGOpen
{
namespace Legacy
{

class STOCMessage
{
public:
	enum { HEADER_LENGTH = 3, MAX_MESSAGE_LENGTH = 1024 };
	STOCMessage(StoC::Msg msgType) :
		type(msgType),
		bm(tmpData, MAX_MESSAGE_LENGTH)
	{}

	char* GetDataPtr()
	{
		return data;
	}

	int GetLength() const
	{
		return length;
	}
	
	BufferManipulator* GetBM()
	{
		return &bm;
	}

	void Encode()
	{
		int msgLength = bm.GetCurrentLength(); 
		length = msgLength + HEADER_LENGTH;

		BufferManipulator tmpBm(data, length);
		tmpBm.Write<uint16_t>(msgLength + 1);
		tmpBm.Write<uint8_t>((uint8_t)type);

		tmpBm.Write(bm);
	}
private:
	uint16_t length;
	StoC::Msg type;
	char data[MAX_MESSAGE_LENGTH];
	char tmpData[MAX_MESSAGE_LENGTH]; // Optimize this into a single value
	BufferManipulator bm;
};

class CTOSMessage
{
public:
	enum { HEADER_LENGTH = 3, MAX_MESSAGE_LENGTH = 1024 };
	
	char* GetDataPtr()
	{
		return data;
	}

	size_t GetMsgLength() const
	{
		return (size_t)length;
	}

	CtoS::Msg GetMsgType() const
	{
		return type;
	}

	// figure length of the message
	// and type
	bool DecodeHeader()
	{
		BufferManipulator bm(data, HEADER_LENGTH);

		length = bm.Read<uint16_t>();
		length--; // Not sure why the client adds 1 to the length

		type = (CtoS::Msg)bm.Read<uint8_t>();

		return true;
	}
private:
	CtoS::Msg type;
	uint16_t length;
	char data[MAX_MESSAGE_LENGTH];
};

} // namespace Legacy
} // namespace YGOpen

#endif // __SERVER_MESSAGE_HPP__
