// WeeklySubscriber.h
//Definition of WeeklySubscriber class

#ifndef WeeklySubscriber_H
#define WeeklySubscriber_H

#include <string> 
#include <sstream>
#include "NewsPaperSubscriber.h"

class WeeklySubscriber:public NewsPaperSubscriber{
    string Weekly_Rate = "10.5";

    public:
    WeeklySubscriber(Address address){
        st_Address = address;
        setRate(Weekly_Rate);
    }
    // destructor
    ~WeeklySubscriber(){}

    public: 
    void setRate(string Weekly_Rate) override{
        rate = Weekly_Rate;
        return;
    }
    public:
    void print() override{
        Address thisaddress = getAddress();
        std::cout << thisaddress.getstreetAddress() << std::endl;
        std::cout << thisaddress.getCity() << ", " << thisaddress.getState() 
            << " " << thisaddress.getzipCode() << std::endl;
        std::cout << "Rate: " << getRate() << std::endl;
        std::cout << "Service Type: Weekly Subscriber" << std::endl;
    }
};
#endif
