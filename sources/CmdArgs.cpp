#include "CmdArgs.h"

CmdArgs::~CmdArgs() {
  delete futs;
  delete dpool;
  delete ppool;
  delete fpool;
  delete path;
  delete sp;
}

void CmdArgs::parse(int ac, char *av[]) {

  path = new std::string();
  sp = new std::string();

  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")
      ( "output,O",
       po::value<std::string>(path)
           ->default_value(std::string("out.txt"))
      , "output file")

      ("damount,D", po::value<unsigned>(&dthread_amount)
           ->default_value(std::thread::hardware_concurrency()),
       "amount of download thread")

      ( "pamount,P", po::value<unsigned>(&pthread_amount)
            ->default_value(std::thread::hardware_concurrency()),
        "amount of parsing thread")

      ("depth,L", po::value<unsigned>(&depth)
          ->default_value(1),
       "depth parsing")

      ("start_page,S", po::value<std::string>(sp)
          ->default_value(std::string("https://habr.com/ru/post/234173/"))
         ,"start page");

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  dpool = new ThreadPool(dthread_amount);
  ppool = new ThreadPool(pthread_amount);
  fpool = new ThreadPool(1);
  futs = new std::vector<std::future<void>>();

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    exit(0);
  }
}
