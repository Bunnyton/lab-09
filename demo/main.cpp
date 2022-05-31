#include <iostream>
#include <vector>
#include <future>
#include <boost/filesystem.hpp>

#include <Handler.hpp>
#include <CmdArgs.h>

#define DIR "html"


int main(int ac, char *av[])
{
  // The SSL context is required, and holds certificates
  ssl::context ctx{ssl::context::sslv23_client};
  // This holds the root certificate used for verification
  load_root_certificates(ctx);
  // Verify the remote server's certificate
  ctx.set_verify_mode(ssl::verify_peer);

  auto* data = new CmdArgs(ctx);
  data->parse(ac, av);

  std::filesystem::remove_all(DIR);
  boost::filesystem::create_directory(DIR);

  start(data);

  try {
    for (;;) {
      if (not_tasks(data))
        break;
      else
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (std::exception &ec) {
      std::cout << ec.what() << std::endl;
      delete data;
      exit(EXIT_FAILURE);
  }

  delete data;
  return EXIT_SUCCESS;
}