#include <iostream>
#include <vector>
#include <string>

struct Token {
  enum Type {
    BEGIN_OBJ = 1,
    END_OBJ = 2,
    COMMA = 4,
    COLON = 8,
    NUMBER = 16,
    STRING = 32,
    END_ALL = 64
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

class JsonObject;
class JsonArray;
struct JsonValue {

  JsonValue() {}
  JsonValue(int v) {}
  JsonValue(std::string& v) {}
  JsonValue(JsonObject* v) {}
  JsonValue(JsonArray* v) {}

};

struct JsonObject {

  void addValue(const std::string& key, int v) {
    std::cout << "add " << key << " int->" << v << std::endl;
  }

  void addValue(const std::string& key, std::string v) {

    std::cout << "add " << key << " string->" << v << std::endl;
  }

  void addValue(const std::string& key, JsonObject* v) {

    std::cout << "add " << key << " object->"  << std::endl;
  }

  void addValue(const std::string& key, JsonArray* v) {

    std::cout << "add " << key << " array->" << std::endl;
  }
};


struct JsonArray {

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

JsonObject* parseObject(std::vector<Token> &tokens) {
  JsonObject* jsonObject = new JsonObject;
  std::string key = "";
  int preType = -1;
  int expectedType = Token::Type::BEGIN_OBJ; 
  for(auto& token : tokens) {
    auto type = token.type;
    switch(type) {
      case Token::Type::END_ALL:
        return jsonObject;
      case Token::Type::BEGIN_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = (Token::Type::STRING);
        preType = type;
        break;
      case Token::Type::END_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = Token::Type::COMMA;
        preType = type;
        break;
      case Token::Type::COLON:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = (Token::Type::NUMBER | Token::Type::STRING | Token::Type::BEGIN_OBJ);
        preType = type;
        break;
      case Token::Type::COMMA:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = (Token::Type::STRING);
        preType = type;
        break;
      case Token::Type::NUMBER:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        jsonObject->addValue(key, stringToInt(token.value)); 
        expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        preType = type;
        break;
      case Token::Type::STRING:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
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
    } // switch
  } 
  return jsonObject;
}

JsonArray* parseArray(std::vector<Token> &tokens) {
  return nullptr;
}
