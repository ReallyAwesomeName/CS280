# CS 280: RA9 Newspaper Subscription

Spring 2022\
Recitation Assignment 9\
April 21, 2022\
Due Date: Monday, April 25, 2022, 23:59\
Total Points: 5

In this assignment you are given a class representing a street address called Address which is used
to  describe  the  location  of  newspaper  subscribers.  You  are  required  to  create  an  abstract  class
called NewsPaperSubscriber, and create three child classes that are derived from it. The three child
classes  represent  three  types  of  newspaper’s  subscribers:  weekly  subscribers  receiving  the
newspaper  daily,  weekdays  subscribers  receiving  the  newspaper  everyday  except  on  weekends,
and weekend subscribers receiving the newspaper on weekends only. The three child classes are
called  WeeklySubscriber,  WeekdaysSubscriber,  and  WeekendSubscriber,  respectively.  The  class
definitions are as follows:

1. NewsPaperSubscriber has two data members: a subscriber’s street address (st_Address) and
the subscription rate (rate).  Include two constructors, one initializes the class object without
passed  parameters,  and  another  that  receives  one  parameter  to  initialize  the  street  address.
Include getter and setter methods for the data members. The setter method, setRate(), for the
rate is abstract. Include an abstract method, called print(),  that does not return a value and does
not  receive  any  parameters.  The  print()  function  is  to  be  used  for  displaying  a  subscriber's
information: address, rate, and service type.  Also, include  an  equals() method that indicates
two subscribers are equal if they have the same street address. The equals() function returns a
Boolean and receives a NewsPaperSubscriber pointer parameter.

2. Create the three child classes of the NewsPaperSubscriber class, WeeklySubscriber,
WeekdaysSubscriber,  WeekendSubscriber.  Each  child  class  has  a  constructor  that  accepts  a
parameter to initialize the street address and calls the setRate() method to set the rate as follows:

    - WeeklySubscriber pays $10.5 per week.
    - WeekdaysSubscriber pays $7.5 per week.
    - WeekendSubscriber pays $4.5 per week.

## Note

Each child class must implement the parent’s abstract methods. See the given examples
for the format of displayed information of a subscriber.

Implement each class in a separate header file that carries the class’s name, such as:
“NewsPaperSubscriber.h”, “WeeklySubscriber.h”, etc.

## Vocareum Automatic Grading

- Upload your classes implementation files to Vocareum.
- The  driver  program  “RA9prog.cpp”  will  be  used  to  test  your  implementations.  The
program  reads  subscribers  information  for  new  subscription  requests  from  a  file  and
displays the list of new subscribers, and the list of rejected request. The program does not
allow  a  user  to  have  more  than  one  subscription  type  for  any  given  street  address.  The
grading  of  the  implementation  is  based  on  the  expected  output  from  two  test  cases.  An
example of the expected output is shown below.

    ```text
    List of new subscribers: 
    
    51 Main Street 
    Tulsa, Oklahoma 74008 
    Rate: 7.5 
    Service Type: Week Days Subscriber 
    
    78 Center Street 
    Brooklyn, NY 11237 
    Rate: 4.5 
    Service Type: Weekend Subscriber 
    
    245 Lexington Avenue 
    Manhattan, NY 10016 
    Rate: 7.5 
    Service Type: Week Days Subscriber 
    
    List of rejected subscriptions: 
    
    51 Main Street 
    Tulsa, Oklahoma 74008 
    Rate: 10.5 
    Service Type: Weekly Subscriber
    ```

## Submission Guidelines  

- Please  upload  your  implementations  to  Vocareum  in  separate  files  one  for  each  class
implementation, such as: “NewsPaperSubscriber.h”, “WeeklySubscriber.h”, etc.  
- Submissions after the due date are accepted with a fixed penalty of 25%. No submission
is accepted after Wednesday 11:59 pm, April 27, 2022.
