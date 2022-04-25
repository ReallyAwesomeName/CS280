// NewsPaperSubscriber.h
//Definition of NewsPaperSubscriber class

#ifndef NewsPaperSubscriber_H
#define NewsPaperSubscriber_H

#include <string> 
#include <sstream>
#include "Address.h"

class NewsPaperSubscriber{
    public:
    Address st_Address;
    string rate = "";

    // constructors
    public:
    NewsPaperSubscriber(){
        st_Address = Address();
        rate = "";
    }
    public:
    NewsPaperSubscriber(Address street_address){
        st_Address = street_address;
    }

    // must be overridden
    virtual ~NewsPaperSubscriber() = 0;
    virtual void setRate(string subrate) = 0;
    virtual void print() = 0;

    void setAddress(Address street_address){
        st_Address = street_address;
        return;
    }
    Address getAddress(){
        return st_Address;
    }

    string getRate(){
        return rate;
    }

    bool equals(NewsPaperSubscriber *newsub){
        bool status = false;
        if (st_Address == newsub->getAddress()){
            status = true;
        }
        return status;
    }

};
NewsPaperSubscriber::~NewsPaperSubscriber(){}

#endif
