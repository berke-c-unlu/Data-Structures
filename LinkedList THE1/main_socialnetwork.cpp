#include <iostream>

#include "SocialNetwork.h"

int main() {
    SocialNetwork snetwork;

    snetwork.print();

    snetwork.addProfile("berke", "unlu", "berke@com");
    snetwork.addProfile("berkins", "pinkguy", "joji@com");
    snetwork.addProfile("hidir", "green", "hidir@com");
    snetwork.addProfile("bora", "sargi", "bora@com");
    snetwork.addProfile("kose", "soke", "kose@com");
    snetwork.addProfile("skyheart","ironalp","gokcan@com");
    snetwork.addProfile("yigit","varloCK","varlock@google.com");
    snetwork.addPost("berkeeeeeeeeee.", "berke@com");
    snetwork.addPost("berkeeeeeeeeee.", "berke@com");
    snetwork.addPost("asdasd","kose@com");
    snetwork.addPost("qwerqwerqw","gokcan@com");
    snetwork.likePost(1,"joji@com");
    snetwork.likePost(2,"hidir@com");
    snetwork.likePost(3,"berke@com");
    snetwork.makeFriends("berke@com","joji@com");
    snetwork.makeFriends("kose@com","berke@com");
    snetwork.makeFriends("berke@com","bora@com");
    snetwork.makeFriends("kose@com","bora@com");
    snetwork.print();
    snetwork.deleteProfile("berke@com");
    snetwork.print();
    snetwork.deleteProfile("kose@com");
    snetwork.print();

    return 0;
}
