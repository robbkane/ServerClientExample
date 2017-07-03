//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"
#include "appdynamics.h"



int main(int argc, char* argv[])
{
    int                  rc;
    appd_config          cfg;

    const char APP_NAME[]   = "CES_Example";
    const char TIER_NAME[] = "CES_Example_ServerTier";
    const char NODE_NAME[] = "CES_Example_ServerTier_Node1";

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

    const std::string host(argv[1]);
    const std::string port(argv[2]);
    const std::string url(argv[3]);

    const char JAVA_BACKEND_NAME[] = "CES_Example_Java_Backend_Name";
    const char SERVERIO_BACKEND_NAME[] = "CES_Example_ServerIO_Backend_Name";

    appd_config_init(&cfg);

    cfg.app_name = (char*)APP_NAME;
    cfg.tier_name = (char*)TIER_NAME;
    cfg.node_name = (char*)NODE_NAME;

    cfg.controller.host = (char*)CONTROLLER_HOST;
    cfg.controller.port = CONTROLLER_PORT;
    cfg.controller.account = (char*)CONTROLLER_ACCOUNT;
    cfg.controller.access_key = (char*)CONTROLLER_ACCESS_KEY;
    cfg.controller.use_ssl = CONTROLLER_USE_SSL;
    cfg.init_timeout_ms = 60000;

#ifdef _WIN32
    cfg.appd_proxy_config.tcp_control_port = PROXY_CTRL_PORT;
    cfg.appd_proxy_config.tcp_request_port = PROXY_REQ_PORT;
    cfg.appd_proxy_config.tcp_reporting_port = PROXY_REP_PORT;
#endif

    rc = appd_sdk_init(&cfg);
    
    if (rc)
    {
        std::cerr <<  "Error: sdk init() failed, rc == " << rc << ". Check that proxy is still running." << std::endl;
        return 1;
    }
    
	// Set up the Java backend context...
	try
	{
	    std::cout << "Setting up backend call type \"" << APPD_BACKEND_HTTP << "\" name: \"" << JAVA_BACKEND_NAME << "\"" << std::endl;
        appd_backend_declare(APPD_BACKEND_HTTP, JAVA_BACKEND_NAME);
    
        rc = appd_backend_set_identifying_property(JAVA_BACKEND_NAME, "HOST", argv[1]);
    
        if (rc)
        {
            std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
                      << "\", \"HOST\", \"" << argv[1] << "\")" << rc << std::endl;
			throw rc;
        }
    
        rc = appd_backend_set_identifying_property(JAVA_BACKEND_NAME, "PORT", argv[2]);
    
        if (rc)
        {
            std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
                      << "\", \"PORT\", \"" << argv[2] << "\")" << rc << std::endl;
			throw rc;
        }
    
        rc = appd_backend_set_identifying_property(JAVA_BACKEND_NAME, "URL",  argv[3]);
    
        if (rc)
        {
            std::cerr <<  "Error: appd_backend_set_identifying_property(\"" << APPD_BACKEND_HTTP
                      << "\", \"URL\", \"" << argv[3] << "\")" << rc << std::endl;
			throw rc;
        }
    
        rc = appd_backend_add(JAVA_BACKEND_NAME);
    
    	if (rc)
    	{
    		std::cerr <<  "Error: appd_backend_add(\"" << JAVA_BACKEND_NAME << "\")" << rc << std::endl;
			throw rc;
    	}
	}
	catch (int rc)
	{
			std::cerr << " rc == " << rc << ", Exiting prematurely." << std::endl;
    	appd_sdk_term();
    	return 1;
	}
    catch (std::exception& e)
    {
        std::cerr << " exception: " << e.what() << "\n";
	    appd_sdk_term();
        return 1;
    }

    try
    {
        // Check command line arguments.
        if (argc != 4)
        {
          std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
          std::cerr << "  For IPv4, try:\n";
          std::cerr << "    receiver 0.0.0.0 80 .\n";
          std::cerr << "  For IPv6, try:\n";
          std::cerr << "    receiver 0::0 80 .\n";
          return 1;
        }

        const std::string arg1 = argv[1];
        const std::string arg2 = argv[2];
        const std::string arg3 = argv[3];

        // Initialise the server.
        http::server::server s(arg1, arg2, arg3);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
	    appd_sdk_term();
        return 1;
    }

	appd_sdk_term();
    return 0;

} // end of main().

// end of main.cpp
