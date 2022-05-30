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

#include <https_client.hpp>

#include "ThreadPool.h"

class Handler {
 private:
  static ssl::context &_ctx;
  static std::vector<std::future<void>> &_futs;
  static ThreadPool &_dpool;
  static ThreadPool &_ppool;

 public:
  Handler(ssl::context &ctx,
          std::vector<std::future<void>> &futs,
          ThreadPool &dpool,
          ThreadPool &ppool) : _ctx(ctx), _futs(futs)
                               , _dpool(dpool), _ppool(ppool){}

  void Handler::start(std::string &req);

  bool is_thread_end();

  static void picture_search(GumboNode* node);

  static void parse(const std::string &filename);

  static void downloader(std::string &req );

  static void download (std::string &s_host , std::string &s_target )
};

#endif  // LAB_09_HANDLER_HPP
