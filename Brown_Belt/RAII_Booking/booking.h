#pragma once

#include <utility>

namespace RAII {
    template<typename T>
    class Booking {
    public:
        Booking(T * provider, int id)
            : provider_(provider),
              id_(id) { }

        Booking(const Booking &) = delete;

        Booking(Booking && other) {
            provider_ = std::move(other.GiveProvider());
            id_ = std::move(other.GiveId());
        }

        T * GiveProvider() {
            T * answer = provider_;
            provider_ = nullptr;
            return answer;
        }

        int GiveId() {
            int answer = id_;
            id_ = 0;
            return answer;
        }

        Booking & operator = (Booking && other) {
            if(provider_ != nullptr) {
                provider_->CancelOrComplete(*this);
            }
            provider_ = std::move(other.GiveProvider());
            id_ = std::move(other.GiveId());
            return *this;
        }

        ~Booking() {
            if(provider_ != nullptr) {
                provider_->CancelOrComplete(*this);
            }
        }
    private:
        T * provider_;
        int id_;
    };
}
