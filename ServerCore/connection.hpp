//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_CONNECTION_HPP
#define HTTP_SERVER3_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"


namespace servercore {
class IHandler
{
public:
	IHandler(size_t head_len,size_t body_len)
	{
		buf_head_.assign(head_len, 0);
		buf_body_.assign(body_len, 0);
	};
	virtual ~IHandler() {};
	virtual void readHeadHandler(std::vector<uint8_t> buf) = 0;
	virtual std::vector<uint8_t> getBufHead()
	{
		return buf_head_;
	}
	virtual void readBodyHandler(std::vector<uint8_t> buf) = 0;
	virtual std::vector<uint8_t> getBufBody()
	{
		return buf_body_;
	}
protected:
	std::vector<uint8_t> buf_head_;
	std::vector<uint8_t> buf_body_;
};

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_context.
  explicit connection(boost::asio::io_context& io_context,
	  IHandler& handler);

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();
  void readHead(/*const boost::system::error_code& e*/);

  void readBody(/*const boost::system::error_code& e*/);
private:
  /// Handle completion of a read operation.
  //void handle_read(const boost::system::error_code& e,
  //    std::size_t bytes_transferred);

  ///// Handle completion of a write operation.
  //void handle_write(const boost::system::error_code& e);

	
  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_context::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The handler used to process the incoming request.
  //request_handler& request_handler_;

  /// Buffer for incoming data.
  std::vector<uint8_t> buffer_;

  std::vector<uint8_t> buf_head_;
  std::vector<uint8_t> buf_body_;
  /// The incoming request.
  request request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  reply reply_;

  IHandler& handler_;
};

typedef boost::shared_ptr<connection> connection_ptr;


} // namespace http

#endif // HTTP_SERVER3_CONNECTION_HPP
