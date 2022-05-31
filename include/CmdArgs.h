#ifndef LAB_09_CMDARGS_H
#define LAB_09_CMDARGS_H

#include <boost/program_options.hpp>
#include <ThreadPool.h>
#include <boost/asio/ssl/context.hpp>
#include <iostream>

namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace po = boost::program_options;

struct CmdArgs {
    ssl::context &ctx;
    std::vector<std::future<void>> *futs;
    ThreadPool *dpool;
    ThreadPool *ppool;
    ThreadPool *fpool;
    std::string *path;
    unsigned dthread_amount;
    unsigned pthread_amount;
    unsigned depth;
    std::string *sp; // start page

   public:
    CmdArgs(ssl::context &_ctx) : ctx(_ctx){ }

    ~CmdArgs(); //владеет всеми объектами

    void parse(int ac, char *av[]);

  };

#endif  // LAB_09_CMDARGS_H
