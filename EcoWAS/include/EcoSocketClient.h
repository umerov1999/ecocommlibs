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


class CEcoSocketClient
{
public:
	CEcoSocketClient(boost::asio::io_service& io_service, unsigned short port, CString strUserID);
	~CEcoSocketClient(void);

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	connection m_connection;
	boost::system::error_code m_errorCord;
	std::vector<CEcoPacket> m_vecPacket;
	CString m_strUserID;

private:
	void SetErrorCode(const boost::system::error_code& e);
	void handle_accept(const boost::system::error_code& e);
	void handle_read(const boost::system::error_code& e);
	void handle_write(const boost::system::error_code& e);
	void handle_close_write(const boost::system::error_code& e);

public:
	void SendCommand(CEcoPacket& packet);
	void SendCloseCommand();
	BOOL GetResponseData(CEcoPacket& packet);
	boost::system::error_code GetErrorCode();
	void Close();
};