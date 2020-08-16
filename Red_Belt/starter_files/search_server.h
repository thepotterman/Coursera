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
  void Add(const string& document);
  const vector<pair<size_t, size_t>> Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  map<string, vector<pair<size_t,size_t>>> index;
  vector<string> docs;
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

