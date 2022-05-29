#include <iostream>
#include <shared_mutex>
#include <vector>
#include <future>

#include "parse.hpp"
#include "https_client.hpp"


//struct Body{
//  std::string link;
//  std::string path;
//  Body(std::string &link_): link(std::move(link_)) {}
//};
//
//struct Queue_body {
//  Body body;
//  Queue_body* next;
//
//  Queue_body() : next(nullptr) {}
//  Queue_body(std::string &link_) :  body(link_), next(nullptr) {}
//};
//
//class Queue {
// private:
////  std::shared_mutex = new std::shared_mutex; #FIXME разобраться с mutex и shared_ptr
//  Queue_body* _head;
//  Queue_body* _last;
//  Queue_body* _front_link;
//
//
// public:
//  Queue() : _head(new Queue_body()){
//    _last = _head;
//    _front_link = _head;
//  }
//
//  void html_add_link(std::string &link_){
//    auto* qb = new Queue_body(link_);
//    _last->next = qb;
//    _last = _last->next;
//  }
//
//  std::string html_get_body() { return _last->body; }
//
//  void parse_add_path(std::string &s) {
//    _front_link
//  }
//
////  parse_get_path
//
//};

bool thread_ready(std::future<void> &fut) {
  if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    return true;
  return false;
}

template <typename T>
std::vector<std::future<T>> make_ready_future(unsigned lot){
  std::vector<std::future<T>> futs(lot);
  for (auto &item : futs) {
    item = std::async([]() -> T {
        T t;
        return t;
    });
  }
  return futs;
}

std::vector<std::future<void>> make_ready_future(unsigned lot){
  std::vector<std::future<void>> futs(lot);
  for (auto &item : futs) {
    item = std::async([]() -> void {
      std::this_thread::sleep_for(std::chrono::seconds(0));
    });
  } return futs; }

bool is_running(std::vector<std::future<void>> vfuts) {
  for ( auto &item : vfuts)
    if (! thread_ready(item))
      return false;

  return true;
}

//int main() {
//  auto start_link = "link.saldfsalj";
////  Queue queue(start_link);
//
////  auto dfuts = make_ready_future<void>(12);
////  auto pfuts = make_ready_future<void>(15);
//
////  for(;;){
////    if (not (is_running(pfuts) and queue.is_have_task()))
////      break;
////
////    for ( auto &item : pfuts) {
////      if (thread_ready(item))
////        t qg = queue.get;
////        if (qg)
////          item = std::async(std::launch::async, method, qg....);
////    }
////    for ( auto &item : dfuts) {
////      if (thread_ready(item))
////        t qg = queue.get;
////      if (qg)
////        item = std::async(std::launch::async, method, qg....);
////    }
////  }
//  auto filename = "pse.html";
//  try {
//    parse(filename);
//  } catch (std::exception &ec) {
//    std::cout << ec.what() << std::endl;
//    exit(EXIT_FAILURE);
//  }
//}

int main(int argc, char** argv)
{
    // Check command line arguments.
//    if(argc != 4 && argc != 5)
//    {
//        std::cerr <<
//            "Usage: http-client-async-ssl <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n" <<
//            "Example:\n" <<
//            "    http-client-async-ssl www.example.com 443 /\n" <<
//            "    http-client-async-ssl www.example.com 443 / 1.0\n";
//        return EXIT_FAILURE;
//    }
//    auto const host = argv[1];
//    auto const port = argv[2];
//    auto const target = argv[3];
//    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;


    auto const host = "www.boost.org";
    auto const port = "443";
    auto const target = "/doc/libs/1_76_0/libs/beast/example/http/client/sync-ssl/http_client_sync_ssl.cpp";
    auto const version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::sslv23_client};

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // Launch the asynchronous operation
    // The session is constructed with a strand to
    // ensure that handlers do not execute concurrently.
    std::make_shared<session>(
        net::make_strand(ioc),
        ctx
        )->run(host, port, target, version);

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return EXIT_SUCCESS;
}