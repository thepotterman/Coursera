#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

//    map<size_t, size_t> docid_count;
    vector<int64_t> ids(50000, 0);
    vector<int64_t> ids_to_count(50000, 0);
    iota(ids.begin(), ids.end(), 0);
    for (const auto& word : words) {
      for (const int64_t id : index.Lookup(word)) {
        ids_to_count[id]++;
      }
    }

//    vector<pair<size_t, size_t>> search_results(
//      docid_count.begin(), docid_count.end()
//    );
/*    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );
*/
    partial_sort(ids.begin(), Head(ids, 5).end(), ids.end(), 
            [&ids_to_count](int64_t lhs, int64_t rhs) {
                return make_pair(ids_to_count[lhs], -lhs) > make_pair(ids_to_count[rhs], -rhs); 
            });
    search_results_output << current_query << ':';
//    for (auto [docid, hitcount] : Head(search_results, 5)) {
    for (auto id : Head(ids, 5)) {
      if(ids_to_count[id] == 0) {
          break;
      }
      search_results_output << " {"
        << "docid: " << id << ", "
        << "hitcount: " << ids_to_count[id] << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

vector<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
