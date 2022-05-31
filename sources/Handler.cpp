#include "Handler.hpp"

void start(CmdArgs *data) {
  data->futs->emplace_back(data->dpool->enqueue(downloader,
                                              *data->sp, data, data->depth));
}

bool thread_ready(std::future<void> &fut) {
  if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    return true;
  return false;
}

bool is_threads_end(CmdArgs *data) {
  for(auto &i : *data->futs)
    if (!thread_ready(i)) return false;
  return true;
}

void to_full_link(std::string &link, const std::string &host){
  if (link.front() == '/')
    link = "https://" + host + link;
}

void save(std::string str, CmdArgs *data, std::string host) {
    to_full_link(str, host);
    std::ofstream stream(*data->path, std::ios::app);
    if (!stream) throw std::logic_error("File " + *data->path + " not found\n");
    stream << str << std::endl;
    stream.close();
}

void picture_search(GumboNode* node, CmdArgs *data, unsigned count, const std::string &host) {
  if (node->type != GUMBO_NODE_ELEMENT)
    return;

  GumboAttribute* atr;
  if (node->v.element.tag == GUMBO_TAG_IMG &&
      (atr = gumbo_get_attribute(&node->v.element.attributes, "src")))
  {
    data->futs->emplace_back(data->fpool->enqueue(save, atr->value, data, host));
  }
  else if (count > 0 && node->v.element.tag == GUMBO_TAG_A &&
           (atr = gumbo_get_attribute(&node->v.element.attributes, "href")))
  {
    std::string req = atr->value;
    to_full_link(req, host);
    data->futs->emplace_back(data->dpool->enqueue(downloader, req, data, count - 1));
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
    picture_search(static_cast<GumboNode*>(children->data[i]), data, count, host);
}


void parse(const std::string &path, CmdArgs *data, unsigned count, const std::string &host) {

  //std::string path = "/" + filename;
  //path = DIR + path;

  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (!in) {
    throw std::logic_error("File " + path + " not found\n");
  }

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput* output = gumbo_parse(contents.c_str());
  picture_search(output->root, data, count, host);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

void downloader(std::string &req, CmdArgs *data, unsigned count) {
  std::string host = req.substr(req.find("//") + 2);
  host = host.erase(host.find('/'));
  std::string target = req.substr(req.find("//") + 2);
  target = target.substr(target.find('/'));
  download(host, target, data, count);
}

void download (const std::string &s_host , std::string &s_target, CmdArgs *data, unsigned count) {
  auto const host = s_host.c_str();
  auto const target = s_target.c_str();
  auto const version = 11;

  auto change_target = std::string(target);

  for (auto &i : change_target) if (i == '/') i = '|';
  change_target = "/" + change_target;
  change_target = DIR + change_target;

  if (std::filesystem::exists(change_target))
    return;

  // The io_context is required for all I/O
  net::io_context ioc;


  // Launch the asynchronous operation
  // The session is constructed with a strand to
  // ensure that handlers do not execute concurrently.
  std::make_shared<session>(
      net::make_strand(ioc),
      data->ctx )->run(host, target, version);
  ioc.run();
  data->futs->emplace_back(data->ppool->enqueue(parse, change_target, data
                                                        , count, s_host));
}
