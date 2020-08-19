#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {
      make_move_iterator(istream_iterator<string>(words_input)), 
      make_move_iterator(istream_iterator<string>())};
}

void UpdateDocumentBaseSingleThread(istream& document_input, Synchronized<InvertedIndex> & index_) {
  InvertedIndex new_index;
  
  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  //index_.GetAccess().ref_to_value.swap(new_index);
  swap(index_.GetAccess().ref_to_value, new_index);
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBaseSingleThread(document_input, index);
}

void SearchServer::UpdateDocumentBase(istream & document_input) {
//    UpdateDocumentBaseSingleThread(document_input, index);
    futures.push_back(async(UpdateDocumentBaseSingleThread, ref(document_input), ref(index)));
}

void AddQueriesStreamSingleThread(
  istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex> & index_
) {
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    vector<int64_t> ids;
    vector<int64_t> ids_to_count;
    {
        auto access = index_.GetAccess();
        auto size = access.ref_to_value.GetSize();
        ids.resize(size);
        ids_to_count.assign(size, 0);
        iota(ids.begin(), ids.end(), 0);
    }
    for (const auto& word : words) {
      auto access = index_.GetAccess();
      for (auto & [id, count] : access.ref_to_value.Lookup(word)) {   
        ids_to_count[id]+= count;
      }
    }

    partial_sort(ids.begin(), Head(ids, 5).end(), ids.end(), 
            [&ids_to_count](int64_t lhs, int64_t rhs) {
                return pair(ids_to_count[lhs], -lhs) > pair(ids_to_count[rhs], -rhs); 
            });
    search_results_output << current_query << ':';
    for (auto id : Head(ids, 5)) {
      if(ids_to_count[id] == 0) {
          break;
      }
      search_results_output << " {"
        << "docid: " << id << ", "
        << "hitcount: " << ids_to_count[id] << '}';
    }
    search_results_output << "\n";
  }
}

void SearchServer::AddQueriesStream(
    istream & query_input, ostream & search_results_output
) {
//    AddQueriesStreamSingleThread(query_input, search_results_output, index);
    futures.push_back(
                async(AddQueriesStreamSingleThread, 
                ref(query_input), 
                ref(search_results_output), 
                ref(index)));
}

void InvertedIndex::Add(const string& document) {
//  docs.push_back(document);
  ++docs_size;

  const size_t id = docs_size - 1;
  for (const auto& word : SplitIntoWords(move(document))) {
      auto & id_to_count = index[word];
      if(!id_to_count.empty() && id_to_count.back().first == id) {
        ++id_to_count.back().second;
      } else {
        id_to_count.push_back({id, 1});
      }
  }
}

const vector<pair<size_t, size_t>> & InvertedIndex::Lookup(const string& word) const {
    static const vector<pair<size_t, size_t>> answer;
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return answer;
  }
}
