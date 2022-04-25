// WeekendSubscriber.h
//Definition of WeekendSubscriber class

#ifndef WeekendSubscriber_H
#define WeekendSubscriber_H

#include <string> 
#include <sstream>
#include "NewsPaperSubscriber.h"

class WeekendSubscriber:public NewsPaperSubscriber{
    string Weekend_Rate = "4.5";

    public:
    WeekendSubscriber(Address address){
        st_Address = address;
        setRate(Weekend_Rate);
    }
    // destructor
    ~WeekendSubscriber(){}

    public:
    void setRate(string Weekend_Rate) override{
        rate = Weekend_Rate;
        return;
    }
    public:
    void print() override{
        Address thisaddress = getAddress();
        std::cout << thisaddress.getstreetAddress() << std::endl;
        std::cout << thisaddress.getCity() << ", " << thisaddress.getState() 
            << " " << thisaddress.getzipCode() << std::endl;
        std::cout << "Rate: " << getRate() << std::endl;
        std::cout << "Service Type: Weekend Subscriber" << std::endl;
    }
};
#endif
