#include "Json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadInt(istream& input) {
      int answer = 0;
      bool is_positive = true;
      if(input.peek() == '-') {
          is_positive = false;
          input.ignore(1);
      }
      while(isdigit(input.peek())) {
          answer *= 10;
          answer += input.get() - '0';
      }
      if(input.peek() == '.') {
          input.ignore(1);
          long double double_answer = answer;
          uint64_t power = 1;
          while(isdigit(input.peek())) {
              double_answer += ((input.get() - '0') / pow(10, power));
              power++;
          }
          if(is_positive) {
              return Node(double_answer);
          } else {
              return Node(-double_answer);
          }
      }
      if(is_positive) {
          return Node(answer);
      } else {
          return Node(-answer);
      }
  }
/*
    Node LoadInt(istream& input) {
      int answer = 0;
      input >> answer;
      if(input.peek() == '.') {
        input.ignore(1);
        uint64_t fractional_part = 0;
        input >> fractional_part;
        long double double_fractional_part = 0.0;
        while(fractional_part > 0) {
            int number = fractional_part % 10;
            fractional_part -= number;
            fractional_part /= 10;
            double_fractional_part /= 10;
            double_fractional_part += number / 10.0;
        }
        long double double_answer = static_cast<long double>(answer) + double_fractional_part;
        return Node(double_answer);
      }
      return Node(answer);
    }
*/

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadBool(istream& input) {
    string line;
    bool answer;
    getline(input, line, 'e');
    if(line == "tru") {
        answer = true;
        return Node(answer);
    } else {
        answer = false;
        return Node(answer);
    }
  }

    Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (c == 't' || c == 'f') {
        input.putback(c);
        return LoadBool(input);
    } else {
        input.putback(c);
        return LoadInt(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}

