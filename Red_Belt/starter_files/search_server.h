#pragma once

#include "synchronized.h"

#include <istream>
#include <ostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <future>
using namespace std;

class InvertedIndex {
public:
  void Add(const string& document);
  const vector<pair<size_t, size_t>> & Lookup(const string& word) const;
  size_t & GetSize() {
    return docs_size;
  }

private:
  map<string, vector<pair<size_t,size_t>>> index;
  size_t docs_size = 0;
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
