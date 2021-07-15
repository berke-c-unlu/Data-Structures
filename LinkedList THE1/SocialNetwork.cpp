#include "SocialNetwork.h"

#include <iostream>
using std::cout;
using std::endl;
SocialNetwork::SocialNetwork() {

}

void SocialNetwork::addProfile(const std::string &firstname, const std::string &lastname, const std::string &email) {
    Profile nw=Profile(firstname,lastname,email);
    profiles.insertSorted(nw);
}

void SocialNetwork::addPost(const std::string &message, const std::string &email) {
    Post new_post=Post(message);
    Node<Profile> *x=profiles.getFirstNode();
    posts.insertAtTheTail(new_post);
    while(x->data.getEmail()!=email){
        x=x->next;
    }
    x->data.addPost(&(posts.getLastNode()->data));

}

void SocialNetwork::deleteProfile(const std::string &email) {
    Node<Profile> *x=profiles.getFirstNode();
    Node<Profile> *others=profiles.getFirstNode();
    while(x->data.getEmail()!=email) {
        x = x->next;
    }
    Node<Profile *> * op= others->data.getFriends().getFirstNode();
    while(others) {
        if(others->data==x->data){
            others=others->next;
            continue;
        }
        op=others->data.getFriends().getFirstNode();
        while (op){
            if(op->data->getEmail()==x->data.getEmail()){
                others->data.dropFriend(&(x->data));
                break;
            }
            op=op->next;
        }
        others=others->next;
    }
    others=profiles.getFirstNode();
    Node<Post*> *xp=x->data.getPosts().getFirstNode();
    Node<Post *> *like_others=others->data.getLikes().getFirstNode();
    while (others){
        if(others->data==x->data){
            others=others->next;
            continue;
        }
        xp=x->data.getPosts().getFirstNode();
        while (xp){
            like_others=others->data.getLikes().getFirstNode();
            while(like_others){
                if(like_others->data==xp->data){
                    others->data.dropLike(xp->data);
                    break;
                }
                like_others=like_others->next;
            }
            xp=xp->next;
        }
        others=others->next;
    }
    Node<Post> *all_posts=posts.getFirstNode();
    Node<Post *> *user_posts=x->data.getPosts().getFirstNode();
    Node<Post> *nextPointer;
    while(all_posts){
        user_posts=x->data.getPosts().getFirstNode();
        while(user_posts->data->getPostId()!=all_posts->data.getPostId()){
            user_posts=user_posts->next;
        }
        if(user_posts->data!= NULL){
            posts.removeNode(all_posts->data);
            break;
        }
        all_posts=all_posts->next;
    }
    LinkedList<Post*> post_del=x->data.getPosts();
    LinkedList<Post*> like_del=x->data.getLikes();
    LinkedList<Profile*> prof_del=x->data.getFriends();
    post_del.removeAllNodes();
    like_del.removeAllNodes();
    prof_del.removeAllNodes();
    profiles.removeNode(x);
}

void SocialNetwork::makeFriends(const std::string &email1, const std::string &email2) {
    Node<Profile> *ptr1=profiles.getFirstNode();
    Node<Profile> *ptr2=profiles.getFirstNode();

    while(ptr1->data.getEmail()!=email1) {
        ptr1 = ptr1->next;
    }
    while(ptr2->data.getEmail()!=email2) {
        ptr2 = ptr2->next;
    }
    ptr1->data.addFriend(&(profiles.getNode(ptr2->data)->data));
    ptr2->data.addFriend(&(profiles.getNode(ptr1->data)->data));
}

void SocialNetwork::likePost(int postId, const std::string &email) {
    Node<Post> *tmp=posts.getFirstNode();
    Node<Profile> *pr_ptr=profiles.getFirstNode();
    if(tmp){
        while(tmp->data.getPostId()!=postId){
            tmp=tmp->next;
        }
        while(pr_ptr->data.getEmail()!=email){
            pr_ptr=pr_ptr->next;
        }
        pr_ptr->data.addLike(&(posts.getNode(tmp->data)->data));
    }
}

void SocialNetwork::unlikePost(int postId, const std::string &email) {
    Node<Post> *tmp=posts.getFirstNode();
    Node<Profile> *pr_ptr=profiles.getFirstNode();

    while(tmp->data.getPostId()!=postId){
        tmp=tmp->next;
    }
    while(pr_ptr->data.getEmail()!=email){
        pr_ptr=pr_ptr->next;
    }
    if(tmp){
        pr_ptr->data.dropLike(&(posts.getNode(tmp->data)->data));
    }
}

LinkedList<Profile *> SocialNetwork::getMutualFriends(const std::string &email1, const std::string &email2) {
    Node<Profile> *ptr1=profiles.getFirstNode();
    Node<Profile> *ptr2=profiles.getFirstNode();
    while(ptr1->data.getEmail()!=email1) {
        ptr1 = ptr1->next;
    }
    while(ptr2->data.getEmail()!=email2) {
        ptr2 = ptr2->next;
    }
    Node<Profile*> *p1=ptr1->data.getFriends().getFirstNode();
    Node<Profile*> *p2=ptr2->data.getFriends().getFirstNode();
    LinkedList<Profile*> ret;
    if(p1!= nullptr && p2!=nullptr){
        while(p1){
            p2=ptr2->data.getFriends().getFirstNode();
            while (p2){
                if (p1->data==p2->data){
                    ret.insertAtTheTail(p1->data);
                }
                p2=p2->next;
            }
            p1=p1->next;
        }
        return ret;
    }
}

LinkedList<Post *> SocialNetwork::getListOfMostRecentPosts(const std::string &email, int k) {
    Node<Profile> *pr=profiles.getFirstNode();
    while(pr->data.getEmail()!=email){
        pr=pr->next;
    }
    Node<Post*> *tmp= pr->data.getPosts().getLastNode();
    LinkedList<Post *> ret;
    if(tmp!=nullptr){
        while(k){
            ret.insertAtTheTail(tmp->data);
            tmp=tmp->prev;
            k--;
        }
        return ret;
    }
}

void SocialNetwork::print() const {
    std::cout << "-------------------------------" << std::endl;

    std::cout << "# Number of profiles is " << profiles.getNumberOfNodes() << ":" << std::endl;
    profiles.print();

    std::cout << "# Number of posts is " << posts.getNumberOfNodes() << ":" << std::endl;
    posts.print();

    std::cout << "--------------------------------" << std::endl;
}
