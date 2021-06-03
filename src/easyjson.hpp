#include <iostream>
#include <vector>
#include <string>

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

struct JsonNull {

};

struct JsonObject;
struct JsonArray;
struct JsonValue {

  JsonValue() {

  }

  JsonValue(int v) {
    (void)(v);
  }

  JsonValue(const std::string& v) {
    (void)(v);
  }

  JsonValue(bool v) {
    (void)(v);
  }

  JsonValue(JsonNull* v) {
    (void)(v);
  }

  JsonValue(JsonObject* v) { 
    (void)(v);
  }

  JsonValue(JsonArray* v) {  
    (void)(v);
  }

};

struct JsonObject {

  void addValue(const std::string& key, int v) {
    std::cout << "add " << key << " int->" << v << std::endl;
  }

  void addValue(const std::string& key, std::string v) {

    std::cout << "add " << key << " string->" << v << std::endl;
  }

  void addValue(const std::string& key, JsonObject* v) {
    (void)(v);
    std::cout << "add " << key << " object->"  << std::endl;
  }

  void addValue(const std::string& key, JsonArray* v) {
    (void)(v);
    std::cout << "add " << key << " array->" << std::endl;
  }

  void addValue(const std::string& key, bool v) {
    (void)(v);
    std::cout << "add " << key << " bool-> " << v << std::endl;
  }

  void addValue(const std::string& key, JsonNull* v) {
    (void)(v);
    std::cout << "add " << key << " null " << std::endl;
  }
};


struct JsonArray {

  void addItem(int v) {
    std::cout << "addItem int->" << v << std::endl;
  }

  void addItem(const std::string& v) { 
    std::cout << "addItem string->" << v << std::endl;
  }

  void addItem(JsonObject* v) {  
    (void)(v);
    std::cout << "addItem object->" << std::endl;
  }

  void addItem(JsonArray* v) {
    (void)(v);
    std::cout << "addItem array->" << std::endl;
  };

  void addItem(JsonNull* v) {
    (void)(v);
    std::cout << "addItem null" << std::endl;
  }

  void addItem(bool v){
    (void)(v);
    std::cout << "addItem bool->" << v << std::endl;
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
  JsonObject* jsonObject = new JsonObject;
  std::string key = "";
  int preType = -1;
  int expectedType = Token::Type::BEGIN_OBJ; 
  while(!tokens.empty()) {
    auto token = tokens.at(0);
    const Token::Type type = token.type;
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
        if(parseOneLevel) { // only parse one level bracket
          return jsonObject;
        }
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
        expectedType = (Token::Type::NUMBER 
            | Token::Type::STRING
            | Token::Type::BEGIN_OBJ
            | Token::Type::BOOLEAN
            | Token::Type::NULL_OBJ);
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
      case Token::Type::BOOLEAN:
        {

          if(!isTypeExpected(expectedType, type)) {
            return jsonObject;
          } 
          jsonObject->addValue(key, (token.value == "true") ? true : false);
          expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        }
        break;
      case Token::Type::NULL_OBJ:
        {
          if(!isTypeExpected(expectedType, type)) {
            return jsonObject;
          } 
          jsonObject->addValue(key, new JsonNull);
          expectedType = (Token::Type::COMMA | Token::Type::END_OBJ);
        }
        break;
    } // switch
    tokens.erase(tokens.begin());
  } 
  return jsonObject;
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
  JsonArray* resArray = nullptr;
  JsonArray *jsonArray = nullptr;
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
          level++;
          if(!isTypeExpected(expectedType, type)) {
            return jsonArray;
          }
          jsonArray = new JsonArray;
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
          if(!resArray) {
            resArray = new JsonArray;
          }
          resArray->addItem(jsonArray);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;
      case Token::Type::BOOLEAN:
        {

          if(!isTypeExpected(expectedType, type)) {
            return resArray;
          } 
          jsonArray->addItem((token.value == "true") ? true : false);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;
      case Token::Type::NULL_OBJ:
        {
          if(!isTypeExpected(expectedType, type)) {
            return resArray;
          } 
          jsonArray->addItem(new JsonNull);
          expectedType = (Token::Type::COMMA | Token::Type::END_ARR);
        }
        break;

    } // switch
    tokens.erase(tokens.begin());
  }
  return resArray;
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
