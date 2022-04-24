// WeekendSubscriber.h
//Definition of WeekendSubscriber class

#ifndef WeekendSubscriber_H
#define WeekendSubscriber_H

#include <string> 
#include <sstream>
#include "NewsPaperSubscriber.h"

class WeekendSubscriber:public NewsPaperSubscriber{
    string Weekend_Rate = "$4.5";

    public:
    WeekendSubscriber(Address address){
        st_Address = address;
        setRate(Weekend_Rate);
    }
    // ~WeekendSubscriber(){}

    void setRate() override;
    public:
    void setRate(string Weekend_Rate){
        rate = Weekend_Rate;
        return;
    }
    void print() override;
    public:
    void print(Address sub){
        Address thisaddress = getAddress();
        std::cout << thisaddress.getstreetAddress() << std::endl;
        std::cout << thisaddress.getCity() << ", " << thisaddress.getState() << thisaddress.getzipCode() << std::endl;
        std::cout << getRate() << std::endl;
        std::cout << "Service Type: Weekend Subscriber" << std::endl;
    }
};
#endif
