#include <iostream>
#include <vector>
#include <string>
#include "easyjson.hpp"

int main(int argc, char* argv[]) {

  const char *input = "{\"age\":12, \"name\":\"lzp\", \"gender\":\"man\"}";

  auto tokens =  tokenize(input);
  for(auto& token : tokens) {
//    std::cout << token.type << " " << token.value << std::endl;
  }
  auto jsonObj = parseObject(tokens);

  return 0;
}
