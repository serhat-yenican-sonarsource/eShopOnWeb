//dummy comment4
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
using namespace std;
int main()
{
    shared_mutex sm;
    atomic_bool mark = false;
    // take first shared lock
    sm.lock_shared();
    // signal is taken
    thread exclusive([&]()
            {
                mark = true;
                lock_guard<shared_mutex> guard(sm);
            });
    // Wait till the thread takes the lock
    do
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    while (!mark);
    // try take the second shared lock
    bool success = sm.try_lock_shared();
    if (success)
    {
        sm.unlock_shared();
        cout << "PTHREAD_RWLOCK_PREFER_READER_NP" << endl;
    }
    else
    {
        cout << "PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP" << endl;
    }
    // release first lock
    sm.unlock_shared();
    // wait for the main thread
    exclusive.join();
    return 0;
}
