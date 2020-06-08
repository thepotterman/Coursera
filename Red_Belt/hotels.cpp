#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <utility>

#include "test_runner.h"

using namespace std;

struct booking {
	string _hotel_name;
	uint64_t _user_id;
	int _room_count;
};

bool operator < (const booking & lhs, const booking & rhs)
{
	return tie(lhs._hotel_name, lhs._user_id, lhs._room_count) <
		   tie(rhs._hotel_name, rhs._user_id, rhs._room_count);
}

class BookingHotel {
public:
	void Book() {
		int64_t time;
		string hotel_name;
		uint64_t user_id;
		int room_count;
		cin >> time >> hotel_name >> user_id >> room_count;
        _times.push({time, {hotel_name, user_id, room_count}});
        this->ClearTimes(time);
        _hotels_to_rooms[hotel_name] += room_count;
        _hotels_to_users_to_count[hotel_name][user_id]++; 
	}
    void ClearTimes(const int64_t & time) {
        while(time - _times.front().first > 86399) {
            auto current_booking = _times.front().second;
            string hotel = current_booking._hotel_name;
            uint64_t user = current_booking._user_id;
            int rooms = current_booking._room_count;
            if(_hotels_to_rooms[hotel] == 0) {
                _hotels_to_rooms.erase(hotel);  
            } else {
                _hotels_to_rooms[hotel] -= rooms;
                if(_hotels_to_rooms[hotel] == 0) {
                    _hotels_to_rooms.erase(hotel);  
                }
            }
            _hotels_to_users_to_count[hotel][user]--;
            if(_hotels_to_users_to_count[hotel][user] == 0) {
                _hotels_to_users_to_count[hotel].erase(user);
            }
            if(_hotels_to_users_to_count[hotel].empty()) {
                _hotels_to_users_to_count.erase(hotel);
            }
            _times.pop();
        }
    }
	void Clients() {
		string hotel_name;
		cin >> hotel_name;
		cout << _hotels_to_users_to_count[hotel_name].size() << '\n';
	}
	void Rooms() {
		string hotel_name;
		cin >> hotel_name;
		cout << _hotels_to_rooms[hotel_name] << '\n';
	}
private:
	queue<pair<int64_t, booking>> _times;
    map<string, int> _hotels_to_rooms;
    map<string, map<uint64_t, int>> _hotels_to_users_to_count;
};

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int q = 0;
	cin >> q;
	BookingHotel BH;
	for(int i = 0; i < q; ++i) {
		string request;
		cin >> request;
		if(request == "BOOK") {
			BH.Book();
		} else if(request == "ROOMS") {
			BH.Rooms();
		} else {
			BH.Clients();
		}
	}
	return 0;
}


