#ifndef LAB_09_HANDLER_HPP
#define LAB_09_HANDLER_HPP

#include "Handler.hpp"
#include <gumbo.h>
#include <fstream>
#include <iostream>
#include <future>
#include <boost/asio/ssl/context.hpp>
#include <vector>
#include <filesystem>
#include <fstream>
#include <shared_mutex>

#include <https_client.hpp>

#include "ThreadPool.h"

struct thread_data{
  ssl::context &ctx;
  std::vector<std::future<void>> &futs;
  ThreadPool &dpool;
  ThreadPool &ppool;
  ThreadPool &fpool;
  std::string &path;
 public:
  thread_data( ssl::context &_ctx,
               std::vector<std::future<void>> &_futs,
               ThreadPool &_dpool,
               ThreadPool &_ppool,
               ThreadPool &_fpool,
               std::string &_path) : ctx(_ctx), futs(_futs), dpool(_dpool),
                                    ppool(_ppool), fpool(_fpool), path(_path) {
  }
};

  void start(std::string &req, thread_data &data, unsigned count);

  bool is_threads_end(thread_data &data);


  static void save(std::string *str, thread_data &data);

  static void picture_search(GumboNode* node, thread_data &data, unsigned count, const std::string &host);

  static void parse(const std::string &filename, thread_data &data, unsigned count, const std::string &host);

  static void downloader(std::string &req, thread_data &data, unsigned count);

  static void download (const std::string &s_host , std::string &s_target
                         , thread_data &data, unsigned count);

#endif  // LAB_09_HANDLER_HPP
