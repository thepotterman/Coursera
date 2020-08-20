#pragma once

#include "synchronized.h"

#include <istream>
#include <ostream>
#include <set>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <future>
using namespace std;

class InvertedIndex {
public:
  void Add(string document);
  const vector<pair<size_t, size_t>> & Lookup(const string_view word) const;
  void reserve() {
    docs.reserve(15000);
    index.reserve(15000);
  }
  size_t & GetSize() {
    return docs_size;
  }

private:
  unordered_map<string_view, vector<pair<size_t,size_t>>> index;
  size_t docs_size = 0;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> futures;
};
