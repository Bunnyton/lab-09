#include "download.h"

void downloader(ssl::context &ctx
    , std::string &req
    , std::vector<std::future<void>> &futs
    , ThreadPool &dpool
    , ThreadPool &ppool) {
  std::string host = req.substr(req.find("//") + 2);
  host = host.erase(host.find('/'));
  std::string target = req.substr(req.find("//") + 2);
  target = target.substr(host.find('/') + 1);
  download(ctx, host, target, futs, dpool, ppool);
}

void download (ssl::context &ctx
    , std::string &s_host
    , std::string &s_target
    , std::vector<std::future<void>> &futs
    , ThreadPool &dpool
    , ThreadPool &ppool) {

  auto const host = s_host.c_str();
  auto const target = s_target.c_str();
  auto const version = 11;
  auto change_target = std::string(target);
  for (auto &i : change_target) if (i == '/') i = '|';
  if (std::filesystem::exists( + "/" + change_target ))
    return;

  // The io_context is required for all I/O
  net::io_context ioc;


  // Launch the asynchronous operation
  // The session is constructed with a strand to
  // ensure that handlers do not execute concurrently.
  std::make_shared<session>(
      net::make_strand(ioc),
      ctx )->run(host, target, version);
  ioc.run();
  futs.emplace_back(ppool.enqueue(parse, change_target));
}
