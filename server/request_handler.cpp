//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace http {
namespace server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
  static unsigned int req_cnt = 0;

  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    std::cout << "Could not decode URL." << std::endl;   //RKane
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  std::cout << std::endl << "Got request " << req_cnt++ << ": " << request_path << std::endl;   //RKane

  // Request path must be absolute and not contain "..".
  if (request_path.empty())
  {
    std::cout << "Request path is empty." << std::endl;   //RKane
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  if (request_path[0] != '/')
  {
    std::cout << "Request: \"" << request_path << "\" must start with \"/\"." << std::endl;   //RKane
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  if (request_path.find("..") != std::string::npos)
  {
    std::cout << "Request: \"" << request_path << "\" may not contain \"..\" within path." << std::endl;   //RKane
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {
    std::cout << "Request: \"" << request_path << "\" is a directory, adding index.html to pathname" << std::endl;   //RKane
    request_path += "index.html";
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
    std::cout << "Extension is \"" << extension << "\"" << std::endl;   //RKane
  }

  // Open the file to send back.
  std::string full_path = doc_root_ + request_path;
  std::cout << "Full path is \"" << full_path << "\"" << std::endl;   //RKane
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    rep = reply::stock_reply(reply::not_found);
    std::cout << "Could not open file." << std::endl;   //RKane
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
