#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <iostream>

using namespace std;

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {make_move_iterator(istream_iterator<string>(words_input)), make_move_iterator(istream_iterator<string>())};
}

InvertedIndex::InvertedIndex(istream & document_input) {
    index.reserve(15000);
    string line;
    while(getline(document_input, line)) {
        ++docs_count;
        size_t id = docs_count - 1;
        for(const string & word : SplitIntoWords(line)) {
            auto & id_to_count = index[word];
            if(!id_to_count.empty() && id_to_count.back().first == id) {
                ++id_to_count.back().second;
            } else {
                id_to_count.emplace_back(id, 1);
            }
        }
    }
}

void UpdateDocumentBaseSingleThread(istream& document_input, Synchronized<InvertedIndex> & _index) {
  InvertedIndex new_index(document_input);

  swap(_index.GetAccess().ref_to_value, new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
//    UpdateDocumentBaseSingleThread(document_input, index);
    futures.push_back(async(UpdateDocumentBaseSingleThread, ref(document_input), ref(index)));
}

void AddQueriesStreamSingleThread(
  istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex> & _index
) {
    vector<int64_t> ids;
    vector<int64_t> ids_to_count;
    string line;
    while(getline(query_input, line)) {
        const auto words = SplitIntoWords(line);
        auto access = _index.GetAccess();
        const size_t docs_count = access.ref_to_value.GetDocsCount();
        ids_to_count.assign(docs_count, 0);
        ids.resize(docs_count);

        auto index = access.ref_to_value;

        for(const auto & word : words) {
            for(const auto & [doc_id, hit_count] : index.Lookup(word)) {
                ids_to_count[doc_id] += hit_count;
            }
        }
        iota(ids.begin(), ids.end(), 0);

        partial_sort(ids.begin(), Head(ids, 5).end(), ids.end(),
                [&ids_to_count](int64_t lhs, int64_t rhs){
                    return pair(ids_to_count(lhs), -lhs) > pair(ids_to_count(rhs), -rhs);
                });
        search_result_output << line << ";";
        
        for(size_t doc_id : Head(ids, 5)) {
            const size_t hit_count = ids_to_count[doc_id];
            if(hit_count == 0) {
                break;
            }

            search_result_output << " {docid: " << doc_id << ", hitcount: " << hit_count << "}";
        }
        search_result_output << "\n";
    }
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
//    AddQueriesStreamSingleThread(query_input, search_results_output, index);
    futures.push_back(async(AddQueriesStreamSingleThread, ref(query_input), ref(search_results_output), ref(index)));
}


const vector<pair<size_t, size_t>> InvertedIndex::Lookup(const string& word) const {
    static const vector<pair<size_t, size_t>> answer;
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return answer;
  }
}
