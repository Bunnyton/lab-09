#ifndef LAB_09_HANDLER_HPP
#define LAB_09_HANDLER_HPP

#include <gumbo.h>
#include <fstream>
#include <iostream>
#include <future>
#include <boost/asio/ssl/context.hpp>
#include <vector>
#include <filesystem>
#include <fstream>

#include <https_client.hpp>
#include "ThreadPool.h"
#include "CmdArgs.h"

  void start(CmdArgs *data);

  bool is_threads_end(CmdArgs *data);


  static void save(std::string str, CmdArgs *data, std::string host);

  static void picture_search(GumboNode* node, CmdArgs *data, unsigned count, const std::string &host);

  static void parse(const std::string &filename, CmdArgs *data, unsigned count, const std::string &host);

  static void downloader(std::string &req, CmdArgs *data, unsigned count);

  static void download (const std::string &s_host , std::string &s_target
                         , CmdArgs *data, unsigned count);

#endif  // LAB_09_HANDLER_HPP
