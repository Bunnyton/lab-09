#include <iostream>
#include <vector>
#include <future>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <Handler.hpp>

#define DIR "html"

namespace po = boost::program_options;
void console_parse(int ac, char *av[], std::string &sp, unsigned &dthread_amount, unsigned &pthread_amount, unsigned &depth, std::string &out_path) {
  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "output,O", po::value<std::string>(&out_path), "output file")(
      "damount,D",
      po::value<unsigned>(&dthread_amount)
          ->default_value(std::thread::hardware_concurrency()),
      "amount of download thread")(
      "pamount,P",
      po::value<unsigned>(&pthread_amount)
          ->default_value(std::thread::hardware_concurrency()),
      "amount of parsing thread")("depth,L",
                          po::value<unsigned>(&depth)->default_value(1),
                          "depth parsing")("start_page,S",
                          po::value<std::string>(&sp), "start page");
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    exit(0);
  }
}



int main(int ac, char *av[])
{
  unsigned dthread_amount;
  unsigned pthread_amount;
  unsigned depth;
  std::string start_page;
  std::string output;

  console_parse(ac, av, start_page, dthread_amount, pthread_amount, depth, output);

  sleep(3);
  // The SSL context is required, and holds certificates
  ssl::context ctx{ssl::context::sslv23_client};

  // This holds the root certificate used for verification
  load_root_certificates(ctx);

  // Verify the remote server's certificate
  ctx.set_verify_mode(ssl::verify_peer);

  std::filesystem::remove_all(DIR);
  boost::filesystem::create_directory(DIR);

  ThreadPool dpool(dthread_amount);
  ThreadPool ppool(pthread_amount);

  std::vector<std::future<void>> futs;

  thread_data data(ctx, futs, dpool, ppool, output);

  start(start_page, data, depth);

  try {
    for (;;) {
      if (is_threads_end(data))
        break;
      else
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (std::exception &ec) {
      std::cout << ec.what() << std::endl;
      exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}