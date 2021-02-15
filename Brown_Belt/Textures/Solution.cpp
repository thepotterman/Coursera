#include "Common.h"
#include<memory>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Ellipse : public IShape {
public:
    std::unique_ptr<IShape> Clone() const {
        auto answer = make_unique<Ellipse>();
        answer.get()->position_ = position_;
        answer.get()->size_ = size_;
        answer.get()->texture_ = texture_;
        return answer;
    }

    void SetPosition(Point position) {
        position_ = position;
    }

    Point GetPosition() const {
        return position_;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    Size GetSize() const {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) {
        texture_ = texture;
    }

    ITexture* GetTexture() const {
        return texture_.get();
    }

    void Draw(Image& image) const {
        Size texture_size = {0, 0};
        Image texture;
        if(texture_) {
            texture_size = texture_.get()->GetSize();
            texture = texture_.get()->GetImage();
        }
        for(int i = position_.y; i < image.size(); ++i) {
            if(i >= position_.y + size_.height) {
                break;
            }
            for(int j = position_.x; j < image[0].size(); ++j) {
                if(j >= position_.x + size_.width) {
                    break;
                }
                if(IsPointInEllipse({j - position_.x, i - position_.y}, size_)) {
                    if(i - position_.y < texture_size.height &&
                       j - position_.x < texture_size.width) {
                        image[i][j] = texture[i - position_.y][j - position_.x];
                    } else {
                        image[i][j] = '.';
                    }
                }
            }
        }
    }
private:
    Point position_;
    Size size_;
    shared_ptr<ITexture> texture_;
};

class Rectangle : public IShape {
public:
    std::unique_ptr<IShape> Clone() const {
        auto answer = make_unique<Rectangle>();
        answer.get()->position_ = position_;
        answer.get()->size_ = size_;
        answer.get()->texture_ = texture_;
        return answer;
    }

    void SetPosition(Point position) {
        position_ = position;
    }

    Point GetPosition() const {
        return position_;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    Size GetSize() const {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) {
        texture_ = texture;
    }

    ITexture* GetTexture() const {
        return texture_.get();
    }

    void Draw(Image& image) const {
        Size texture_size = {0, 0};
        Image texture;
        if(texture_) {
            texture_size = texture_.get()->GetSize();
            texture = texture_.get()->GetImage();
        }
        for(int i = position_.y; i < image.size(); ++i) {
            if(i >= position_.y + size_.height) {
                break;
            }
            for(int j = position_.x; j < image[0].size(); ++j) {
                if(j >= position_.x + size_.width) {
                    break;
                }
                if(i - position_.y < texture_size.height &&
                   j - position_.x < texture_size.width) {
                    image[i][j] = texture[i - position_.y][j - position_.x];
                } else {
                    image[i][j] = '.';
                }
            }
        }
    }

private:
    Point position_;
    Size size_;
    shared_ptr<ITexture> texture_;
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if(shape_type == ShapeType::Rectangle) {
        return make_unique<Rectangle>();
    } else {
        return make_unique<Ellipse>();
    }
}
