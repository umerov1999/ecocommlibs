#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>
#include "EcoPacket.h"


/// Serves stock quote information to any client that connects to it.
class CEcoSocketServer
{
public:
	CEcoSocketServer(boost::asio::io_service& io_service, const std::string& strHost, const std::string& strPort);
	~CEcoSocketServer(void);

private:
	connection m_connection;
	std::vector<CEcoPacket> m_vecPacket;
	boost::system::error_code m_errorCode;

private:
	void SetErrorCode(const boost::system::error_code& e);
	void handle_connect(const boost::system::error_code& e);
	void handle_read(const boost::system::error_code& e);
	void handle_write(const boost::system::error_code& e);

public:
	BOOL GetCommandData(CEcoPacket& packet);
	void SendResponseData(CEcoPacket& packet);
	boost::system::error_code GetErrorCode();
	void Close(void);

};