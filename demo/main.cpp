#include <iostream>
#include <shared_mutex>
#include <vector>
#include <future>
#include <filesystem>
#include <boost/filesystem.hpp>

#include "ThreadPool.h"
#include "parse.hpp"
#include "https_client.hpp"
#include "download.h"

#define DIR "html"

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


int main()
{
//    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;
  // The SSL context is required, and holds certificates
  ssl::context ctx{ssl::context::sslv23_client};

  // This holds the root certificate used for verification
  load_root_certificates(ctx);

  // Verify the remote server's certificate
  ctx.set_verify_mode(ssl::verify_peer);

  boost::filesystem::path dir(DIR);
  boost::filesystem::create_directory(dir);


    ThreadPool dpool{1};
    ThreadPool ppool(4);

    std::vector<std::future<void>> futs;


    std::string str{"https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/crawl/http_crawl.cpp"};



    futs.emplace_back(dpool.enqueue(downloader, ctx, str, futs
                                                ,dpool , ppool));
    for(;;) {
      break;
    }


    // Run the I/O service. The call will return when
    // the get operation is complete.
    //auto start_link = "link.saldfsalj";
    //  Queue queue(start_link);

    //  auto futs = make_ready_future<void>(12);
    //  auto pfuts = make_ready_future<void>(15);

    //  for(;;){
    //    if (not (is_running(pfuts) and queue.is_have_task()))
    //      break;
    //
    //    for ( auto &item : pfuts) {
    //      if (thread_ready(item))
    //        t qg = queue.get;
    //        if (qg)
    //          item = std::async(std::launch::async, method, qg....);
    //    }
    //    for ( auto &item : futs) {
    //      if (thread_ready(item))
    //        t qg = queue.get;
    //      if (qg)
    //        item = std::async(std::launch::async, method, qg....);
    //    }
    //  }
//    sleep(5);
//    auto filename = "http_client_sync_ssl.cpp";
//    try {
//      for (auto &i : target) if (i == '/') i = '|';
//
//      parse("/" + target + DIR);
//    } catch (std::exception &ec) {
//      std::cout << ec.what() << std::endl;
//      exit(EXIT_FAILURE);
//    }

    return EXIT_SUCCESS;
}