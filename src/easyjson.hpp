#include <iostream>
#include <vector>
#include <string>
#include <map>

struct Token {
  enum Type {
    BEGIN_OBJ = 1<<0,
    END_OBJ = 1<<1,
    BEGIN_ARR = 1<<2,
    END_ARR = 1<<3,
    COMMA = 1<<4,
    COLON = 1<<5,
    NUMBER = 1<<6,
    STRING = 1<<7,
    BOOLEAN = 1<<8,
    NULL_OBJ = 1<<9,
    END_ALL = 1<<10  
  };
  Type type;
  std::string value;

  Token(Token::Type t, const std::string& v):type(t), value(v) {

  }
};

std::vector<Token> tokenize(const char* input) {
  int cursor = 0;
  std::vector<Token> tokens;
  std::string value =  "";
  char ch = input[cursor];
  while(ch != '\0') { 
    switch(ch) {
      case '[':
        tokens.push_back(Token(Token::Type::BEGIN_ARR, "["));
        break;
      case ']':
        tokens.push_back(Token(Token::Type::END_ARR, "]"));
        break;
      case '{':
        tokens.push_back(Token(Token::Type::BEGIN_OBJ, "{"));
        break;
      case '}':
        tokens.push_back(Token(Token::Type::END_OBJ, "}"));
        break;
      case ',': 
        tokens.push_back(Token(Token::Type::COMMA, ","));
        break;
      case ':':
        tokens.push_back(Token(Token::Type::COLON, ":"));
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        value += ch; 
        cursor ++;
        ch = input[cursor];
        while(ch != '\0') {
          if(ch < '0' || ch > '9') {
            break;
          }
          value += ch;
          cursor++;
          ch = input[cursor];
        }
        cursor--;
        tokens.push_back(Token(Token::Type::NUMBER, value));
        break;
      case '"':
        cursor++; 
        ch = input[cursor];
        while(ch != '\0' && ch != '"') {
          value += ch;
          cursor++;
          ch = input[cursor];
        }
        tokens.push_back(Token(Token::Type::STRING, value)); 
        break;
      case 'n':
        {
          const std::string origin = "null";
          for(int i =0; i<origin.size(); i++){
            if(ch != origin[i]) {
              //error 
              std::cout << "tokenize error while parse null" << std::endl;
              return tokens;
            } 
            cursor++;
            ch = input[cursor];
          }
          cursor--;
          tokens.push_back(Token(Token::Type::NULL_OBJ, "null"));
        }
        break;
      case 't':
      case 'f':
        {
          const std::string origin = (ch == 't') ? "true" : "false";
          for(int i = 0; i<origin.size(); i++) {
            if(ch != origin[i]) {
              std::cout << "tokenize error while parse " << origin << std::endl;
              return tokens;
            }
            cursor++;
            ch = input[cursor];
          }
          cursor--;
          tokens.push_back(Token(Token::Type::BOOLEAN, origin));        
        }
        break;
      default:

        break;
    } // switch
    if(ch == '\0') break;
    cursor++;
    value = "";
    ch = input[cursor];
  } // while
  tokens.push_back(Token(Token::Type::END_ALL, ""));
  return tokens;
}

struct JsonValue;
struct JsonObject;
struct JsonArray;

std::string toJson(JsonValue* jsonValue);
std::string toJson(JsonObject* jsonObject);
std::string toJson(JsonArray* jsonArray);

struct JsonNull {
  JsonNull() {
    type = "null";
  }
  std::string type;
};

struct JsonValue {

  std::string type;
  int valueInt;
  std::string valueString;
  bool valueBoolean;
  JsonObject* valueObject;
  JsonArray* valueArray;
  JsonNull* valueNull;

    void print() {
    std::cout << type << ":";
    if("int" == type) 
    {
      std::cout << valueInt;
    }
    else if( "boolean" == type)
    {
      std::cout << valueBoolean;
    }
    else if( "string" == type)
    {
      std::cout << valueString;
    }
    else if( "object" == type)
    {

    }
    else if( "array" == type) 
    {

    }
    else if( "null" == type)
    {
      std::cout << "null";    
    }
    std::cout << ";";
  }

  JsonValue() {
   // default ? 
  }

  JsonValue(int v) {
    type = "int";
    valueInt = v;
  }

  JsonValue(const std::string& v) {
    type = "string";
    valueString = v;
  }

  JsonValue(bool v) {
    type = "boolean";
    valueBoolean = v;
  }

  JsonValue(JsonNull* v) {
    type = "null";
    valueNull = v;
  }

  JsonValue(JsonObject* v) { 
    type = "object";
    valueObject = v;
  }

  JsonValue(JsonArray* v) {  
    type = "array";
    valueArray = v;
  }

  int getIntValue(){
    if(type != "int"){
      return 0;
    } 
    return valueInt;
  }

  std::string getStringValue() {
    if(type != "string") {
      return "";
    }
    return valueString;
  }

  bool getBooleanValue(bool defaultV = false){
    if(type != "boolean"){
      return defaultV; // err
    }
    return valueBoolean; 
  }

  JsonObject* getObjectValue() {
    if(type != "object") {
      return nullptr;
    }
    return valueObject;
  }

  JsonArray* getArrayValue() {
    if(type != "array") {
      return nullptr;
    }
    return valueArray;
  }

  JsonNull* getNullValue() {
    if(type != "null") {
      return nullptr;
    }
    return valueNull;
  }
};

struct JsonObject {

  std::map<std::string, JsonValue*> jsonValueMap;

  void addValue(const std::string& key, int v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  void addValue(const std::string& key, std::string v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  void addValue(const std::string& key, JsonObject* v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  void addValue(const std::string& key, JsonArray* v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  void addValue(const std::string& key, bool v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  void addValue(const std::string& key, JsonNull* v) {
    jsonValueMap[key] = new JsonValue(v);
  }

  int getIntValue(const std::string& key) {
    return jsonValueMap[key]->getIntValue();
  }

  bool getBooleanValue(const std::string& key) {
    auto mapv = jsonValueMap[key]; 
    return mapv->getBooleanValue();
  }
  
  std::string getStringValue(const std::string& key) {
    return jsonValueMap[key]->getStringValue();
  }
  
  JsonObject* getObjectValue(const std::string& key) {
    return jsonValueMap[key]->getObjectValue();
  }
  
  JsonArray* getArrayValue(const std::string& key) {
    return jsonValueMap[key]->getArrayValue();
  }
  
  JsonNull* getNullValue(const std::string& key) {
    auto mapv = jsonValueMap[key];
    return mapv->getNullValue();
  }
};

struct JsonArray {

  std::vector<JsonValue*> jsonValueList;

  JsonArray() {
  
  }

  void addItem(int v) {
    jsonValueList.push_back(new JsonValue(v));
  }

  void addItem(const std::string& v) { 
    jsonValueList.push_back(new JsonValue(v));
  }

  void addItem(JsonObject* v) {  
    jsonValueList.push_back(new JsonValue(v));
  }

  void addItem(JsonArray* v) {
    jsonValueList.push_back(new JsonValue(v));
  };

  void addItem(JsonNull* v) {
    jsonValueList.push_back(new JsonValue(v));
  }

  void addItem(bool v){
    jsonValueList.push_back(new JsonValue(v));
  }

  JsonValue* getItem(int idx) {
    if(idx >=0 && idx < jsonValueList.size()) {
      return jsonValueList.at(idx);
    }
    return nullptr;
  }

  int size() {
    return jsonValueList.size();
  }
};

bool isTypeExpected(int expectedType, int type) {
  return (expectedType & type) > 0;
}

int stringToInt(const std::string& v) {
  int size = v.length();
  int res = 0;
  for(int i = 0; i<size; i++) {
    res = res *10 + (v[i] - '0');  
  }
  return res;
}

JsonObject* parseObject(std::vector<Token> &tokens, bool parseOneLevel = false) {
  JsonObject *resJsonObject = nullptr;
  JsonObject* jsonObject = nullptr;
  std::vector<JsonObject*> stack;
  std::string key = "";
  int preType = -1;
  int expectedType = Token::Type::BEGIN_OBJ; 
  while(!tokens.empty()) {
    auto token = tokens.at(0);
    const Token::Type type = token.type;
    switch(type) {
      case Token::Type::END_ALL:
        return resJsonObject;
      case Token::Type::BEGIN_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        } 
        jsonObject = new JsonObject;
        if(preType != -1) {
          stack.back()->addValue(key, jsonObject);
        }else{
          resJsonObject = jsonObject;
        }
        stack.push_back(jsonObject);
        
        expectedType = (Token::Type::STRING);
        preType = type;
        break;
      case Token::Type::END_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        }
        if(parseOneLevel) { // only parse one level bracket
          return resJsonObject;
        }
        stack.pop_back();
        jsonObject = stack.back();
        expectedType = Token::Type::COMMA | Token::Type::END_OBJ;
        preType = type;
        break;
      case Token::Type::COLON:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        } 
        expectedType = (Token::Type::NUMBER 
            | Token::Type::STRING
            | Token::Type::BEGIN_OBJ
            | Token::Type::BOOLEAN
            | Token::Type::NULL_OBJ);
        preType = type;
        break;
      case Token::Type::COMMA:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        } 
        expectedType = (Token::Type::STRING);
        preType = type;
        break;
      case Token::Type::NUMBER:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        } 
        jsonObject->addValue(key, stringToInt(token.value)); 
        expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        preType = type;
        break;
      case Token::Type::STRING:
        if(!isTypeExpected(expectedType, type)) {
          return resJsonObject;
        } 
        if(preType == Token::Type::COLON) { // string as value
          jsonObject->addValue(key, token.value);
          expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        } else{ // string as key
          key = token.value;
          expectedType = Token::Type::COLON; 
        }
        preType = type;
        break;
      case Token::Type::BOOLEAN:
        {

          if(!isTypeExpected(expectedType, type)) {
            return resJsonObject;
          } 
          jsonObject->addValue(key, (token.value == "true") ? true : false);
          expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        }
        break;
      case Token::Type::NULL_OBJ:
        {
          if(!isTypeExpected(expectedType, type)) {
            return resJsonObject;
          } 
          jsonObject->addValue(key, new JsonNull);
          expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        }
        break;
    } // switch
    tokens.erase(tokens.begin());
  } 
  return resJsonObject;
}

JsonObject* parseObject(const char* json) {
  auto tokens = tokenize(json);
#ifdef PRINT_DEBUG
  for(auto& item : tokens) {
    std::cout << "tokenize: " << item.type << " " << item.value << std::endl;
  }
#endif 
  return parseObject(tokens);
}

JsonArray* parseArray(std::vector<Token> &tokens) {
  JsonArray *jsonArray = nullptr;
  std::vector<JsonArray*> stack;
  int level = 0;
  int expectedType = Token::Type::BEGIN_ARR;
  while(!tokens.empty()) {
    auto token = tokens.at(0);
    int type = token.type;
    switch(type) {
      case Token::Type::END_ALL:
        break;
      case Token::Type::COMMA:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          expectedType = (Token::Type::NUMBER
              | Token::Type::STRING 
              | Token::Type::BOOLEAN
              | Token::Type::NULL_OBJ
              | Token::Type::BEGIN_OBJ 
              | Token::Type::BEGIN_ARR );
        }
        break;
      case Token::Type::BEGIN_ARR:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          level++;
          jsonArray = new JsonArray;
          if(level > 1 && !stack.empty()) {
            stack.back()->addItem(jsonArray);
          }else{
            stack.push_back(jsonArray);
          }
          stack.push_back(jsonArray);
          expectedType = (Token::Type::NUMBER | Token::Type::STRING 
              | Token::Type::BEGIN_OBJ | Token::Type::BEGIN_ARR 
              | Token::Type::END_ARR);
        }
        break;
      case Token::Type::NUMBER:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          jsonArray->addItem(stringToInt(token.value)); 
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;
      case Token::Type::STRING:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          jsonArray->addItem(token.value);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;

      case Token::Type::BEGIN_OBJ:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          jsonArray->addItem(parseObject(tokens, true));
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;

      case Token::Type::END_ARR:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          if(level == 1) {
            break;
          }
          level--;
          stack.pop_back();
          jsonArray = stack.back();
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;
      case Token::Type::BOOLEAN:
        {

          if(!isTypeExpected(expectedType, type)) {
            // error 
            return nullptr;
          } 
          jsonArray->addItem((token.value == "true") ? true : false);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;
      case Token::Type::NULL_OBJ:
        {
          if(!isTypeExpected(expectedType, type)) {
            // error 
            return nullptr;
          } 
          jsonArray->addItem(new JsonNull);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;

    } // switch
    tokens.erase(tokens.begin());
  }
  return stack.empty() ? nullptr : stack.back();
}

JsonArray* parseArray(const char* json) {
  auto tokens = tokenize(json);
#ifdef PRINT_DEBUG
  for(auto& item : tokens) {
    std::cout << "tokenize: " << item.type << " " << item.value << std::endl;
  }
#endif
  return parseArray(tokens);
}

std::string toJson(JsonValue* jsonValue) {
  std::string json = "";
  if(jsonValue == nullptr ) {
   return json;
  }
  std::string type = jsonValue->type;
  if("int" == type) 
  {
    int tmp = jsonValue->valueInt;
    std::vector<char> stack;
    while(tmp != 0) {
      stack.push_back((char)('0' + (tmp % 10)));
      tmp /= 10;
    }
    while(!stack.empty()) {
      json += stack.back();
      stack.pop_back();
    }
  }
  else if( "boolean" == type)
  {
    json = (jsonValue->valueBoolean ? "true" : "false");
  }
  else if( "string" == type)
  {
    json = jsonValue->valueString;
  }
  else if( "object" == type)
  {
    json = toJson(jsonValue->valueObject);
  }
  else if( "array" == type) 
  { 
    json = toJson(jsonValue->valueArray);
  }
  else if( "null" == type)
  {
    json = "null";    
  }
  return json;
}

std::string toJson(JsonObject* jsonObject) {
  std::string json = ""; 
  if(jsonObject == nullptr ) {
   return json;
  }
  json += "{";  
  auto valueMap = jsonObject->jsonValueMap;
  for(auto ite = valueMap.begin(); ite!=valueMap.end(); ite++){
    json += "\"";
    json += ite->first;
    json += "\":";
    json += toJson(ite->second);
    json += ",";
  }
  json.erase(json.length() - 1);
  json += "}";
  return json;
}


std::string toJson(JsonArray* jsonArray) {
  std::string json = "";
  if(jsonArray == nullptr ) {
   return json;
  }
  json += "[";
  for(const auto& item : jsonArray->jsonValueList) {
    json += toJson(item);
    json += ",";
  }
  json.erase(json.length() - 1);
  json += "]";
  return json;
}
