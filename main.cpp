

#include <iostream>
#include <chrono>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

const unsigned int NUM_PHILOSO = 5;

class Semaphore
{
public:
Semaphore (int count_ = 0):count (count_)
  {
  }

  inline void notify (int tid)
  {
    std::unique_lock < std::mutex > lock (mtx);
    count++;
    cout << "thread " << tid << " notify" << endl;
    //notify the waiting thread
    cv.notify_one ();
  }
  inline void wait (int tid)
  {
    std::unique_lock < std::mutex > lock (mtx);
    while (count == 0)
      {
	cout << "thread " << tid << " wait" << endl;
	//wait on the mutex until notify is called
	cv.wait (lock);
	cout << "thread " << tid << " run" << endl;
      }
    count--;
  }
  int waiting()
  {
      return count;
  }
private:
  std::mutex mtx;
  std::condition_variable cv;
  int count;
};

struct Fork
{
        Fork()
        {
            
        };
        // ~Fork();
        mutex mx;
    
};

class Philos
{
    public:
        Philos(pair<Fork*, Fork*> forkpair, int id): forks(forkpair), id(id), t(&Philos::Eat, this){};
        void Eat()
        {
            unique_lock<mutex> lck(forks.first->mx);
            unique_lock<mutex> lck2(forks.second->mx);
            cout << id << " is eating!" << endl;
            std::chrono::milliseconds timeout(500);
            std::this_thread::sleep_for(timeout);
        };
        ~Philos()
        {
            t.join();  
        };
    private:
        thread t;
        pair<Fork*, Fork*> forks;
        int id;
};

class testclass
{
    public:
        testclass(pair<int*, int*> testpairinp): pairtest(testpairinp){};
        int getfirst()
        {
            
            return* pairtest.first;
        }
        
    private:
        int * jsd;
        pair<int*, int*> pairtest;
};

int
main ()
{
    Semaphore seph;
    vector<Fork*> forkvec;
    vector<Philos*> philosofs;
    forkvec.reserve(NUM_PHILOSO);
    philosofs.reserve(NUM_PHILOSO);
    // Fork t;
    for(int i=0; i<NUM_PHILOSO; i++)
    {
        forkvec.push_back(new Fork());
    }

    for(int i=0; i<NUM_PHILOSO; i++)
    {
        if(i==0)
        {
            philosofs.push_back(new Philos(make_pair(forkvec.at(NUM_PHILOSO-1), forkvec.at(i)), i+1)); // This works! I tested with pntrs
        }
        else
        {
            philosofs.push_back(new Philos(make_pair(forkvec.at(i-1), forkvec.at(i)), i+1));
        }
    }
    
    for(int i=0; i<NUM_PHILOSO; i++)
    {
        delete philosofs.at(i);
    }
    

  

  cout << "Program has ended." << endl;
  return 0;
}
