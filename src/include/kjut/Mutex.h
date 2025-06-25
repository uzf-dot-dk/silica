

#define DISABLE_COPY(T) \
private: \
T(const T &other) = delete; \
    T& operator=(const T &rhs) = delete;

#define DISABLE_MOVE(T) \
private: \
T(T &&other) = delete; \
T& operator=(T &&rhs) = delete;

#if \
       defined(KJUT_OS_WINDOWS) \
    || defined(KJUT_OS_LINUX) \
    || defined(KJUT_OS_MACOS)

        #include <mutex>
#endif



namespace Kjut
{


class Mutex
{

    DISABLE_COPY(Mutex);
    DISABLE_MOVE(Mutex);

public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
    bool tryLock();

private:

    struct
    {
#if defined(KJUT_OS_WINDOWS) || defined(KJUT_OS_LINUX) || defined(KJUT_OS_MACOS)
        std::mutex mutex;
#endif
    } d;

};

class MutexLocker
{

    DISABLE_COPY(MutexLocker);
    DISABLE_MOVE(MutexLocker);

public:
    MutexLocker(Mutex &target);
    ~MutexLocker();

private:
    Mutex &target;
};

}
