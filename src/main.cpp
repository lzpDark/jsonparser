#include <iostream>
#include <vector>
#include <string>
#include "easyjson.hpp"

void testObj() {
  std::cout << "parse obj:" << std::endl;
  const char *input = "{\"subObj\":{\"subAge\":13.5}, \"age\":12, \"name\":\"lzp\", \"gender\":\"man\", \"good\":true, \"hungry\":false, \"tag\":null}";
  auto jsonObj = parseObject(input);

  std::cout << "json:" << std::endl
    << toJson(jsonObj) << std::endl;
}

void testObj2Str() {
  JsonObject obj;
  obj.addValue("num_key", 1);
  obj.addValue("false_key", false);
  obj.addValue("null_key", new JsonNull);
  obj.addValue("string_key", "string_value");
  JsonObject subObj;
  subObj.addValue("subInt", 13);
  obj.addValue("sub_obj_key", &subObj);
  JsonArray subArr;
  subArr.addItem(13);
  obj.addValue("sub_arr_key", &subArr);
  std::cout << toJson(&obj) << std::endl;
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
//      item->print();
    }
  }
  std::cout << "json:" << std::endl
    << toJson(jsonArr) << std::endl;
}

void testArr2Str() {
  JsonArray arr;
  arr.addItem(1);
  arr.addItem(false);
  arr.addItem("string_value");
  JsonObject subObj;
  subObj.addValue("sub_obj_int_key", 13);
  arr.addItem(&subObj);
  JsonArray subArr;
  subArr.addItem(11);
  arr.addItem(&subArr);
  std::cout << toJson(&arr) << std::endl;

}


int main(int argc, char* argv[]) {

  testObj();
  testObj2Str();
  testArray();
  testArr2Str();
  return 0;
}
