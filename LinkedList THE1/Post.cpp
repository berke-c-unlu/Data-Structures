#include "Post.h"

Post::Post() {

}

Post::Post(const std::string &message) {
    static int postId = 1;

    this->message = message;
    this->postId = postId;

    postId += 1;
}

const std::string &Post::getMessage() const {
    return this->message;
}

int Post::getPostId() const {
    return this->postId;
}

void Post::setMessage(const std::string &message) {
    this->message = message;
}

void Post::setPostId(int postId) {
    this->postId = postId;
}

std::ostream &operator<<(std::ostream &os, const Post &post) {
    os << "message: " << post.message;
    os << " postId: " << post.postId;
    return os;
}

bool Post::operator==(const Post &rhs) const {
    bool msg;
    bool postid;
    msg= this->message==rhs.message;
    postid= this->postId==rhs.postId;
    return (msg && postid);
}

bool Post::operator!=(const Post &rhs) const {
    return !(rhs == *this);
}

bool Post::operator<(const Post &rhs) const {
    bool msg;
    bool postid;
    msg= this->message<rhs.message;
    postid= this->postId<rhs.postId;
    return (msg && postid);
}

bool Post::operator>(const Post &rhs) const {
    return rhs < *this;
}

bool Post::operator<=(const Post &rhs) const {
    return !(rhs < *this);
}

bool Post::operator>=(const Post &rhs) const {
    return !(*this < rhs);
}
