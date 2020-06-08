#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

using namespace std;

class RouteManager {
public:
  void AddRoute(int & start, int & finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[finish].insert(start);
  }
  int FindNearestFinish(int & start, int & finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
        return result;
    }
    const set<int> & reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
    	auto low = reachable_stations.lower_bound(finish);
    	if (low == reachable_stations.end())
    	{
    		result = min(result, abs(finish - *(prev(low))));
        return result;
    	}
    	result = min(result, min(abs(finish - *(low)), abs(finish - *(prev(low)))));
    }
    return result;
  }
private:
  map<int, set<int>> reachable_lists_; 
};


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
  	RouteManager routes;

	  int query_count;
	  cin >> query_count;

	  for (int query_id = 0; query_id < query_count; ++query_id) {
	    string query_type;
	    cin >> query_type;
	    int start, finish;
	    cin >> start >> finish;
	    if (query_type == "ADD") {
	      routes.AddRoute(start, finish);
	    } else if (query_type == "GO") {
	      cout << routes.FindNearestFinish(start, finish) << "\n";
	    }
	  }

  return 0;
}
