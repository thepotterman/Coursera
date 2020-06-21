#include "stats.h"

void Stats::AddMethod(string_view method) {
    if( method != "GET" &&
        method != "POST" &&
        method != "PUT" &&
        method != "DELETE") {
            method = "UNKNOWN";
    }
        ++_methods[method];
    }
void Stats::AddUri(string_view uri) {
    if( uri != "/" &&
        uri != "/order" &&
        uri != "/product" &&
        uri != "/basket" &&
        uri != "/help") {
            uri = "unknown";
        }
        ++_uris[uri];
    }
const map<string_view, int>& Stats::GetMethodStats() const {
        return _methods;
    }
const map<string_view, int>& Stats::GetUriStats() const {
        return _uris;
    }

HttpRequest ParseRequest(string_view line) {
    HttpRequest answer;
    line.remove_prefix(line.find_first_not_of(" "));
    for(int i = 0; i < 3; ++i) {
        size_t space = line.find(' ');
        if(i == 0) {
            string_view method = line.substr(0, space);
            line.remove_prefix(space + 1);
            answer.method = method;
        } else if(i == 1) {
            string_view uri = line.substr(0, space);
            line.remove_prefix(space + 1);
            answer.uri = uri;
        } else {
            answer.protocol = line.substr(0, space);
            line.remove_prefix(space + 1);
        }
    }
    return answer;
}

