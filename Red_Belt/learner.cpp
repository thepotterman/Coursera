#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
  		size_t cur = dict.size();
  		for(const string & s : words)
  		{
  			dict.insert(s);	
  		}
  		return dict.size() - cur;
    }

  vector<string> KnownWords() {
  		return {dict.begin(), dict.end()};
    }
};
