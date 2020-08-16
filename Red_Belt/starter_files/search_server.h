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
  InvertedIndex() = default;

  explicit InvertedIndex(istream & document_input);

  const vector<pair<size_t, size_t>> Lookup(const string& word) const;

  const string& GetDocsCount() const {
    return docs_count;
  }

private:
  size_t docs_count = 0;
  map<string, vector<pair<size_t,size_t>>> index;
};

class SearchServer {
public:
  SearchServer() = default;

  explicit SearchServer(istream& document_input)
      : index(InvertedIndex(document_input)) {}

  void UpdateDocumentBase(istream& document_input);

  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> futures;
};

