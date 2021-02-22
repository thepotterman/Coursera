#include "Common.h"
#include <deque>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
            shared_ptr<IBooksUnpacker> books_unpacker,
            const Settings& settings
    ) {
        books_unpacker_ = books_unpacker;
        max_memory_ = settings.max_memory;
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> lock(m);
        bool success = false;
        for(const auto & book : storage_) {
            if(book.get()->GetName() == book_name) {
                success = true;
                return book;
            }
        }
        if(!success) {
            BookPtr book = move(books_unpacker_.get()->UnpackBook(book_name));
            size_t book_size = book.get()->GetContent().size();
            if(book_size > max_memory_) {
                storage_.clear();
                current_memory_ = 0;
                return book;
            } else {
                while(max_memory_ - current_memory_ < book_size) {
                    auto book_to_delete = storage_.front();
                    current_memory_ -= book_to_delete.get()->GetContent().size();
                    storage_.pop_front();
                }
                storage_.push_back(book);
                auto book_to_return = storage_.back();
                current_memory_ += book_to_return.get()->GetContent().size();
                return book_to_return;
            }
        }
    }
private:
    size_t max_memory_;
    size_t current_memory_ = 0;
    deque<BookPtr> storage_;
    shared_ptr<IBooksUnpacker> books_unpacker_;
    mutex m;
};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings& settings
) {
    return make_unique<LruCache>(books_unpacker, settings);
}
