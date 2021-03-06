#ifndef LAB_09_HTTPS_CLIENT_HPP
#define LAB_09_HTTPS_CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include "root_certificates.hpp"

#define _OK 200
#define DIR "html"
#define PORT "443"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class session : public std::enable_shared_from_this<session>
{
  tcp::resolver resolver_;
  beast::ssl_stream<beast::tcp_stream> stream_;
  beast::flat_buffer buffer_; // (Must persist between reads)
  http::request<http::empty_body> req_;
  http::response<http::string_body> res_;

 public:
  explicit session( net::any_io_executor ex,
      ssl::context& ctx)
      : resolver_(ex)
      , stream_(ex, ctx)
  { }

  // Start the asynchronous operation
  void run(
      char const* host,
      char const* target,
      int version);


  void on_resolve(
      beast::error_code ec,
      tcp::resolver::results_type results);

  void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);

  void on_handshake(beast::error_code ec);

  void on_write(
      beast::error_code ec,
      std::size_t bytes_transferred);

  void on_read(
      beast::error_code ec,
      std::size_t bytes_transferred);

  void on_shutdown(beast::error_code ec);
};

#endif  // LAB_09_HTTPS_CLIENT_HPP
