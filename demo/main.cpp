#include <iostream>
#include <shared_mutex>

#include <parse.hpp>

struct Body{
  std::string link;
  std::string path;
  Body(std::string &link_): link(std::move(link_)) {}
};

struct Queue_body {
  Body body;
  Queue_body* next;

  Queue_body() : next(nullptr) {}
  Queue_body(std::string &link_) :  body(link_), next(nullptr) {}
};

class Queue {
 private:
//  std::shared_mutex = new std::shared_mutex; #FIXME разобраться с mutex и shared_ptr
  Queue_body* _head;
  Queue_body* _last;
  Queue_body* _front_link;


 public:
  Queue() : _head(new Queue_body()){
    _last = _head;
    _front_link = _head;
  }

  void html_add_link(std::string &link_){
    auto* qb = new Queue_body(link_);
    _last->next = qb;
    _last = _last->next;
  }

  std::string html_get_body() { return _last->body; }

  void parse_add_path(std::string &s) {
    _front_link
  }

//  parse_get_path

};

int main() {
  auto start_link = "link.saldfsalj";
  Queue queue(start_link);

  for(;;){
    queue.get_
  }
  auto filename = "pse.html";
  try {
    parse(filename);
  } catch (std::exception &ec) {
    std::cout << ec.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}