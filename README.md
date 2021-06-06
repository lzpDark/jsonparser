# jsonparser

An easy implement of json-parser.



## Usage

#### parse string into object

```cpp
const char *input = "{\"subObj\":{\"subAge\":13}, \"age\":12, \"name\":\"lzp\", \"gender\":\"man\", \"good\":true, \"hungry\":false, \"tag\":null}";
auto jsonObj = parseObject(input);
```

#### get string from object

```cpp
JsonObject obj;
obj.addValue("num_key", 1);
obj.addValue("false_key", false);
std::cout << toJson(&obj) << std::endl;
```

