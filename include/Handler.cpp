#include "Handler.hpp"

void Handler::start(std::string &req) {
  _futs.emplace_back(_dpool.enqueue(downloader, req));
}

bool thread_ready(std::future<void> &fut) {
  if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    return true;
  return false;
}

void Handler::is_threads_end() {
  for(auto &i : _futs){
    if (i.)
  }
}

void Handler::picture_search(GumboNode* node ) {

  if (node->type != GUMBO_NODE_ELEMENT)
    return;

  GumboAttribute* atr;
  if (node->v.element.tag == GUMBO_TAG_IMG &&
      (atr = gumbo_get_attribute(&node->v.element.attributes, "src"))) {
    std::cout << atr->value << std::endl;
  }
  else if (node->v.element.tag == GUMBO_TAG_A &&
           (atr = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::string req = atr->value;
    _futs.emplace_back(_dpool.enqueue(Handler::downloader, req));
    std::cout << "LINK" << atr->value << std::endl;
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
    picture_search(static_cast<GumboNode*>(children->data[i]));
}


void Handler::parse(const std::string &filename) {

  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (!in)
    throw std::logic_error("File " + filename + " not found\n");


  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput* output = gumbo_parse(contents.c_str());
  picture_search(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

void Handler::downloader(std::string &req) {
  std::string host = req.substr(req.find("//") + 2);
  host = host.erase(host.find('/'));
  std::string target = req.substr(req.find("//") + 2);
  target = target.substr(host.find('/') + 1);
  download(host, target);
}

void Handler::download (std::string &s_host , std::string &s_target) {

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
      _ctx )->run(host, target, version);
  ioc.run();
  _futs.emplace_back(_ppool.enqueue(Handler::parse, change_target));
}
