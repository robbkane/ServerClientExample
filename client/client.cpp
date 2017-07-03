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
      const std::string& server, const std::string& port, const std::string& path, const std::string& appd_correlation_header)
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

    request_stream << "singularityheader: " << appd_correlation_header << "\r\n";  // RKane

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
  unsigned int           req_cnt = 0;
  int                    winks = 3;
  int                    rc;
  appd_config            cfg;

  // appd_context_config    app_context2;

  // Topology is ApacheWeb=>C++Client=>C++Server=>JavaApp

  const char APP_NAME[] = "CES_Example";
  const char TIER_NAME[] = "CES_Example_ClientTier";
  const char NODE_NAME[] = "CES_Example_ClientTier_Node1";

  const char CONTROLLER_HOST[] = "osxltrkane";
  const int  CONTROLLER_PORT = 8090;
  const char CONTROLLER_ACCOUNT[] = "customer1";
  const char CONTROLLER_ACCESS_KEY[] = "64ede871-3750-4b63-ae7f-ec0a7413e456";
  const int  CONTROLLER_USE_SSL = 0; 

#ifdef _WIN32
  const int PROXY_CTRL_PORT = 10101;
  const int PROXY_REQ_PORT = 10102;
  const int PROXY_REP_PORT = 10103;
#endif
  
  const char SERVER_BACKEND_NAME[]    = "CES_Example_Server_Backend_Name";
  const char SERVER_TIER_NAME[]       = "CES_Example_ServerTier";
  const char SERVER_NODE_NAME[]       = "CES_Example_ServerTier_Node1";

  const char APACHE_CONTEXT_APP_NAME[]  = "CES_Example";
  const char APACHE_CONTEXT_TIER_NAME[] = "CES_Example_ApacheTier";
  const char APACHE_CONTEXT_NODE_NAME[] = "CES_Example_ApacheTier_Node1";

  const std::string host(argv[1]);
  const std::string port(argv[2]);
  const std::string url(argv[3]);

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
 
  // app_context2.app_name  = APACHE_CONTEXT_APP_NAME;
  // app_context2.tier_name = APACHE_CONTEXT_TIER_NAME;
  // app_context2.node_name = APACHE_CONTEXT_NODE_NAME;

  try
  {
    if (argc != 4)
    {
      std::cout << "Usage: httpclient <server> <port> <path>" << std::endl;
      std::cout << "Example:" << std::endl;
      std::cout << "  httpclient www.boost.org 80 /LICENSE_1_0.txt" << std::endl;
      return 1;
    }

	std::cout << "Primary context:  (originating at this Tier)" << std::endl
			  << "   Application: \"" << cfg.app_name           << std::endl
			  << "   Tier Name:   \"" << cfg.tier_name          << std::endl
			  << "   Node Name:   \"" << cfg.node_name          << std::endl;

	// std::cout << "Secondary Context: (from Apache Tier)"        << std::endl
	// 		  << "   Application: \"" << app_context2.app_name  << std::endl
	// 		  << "   Tier Name:   \"" << app_context2.tier_name << std::endl
	// 		  << "   Node Name:   \"" << app_context2.node_name << std::endl;

    rc = appd_sdk_init(&cfg);

    if (rc)
    {
      std::cerr <<  "Error: sdk init() call returned " << rc << ". Check that proxy is still running." << std::endl;
      return 1;
    }

	// rc = appd_config_add_app_context(&cfg, APACHE_CONTEXT_APP_NAME, &app_context2);

	// All requests go to the C++Server Tier.
    appd_backend_declare(APPD_BACKEND_HTTP, SERVER_BACKEND_NAME);

    rc = appd_backend_set_identifying_property(SERVER_BACKEND_NAME, "HOST", argv[1]);

	if (rc)
	{
      std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
			    << "\", \"HOST\", \"" << argv[1] << "\") returned " << rc << std::endl;
      return 1;
	}

    rc = appd_backend_set_identifying_property(SERVER_BACKEND_NAME, "PORT", argv[2]);

	if (rc)
	{
      std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
			    << "\", \"PORT\", \"" << argv[2] << "\") returned " << rc << std::endl;
      return 1;
	}

    rc = appd_backend_set_identifying_property(SERVER_BACKEND_NAME, "URL",  argv[3]);

	if (rc)
	{
      std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
			    << "\", \"URL\", \"" << argv[3] << "\") returned " << rc << std::endl;
      return 1;
	}

    rc = appd_backend_add(SERVER_BACKEND_NAME);

	if (rc)
	{
      std::cerr <<  "Error: appd_backend_add(\"" << APPD_BACKEND_HTTP << "\") returned " << rc << std::endl;
      return 1;
	}

    while (1)
    {
	  { // BT call block...

          appd::sdk::BT bt(url);
    
          bt.set_url(host);
          bt.add_user_data("host", host);
          bt.add_user_data("port", port);
          bt.add_user_data("sample_user_data", "client_mcclientface");
    
    	  { // Exit call block...

            appd::sdk::ExitCall ec(bt, SERVER_BACKEND_NAME);
    
            const std::string& appd_correlation_header = ec.get_correlation_header();
    
    		// Only need this if we are getting things from Apache.
    		// auto handle = appd_bt_begin_with_app_context(APACHE_CONTEXT_APP_NAME,"bt2", appd_correlation_header.c_str());
    
            // std::cout << "appd_bt_begin_with_app_context(\"" 
    		// 		  << APACHE_CONTEXT_APP_NAME << "\", \"bt2\", " 
    		// 		  << appd_correlation_header.c_str() 
    		// 		  << ") returned " << handle << std::endl;
    		//
    	
            // appd_bt_end(handle);
    		//
    
            std::cout << "About to make request " << req_cnt++ 
                      << " URL: \"" << url << "\""
                      << " on server: " << host 
                      << " port: " << port 
                      << " with correlation header: \"" << appd_correlation_header << "\"."
                      << std::endl << ">>>>>" << std::endl;
    
            boost::asio::io_service io_service;
            client c(io_service, argv[1], argv[2], argv[3], appd_correlation_header);
            io_service.run();
    
    	  } // end of Exit call block

          std::this_thread::sleep_for (std::chrono::milliseconds(1));
		  // bt.override_time_ms(7);

	  } // end of BT call block
      
      std::cout << "<<<<<" << std::endl << std::endl << "Sleeping for " << winks << " seconds..." << std::endl;
      std::this_thread::sleep_for (std::chrono::seconds(winks));

    } // end of while (1)     
	
	// when we exit the above loop, we are done.

  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  appd_sdk_term();

  return 0;

} // end of main().


// end of client.cpp
