// NewsPaperSubscriber.h
//Definition of NewsPaperSubscriber class

#ifndef NewsPaperSubscriber_H
#define NewsPaperSubscriber_H

#include <string> 
#include <sstream>
#include "Address.h"

class NewsPaperSubscriber{
    // string st_Address = Address::getstreetAddress();
    public:
    Address st_Address;
    string rate = "";

    // constructors
    public:
    NewsPaperSubscriber(){
        st_Address = st_Address;
        rate = "";
    }
    public:
    NewsPaperSubscriber(Address street_address){
        setAddress(street_address);
    }

    // must be overridden
    virtual ~NewsPaperSubscriber() = 0;
    virtual void setRate() = 0;
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

    // FIXME: add condition
    bool equals(NewsPaperSubscriber *newsub){
        string stringsub = "";
        stringsub = newsub -> getAddress().getstreetAddress();
        bool status = true;

        return status;
    }

};
NewsPaperSubscriber::~NewsPaperSubscriber(){}

#endif
