//
// client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <chrono>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "appdynamics.h"

using boost::asio::ip::tcp;

class client
{
public:
  client(boost::asio::io_service& io_service,
      const std::string& server, const std::string& port, const std::string& path)
    : resolver_(io_service),
      socket_(io_service)
  {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, port);
    resolver_.async_resolve(query,
        boost::bind(&client::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
  }

private:
  void handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // Attempt a connection to each endpoint in the list until we
      // successfully establish a connection.
      boost::asio::async_connect(socket_, endpoint_iterator,
          boost::bind(&client::handle_connect, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_connect(const boost::system::error_code& err)
  {
    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_,
          boost::bind(&client::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_write_request(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_, "\r\n",
          boost::bind(&client::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_read_status_line(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cout << "Invalid response\n";
        return;
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code ";
        std::cout << status_code << "\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
          boost::bind(&client::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

  void handle_read_headers(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        std::cout << header << "\n";
      std::cout << "\n";

      // Write whatever content we already have to output.
      if (response_.size() > 0)
        std::cout << &response_;

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

  void handle_read_content(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Write all of the data that has been read so far.
      std::cout << &response_;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }
  }

  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
};

int main(int argc, char* argv[])
{
  unsigned int    req_cnt = 0;
  int             winks = 20;
  int             initRC;
  appd_config     cfg;

  const char APP_NAME[] = "CES_89333_Client";
  const char TIER_NAME[] = "CES_89333_Client_T1";
  const char NODE_NAME[] = "CES_89333_Client_T1_N1";
  const char CONTROLLER_HOST[] = "osxltrkane";
  const int CONTROLLER_PORT = 8090;
  const char CONTROLLER_ACCOUNT[] = "customer1";
  const char CONTROLLER_ACCESS_KEY[] = "d060f41b-ef56-433d-b2cf-0219ea2018e9";
  const int CONTROLLER_USE_SSL = 0; 
#ifdef _WIN32
  const int PROXY_CTRL_PORT = 10101;
  const int PROXY_REQ_PORT = 10102;
  const int PROXY_REP_PORT = 10103;
#endif
  
  appd_config_init(&cfg);

  cfg.app_name = (char*)APP_NAME;
  cfg.tier_name = (char*)TIER_NAME;
  cfg.node_name = (char*)NODE_NAME;
  cfg.controller.host = (char*)CONTROLLER_HOST;
  cfg.controller.port = CONTROLLER_PORT;
  cfg.controller.account = (char*)CONTROLLER_ACCOUNT;
  cfg.controller.access_key = (char*)CONTROLLER_ACCESS_KEY;
  cfg.controller.use_ssl = CONTROLLER_USE_SSL;
  cfg.init_timeout_ms = 4000;
#ifdef _WIN32
  cfg.appd_proxy_config.tcp_control_port = PROXY_CTRL_PORT;
  cfg.appd_proxy_config.tcp_request_port = PROXY_REQ_PORT;
  cfg.appd_proxy_config.tcp_reporting_port = PROXY_REP_PORT;
#endif
 
  initRC = appd_sdk_init(&cfg);

  if (initRC)
  {
    std::cerr <<  "Error: sdk init: " << initRC << ". Check that proxy is still running.";
    return 1;
  }

  try
  {
    if (argc != 4)
    {
      std::cout << "Usage: httpclient <server> <port> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  httpclient www.boost.org 80 /LICENSE_1_0.txt\n";
      return 1;
    }

    const std::string host(argv[1]);
    const std::string port(argv[2]);
    const std::string url(argv[3]);

    while (1)
    {
      appd::sdk::BT bt(url);

      bt.set_url(host);
	  bt.add_user_data("host", host);
	  bt.add_user_data("port", port);
	  bt.add_user_data("junk", "junky");

      std::cout << "About to request \"" << url << "\" [#" << req_cnt++ << "] on server: " << host << " port: " << port << std::endl << ">>>>>" << std::endl;

      { // Exit call block...
      boost::asio::io_service io_service;
      client c(io_service, argv[1], argv[2], argv[3]);
      appd::sdk::ExitCall ec(bt, url);
      io_service.run();
      }

      std::cout << "<<<<<" << std::endl << std::endl << "Sleeping for " << winks << " seconds..." << std::endl;
      std::this_thread::sleep_for (std::chrono::seconds(winks));
    }
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  appd_sdk_term();

  return 0;
}
