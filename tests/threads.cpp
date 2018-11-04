// 
// Threading with Boost - Part I: Creating Threads
// ===============================================
// 
// By Gavin Baker <gavinb@antonym.org>
// This package contains the source code examples to accompany the article:
// 
//   http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
// 
// The source is provided as-is, with no warranties express or implied.  Use it
// however you see fit.
// 


#include <iostream>
#include <thread>

class Worker
{
public:
    Worker() 
    {
        // the thread is not-a-thread until we call start()
    }

    void start(int N)
    {
        m_Thread = std::thread(&Worker::processQueue, this, N);
    }

    void join()
    {
        m_Thread.join();
    }
    
    void processQueue(unsigned N)
    {
        float ms = N * 1e3;
        std::posix_time::milliseconds workTime(ms);

        std::cout << "Worker: started, will work for "
                  << ms << "ms"
                  << std::endl;

        // We're busy, honest!
        std::this_thread::sleep(workTime);

        std::cout << "Worker: completed" << std::endl;
    }

private:
    std::thread m_Thread;
};

int main(int argc, char* argv[])
{
    std::cout << "main: startup" << std::endl;

    Worker worker;

    worker.start(3);

    std::cout << "main: waiting for thread" << std::endl;

    worker.join();

    std::cout << "main: done" << std::endl;

    return 0;
}
