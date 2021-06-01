#include <iostream>
#include <vector>
#include <string>

struct Token {
    enum Type {
      BEGIN_OBJ = 0,
      END_OBJ,
      COMMA,
      COLON,
      NUMBER,
      STRING,
      END_ALL
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

  void addValue(std::string& key, JsonValue* value) {}
};


struct JsonArray {

};

bool isTypeExpected(int expectedType, int type) {
  return expectedType | type;
}


JsonObject* parseObject(std::vector<Token> &tokens) {
  JsonObject* jsonObject = new JsonObject;
  std::string key = "";
  int preType = -1;
  Token::Type expectedType = Token::Type::BEGIN_OBJ; 
  for(auto& token : tokens) {
    auto type = token.type;
    switch(type) {
      case Token::Type::END_ALL:
        return jsonObject;
      case Token::Type::BEGIN_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = (Token::Type::COLON);
        break;
      case Token::Type::END_OBJ:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        expectedType = Token::Type::COMMA;
        break;
      case Token::Type::COLON:
        if(!isTypeExpected(expectedType, type)) {
          return jsonObject;
        } 
        // number or string
        break;
      case Token::Type::COMMA:
          if(!isTypeExpected(expectedType, type)) {
            return jsonObject;
          } 
          // expect {
          break;
      case Token::Type::NUMBER:
          if(!isTypeExpected(expectedType, type)) {
            return jsonObject;
          } 
          //           jsonObject->addValue(key, parseNumber());
          // expect , or }
          break;
      case Token::Type::STRING:
          if(!isTypeExpected(expectedType, type)) {
            return jsonObject;
          } 
          if(preType == Token::Type::COLON) { // string as value
            JsonValue *jv = new JsonValue(); // todo set v
            jsonObject->addValue(key, jv);
            // expect , or }
          } else{ // string as key
            //              key = v;
            // expect : 
          }
          break;
    }
  } 
  return nullptr;
}

JsonArray* parseArray(std::vector<Token> &tokens) {
  return nullptr;
}
