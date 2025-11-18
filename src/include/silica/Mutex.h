#include <silica/Macros.h>

#if \
       defined(SILICA_OS_WINDOWS) \
    || defined(SILICA_OS_LINUX) \
    || defined(SILICA_OS_MACOS)

        #include <mutex>
#endif



namespace Silica
{


/** \brief Mutex provices a synchronization mechanism that can prevent simultaneously access to shared data.

\ingroup Core
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

    /// \cond DEVELOPER_DOC
private:

    struct
    {
#if defined(SILICA_OS_WINDOWS) || defined(SILICA_OS_LINUX) || defined(SILICA_OS_MACOS)
        std::mutex mutex;
#endif
    } d;
    /// \endcond
};


/** This is a RAII guard for maintaining Mutex'es.
 *
 *  The MutexLocker can be used to lock a Mutex, and the locker will then automatically release the Mutex in its destructor.
 * \ingroup Core
 */
class MutexLocker
{

    DISABLE_COPY(MutexLocker);
    DISABLE_MOVE(MutexLocker);

public:
    /** \brief Locks the Mutex \p target.

    The constructor will block until the Mutex could be locked.

      \param target The Mutex to lock.
    */
    MutexLocker(Mutex &target);

    /** \brief Releases the locked Mutex. */
    ~MutexLocker();

    /// \cond DEVELOPER_DOC
private:
    Mutex &target;
    /// \endcond
};

}
