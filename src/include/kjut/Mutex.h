

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


/** \brief Mutex provices a synchronization mechanism that can prevent simultaneously access to shared data.
 */
class Mutex
{

    DISABLE_COPY(Mutex);
    DISABLE_MOVE(Mutex);

public:
    /** \brief Creates a new unlocked Mutex. */
    Mutex();
    ~Mutex();

    /** \brief Locks this Mutex.
     *
     *  Calling lock() will prevent further execution in the calling thread until this Mutex can be locked. */
    void lock();

    /** \brief Unlocks this Mutex allowing it to be locked again. */
    void unlock();

    /** \brief Tries to lock this Mutex but does not block if this Mutex is already locked.
     *  \returns True if this Mutex could be locked, false if not. */
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
