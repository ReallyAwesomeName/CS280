// WeekdaysSubscriber.h
//Definition of WeekdaysSubscriber class

#ifndef WeekdaysSubscriber_H
#define WeekdaysSubscriber_H

#include <string> 
#include <sstream>
#include "NewsPaperSubscriber.h"

class WeekdaysSubscriber:public NewsPaperSubscriber{
    string Weekdays_Rate = "$7.5";

    public:
    WeekdaysSubscriber(Address address){
        st_Address = address;
        setRate(Weekdays_Rate);
    }
    // ~WeekdaysSubscriber(){}

    void setRate() override;
    public:
    void setRate(string Weekdays_Rate){
        // NewsPaperSubscriber::setRate();
        rate = Weekdays_Rate;
        return;
    }
    void print() override;
    public:
    void print(Address sub){
        // NewsPaperSubscriber::print();
        Address thisaddress = getAddress();
        std::cout << thisaddress.getstreetAddress() << std::endl;
        std::cout << thisaddress.getCity() << ", " << thisaddress.getState() << thisaddress.getzipCode() << std::endl;
        std::cout << getRate() << std::endl;
        std::cout << "Service Type: Week Days Subscriber" << std::endl;
    }
};
#endif
