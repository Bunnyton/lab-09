#ifndef LAB_09_DOWNLOAD_H
#define LAB_09_DOWNLOAD_H

#include <iostream>
#include <vector>
#include <filesystem>

#include <https_client.hpp>
#include <ThreadPool.h>
#include <parse.hpp>

void downloader (ssl::context &ctx
    , std::string &req
    , std::vector<std::future<void>> &futs
    , ThreadPool &dpool
    , ThreadPool &ppool);

void download (ssl::context &ctx
    , std::string &s_host
    , std::string &s_target
    , std::vector<std::future<void>> &futs
    , ThreadPool &dpool
              , ThreadPool &ppool);

#endif  // LAB_09_DOWNLOAD_H
