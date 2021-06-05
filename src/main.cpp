#include <iostream>
#include <vector>
#include <string>
#include "easyjson.hpp"

void testObj() {
  std::cout << "parse obj:" << std::endl;
  const char *input = "{\"subObj\":{\"subAge\":13}, \"age\":12, \"name\":\"lzp\", \"gender\":\"man\", \"good\":true, \"hungry\":false, \"tag\":null}";
  auto jsonObj = parseObject(input);

  auto subObj = jsonObj->getObjectValue("subObj");
  int subAge = subObj->getIntValue("subAge");
  int age =  jsonObj->getIntValue("age");
  std::string name =  jsonObj->getStringValue("name");
  std::string gender =  jsonObj->getStringValue("gender");
  bool good = jsonObj->getBooleanValue("good");
  bool hungry = jsonObj->getBooleanValue("hungry");
  auto nullObj =  jsonObj->getNullValue("tag");
  std::string tagType = nullObj->type;

  std::cout << "parsed-obj:" << std::endl 
    << " subAge = " << subAge
    << " age=" << age 
    << " name=" << name
    << " gender=" << gender
    << " good=" << good
    << " hungry=" << hungry
    << " tag=" << tagType
    << std::endl;
}

void testArray() {
  std::cout << "parse arr:" << std::endl;
  const char* arrInput = "[1, [2], \"ok\", {\"age1\": 13}, true, false, null]";
  auto jsonArr = parseArray(arrInput);
  int size = jsonArr->size();
  for(int i = 0; i<size; i++) {
    auto item = jsonArr->getItem(i);
    if(item == nullptr ){
     std::cout << "no such itme with idx = " << i << "; ";
    }else{
      item->print();
    }
  }
}


int main(int argc, char* argv[]) {

  testObj();
  testArray();
  return 0;
}
