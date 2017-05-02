// a thread pool using polymorphism and a thread-safe queue
// there are some pretty big design issues with this code
// what are they?

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <string>

#include "message_queue.h"

// define a base class to encapsulate a unit of work
class WorkBase
{
public:
  virtual void run() = 0;  
};

// make the message a WorkBase pointer
typedef WorkBase * MessageType;
typedef message_queue<MessageType> MessageQueue;

// a thread function to remove work and run it
// exits on a nullptr message
void thread_func(MessageQueue * inq, MessageQueue * outq)
{
  while(true){

    // take a unit fo work from the input queue
    MessageType m;
    inq->wait_and_pop(m);

    if(m == nullptr) break;
    
    // execute m
    m->run();

    // put the result back into the output queue
    outq->push(m);
  }
}

class ThreadPool
{
public:

  ThreadPool(int n, MessageQueue * input_queue, MessageQueue * output_queue){
    
    for(int i = 0; i < n;++i){
      pool.emplace_back(std::thread(&thread_func, input_queue, output_queue));
    }
  }

  void joinAll(){
    for(auto &t : pool) t.join();
  }
  
private:

  std::vector<std::thread> pool;
};
  
// now create a couple of work units
class Work1: public WorkBase
{
public:
  void run(){
    result = "This is the result of Work 1";
  }

  std::string get(){
    return result;
  }
  
private:
  std::string result;
};

class Work2: public WorkBase
{
public:
  void run(){
    result = 42;
  }

  int get(){
    return result;
  }
  
private:

  int result;
};

  
int main()
{
  MessageQueue inq, outq;
  
  ThreadPool P(3, &inq, &outq);

  Work1 w1;
  inq.push(&w1);

  Work2 w2;
  inq.push(&w2);

  // cancel the 3 threads
  inq.push(nullptr);
  inq.push(nullptr);
  inq.push(nullptr);

  P.joinAll();

  while(!outq.empty()){
    MessageType m;
    outq.wait_and_pop(m);

    if( Work1 *rp = dynamic_cast<Work1 *>(m) ){
      std::cout << "Message is a Work1" << std::endl;
      std::cout << "Result: " << rp->get() << std::endl;
    }
    else if( Work2 *rp = dynamic_cast<Work2 *>(m) ){
      std::cout << "Message is a Work2" << std::endl;
      std::cout << "Result: " << rp->get() << std::endl;
    }
    else{
      std::cout << "Unknown Work Message Type!" << std::endl;
    }
  }
  
  return 0;
}
