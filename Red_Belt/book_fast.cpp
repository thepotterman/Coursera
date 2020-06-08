#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <sstream>
#include <string>

using namespace std;

void PrintMap(const map<int, int> & _map);

class ReadingManager {
public:

  void Read(int user_id, int page_count) {
    if (users_to_progress.count(user_id) == 0) {
      users_to_progress[user_id] = page_count;
      progress_to_users[page_count]++;
      return;
    }
    int current = users_to_progress[user_id]; 
    progress_to_users[page_count]++;
    progress_to_users[current]--;
    if(progress_to_users[current] == 0) {
    	progress_to_users.erase(current);
    }
    users_to_progress[user_id] = page_count;
  }

  double Cheer(int user_id) {
    if(users_to_progress.count(user_id)== 0) {
      return 0;
    }
    if(users_to_progress.size() == 1) {
      return 1;
    }
    auto it = progress_to_users.begin();
    int less = 0;
    int page_count = users_to_progress[user_id];
    while(it->first != page_count) {
    	less += it->second;
    	it++;
    }
    // cout << "\nusers to progress: ";
    // PrintMap(users_to_progress);
    // cout << "\nprogress to users: ";
    // PrintMap(progress_to_users);
    // cout << "\n";

    return (less * 1.0) / (users_to_progress.size() - 1);
  }

private:

  map <int, int> users_to_progress;
  map <int, int> progress_to_users;

};

void PrintMap(const map<int, int> & _map)
{
    stringstream ss;
    ss << "{";
    for(const auto el : _map) {
        ss << el.first << ": " << el.second << ", ";
    }
    string s = ss.str();
//    s.pop_back();
 //   s.pop_back();
    s.push_back('}');
    cout << s;
}

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}



