#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <cmath>

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int,
                            bool,
                            long double,
                            std::string> {
  public:
    using variant::variant;

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    int AsInt() const {
      return std::get<int>(*this);
    }
    int AsBool() const {
      return std::get<bool>(*this);
    }
    long double AsLongDouble() const {
        if(std::holds_alternative<long double>(*this)) {
            return std::get<long double>(*this);
        } else {
            return std::get<int>(*this);
        }
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);

}
