#include <iostream>
#include <vector>
#include <string>
#include "easyjson.hpp"

int main(int argc, char* argv[]) {

  std::cout << "parse obj:" << std::endl;
  const char *input = "{\"age\":12, \"name\":\"lzp\", \"gender\":\"man\"}";
  auto jsonObj = parseObject(input);

  std::cout << "parse arr:" << std::endl;
  const char* arrInput = "[1, \"ok\", {\"age1\",13}]";
  auto jsonArr = parseArray(arrInput);

  return 0;
}
