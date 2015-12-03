//
//  WindowServer.hpp
//  eos
//
//  Created by Alexandre Arsenault on 2015-11-26.
//  Copyright © 2015 axLib. All rights reserved.
//

#ifndef WindowServer_hpp
#define WindowServer_hpp

#include <axLib/axLib.h>
#include <iostream>
#include <cstdio>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

struct Patate {
	int a, b, c;
};

struct Message {
	unsigned int type;
};

namespace ws {
	enum Event {
		TEST_MESSAGE = 8000,
		CREATE_FRAME
	};
}

class session : public boost::enable_shared_from_this<session> {
public:
	session(boost::asio::io_service& io_service, ax::Event::Object& obj)
		: socket_(io_service)
		, _obj(obj)
	{
	}

	boost::asio::local::stream_protocol::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		socket_.async_read_some(
			boost::asio::buffer(data_),
			boost::bind(&session::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(
		const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error) {
			
			if (bytes_transferred >= sizeof(Message) + sizeof(ax::Point)) {
				
				Message* msg = (Message*)data_.data();
				ax::Rect* p = (ax::Rect*)(data_.data() + sizeof(Message));

				_obj.PushEvent(ws::Event::CREATE_FRAME,
					new ax::Event::SimpleMsg<ax::Rect>(*p));
			}
			else {
				Message* msg = (Message*)data_.data();
				std::cout << "msg : " << msg->type << std::endl;

				socket_.async_read_some(
					boost::asio::buffer(data_),
					boost::bind(&session::handle_read_data, shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}
	}

	void handle_read_data(
		const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error) {
			ax::Rect* p = (ax::Rect*)(data_.data());
			std::cout << "DATA" << std::endl;
			_obj.PushEvent(ws::Event::CREATE_FRAME,
						   new ax::Event::SimpleMsg<ax::Rect>(*p));

		}

		std::cout << "DATA END" << std::endl;
	}

	void handle_write(const boost::system::error_code& error)
	{
		//		if (!error) {
		//
		//			socket_.async_read_some(boost::asio::buffer(data_),
		//									boost::bind(&session::handle_read,
		//            shared_from_this(),
		//            boost::asio::placeholders::error,
		//            boost::asio::placeholders::bytes_transferred));
		//		}

		//		if (!error) {
		//			socket_.async_read_some(boost::asio::buffer(data_),
		//									boost::bind(&session::handle_read,
		//            shared_from_this(),
		//            boost::asio::placeholders::error,
		//            boost::asio::placeholders::bytes_transferred));
		//		}
	}

private:
	// The socket used to communicate with the client.
	boost::asio::local::stream_protocol::socket socket_;

	ax::Event::Object& _obj;

	// Buffer used to store data received from the client.
	boost::array<char, 1024> data_;
};

typedef boost::shared_ptr<session> session_ptr;

class server {
public:
	server(boost::asio::io_service& io_service, const std::string& file,
		ax::Event::Object& obj)
		: io_service_(io_service)
		, acceptor_(
			  io_service, boost::asio::local::stream_protocol::endpoint(file))
		, _obj(obj)
	{
		session_ptr new_session(new session(io_service_, _obj));

		acceptor_.async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session,
								   boost::asio::placeholders::error));
	}

	void handle_accept(
		session_ptr new_session, const boost::system::error_code& error)
	{
		if (!error) {
			new_session->start();
		}

		new_session.reset(new session(io_service_, _obj));
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session,
								   boost::asio::placeholders::error));
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::local::stream_protocol::acceptor acceptor_;
	ax::Event::Object& _obj;
};

#endif /* WindowServer_hpp */
