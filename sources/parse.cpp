#include "parse.hpp"

static void picture_search(GumboNode* node) {

  if (node->type != GUMBO_NODE_ELEMENT)
    return;

  GumboAttribute* atr;
  if (node->v.element.tag == GUMBO_TAG_IMG &&
      (atr = gumbo_get_attribute(&node->v.element.attributes, "src"))) {
    std::cout << atr->value << std::endl;
  }
  else if (node->v.element.tag == GUMBO_TAG_A &&
           (atr = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    std::cout << atr->value << std::endl;
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
    picture_search(static_cast<GumboNode*>(children->data[i]));
}


void parse(const std::string &filename) {

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