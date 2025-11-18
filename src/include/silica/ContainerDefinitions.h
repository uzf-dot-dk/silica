#include <silica/LoggingSystem.h>
#ifndef DOXYGEN
    #define ContainerWarning(message, ...) WARN(message, ##__VA_ARGS__)
#endif

#ifndef SILICA_ARRAY_INITIAL_CAPACITY
    /*! This  define specifies the default capacity of dynamic Kjut::Array<T,S> (where S == 0). You may spacify any non zero capacity as pre processor directive when buildling.
    E.g.:

    <code>
        g++ -DSILICA_ARRAY_INITIAL_CAPACITY=100 myFile.cpp ...
    </code>
    */
    #define SILICA_ARRAY_INITIAL_CAPACITY 16
#endif

// ----------------------------------------------------------------
// INITIALIZER LIST CONSTRUCTOR

/*

#define SILICA_TRY try {
#define SILICA_CATCH(Type) } catch( Type e ) {
#define SILICA_CATCH_ALL } catch( ... ) {
#define SILICA_TRY_END }
#else
#define SILICA_TRY {
#define SILICA_CATCH_ALL } if( false ) {
#define SILICA_CATCH(Type)  } if ( false ) {
#define SILICA_TRY_END }
#endif

*/

// ----------------------------------------------------------------
// ENABLE / DISABLE EXCEPTIONS

#ifdef DOXYGEN
    /*!
    Disables exceptions in any and all Kjut code.
    By default, exceptions are supported. This does not disable exception as a compilation feature, but only the parts in the Kjut code that handles exceptions.
    \note Should both \c SILICA_DISABLE_EXCEPTIONS and \ref SILICA_ENABLE_EXCEPTIONS be defined, \ref SILICA_ENABLE_EXCEPTIONS takes precedence.
    */
#define SILICA_DISABLE_EXCEPTIONS


    /*!
    Disables exceptions in any and all Kjut code.
    By default, exceptions are supported.
    \note Should both \ref SILICA_DISABLE_EXCEPTIONS and \c SILICA_ENABLE_EXCEPTIONS be defined, \c SILICA_ENABLE_EXCEPTIONS takes precedence.

    */
#define SILICA_ENABLE_EXCEPTIONS
#endif

#ifndef DOXYGEN
    #ifndef SILICA_ENABLE_EXCEPTIONS
        #ifndef SILICA_DISABLE_EXCEPTIONS
            #define SILICA_ENABLE_EXCEPTIONS
        #endif
    #endif

    #ifdef SILICA_ENABLE_EXCEPTIONS
        #define SILICA_TRY try {
        #define SILICA_CATCH(Type) } catch( Type e ) {
        #define SILICA_CATCH_ALL } catch( ... ) {
        #define SILICA_TRY_CATCH_END }
    #else
        #define SILICA_TRY
        #define SILICA_CATCH_ALL
        #define SILICA_CATCH(Type)
        #define SILICA_TRY_CATCH_END
    #endif

#endif


// ----------------------------------------------------------------
// LIST INITIALIZERS FOR CONTAINERS

#ifdef DOXYGEN
    /*!
     Disables initializer list constructors for all container classes.
    By default, initializer lists are enabled.
    \note Should both \c SILICA_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR and \ref SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR be defined, \ref SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR takes precedence.
    */
#define SILICA_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR


    /*!
    Enables initializer list constructors for all container classes.
    By default, initializer lists are enabled.
    \note Should both \ref SILICA_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR and \c SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR be defined, \c SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR takes precedence.

    */
#define SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
#endif

#ifndef DOXYGEN
#ifndef SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
#ifndef SILICA_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
#define SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
#endif
#endif
#endif



// ----------------------------------------------------------------
// COPY CONSTRUCTORS

#ifdef DOXYGEN
    /*! Disables copy constructors for all container classes.
    By default, copy constructors are disabled.
        \note Should both \c SILICA_DISABLE_CONTAINERS_COPY_CONSTRUCTOR and \ref SILICA_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR be defined, \ref SILICA_ENABLE_CONTAINERS_COPY_CONSTRUCTOR takes precedence.
*/
#define SILICA_DISABLE_CONTAINERS_COPY_CONSTRUCTOR


    /*!  Disables copy constructors for all container classes.
       By default, copy constructors are disabled.
        \note Should both \ref SILICA_DISABLE_CONTAINERS_COPY_CONSTRUCTOR and \c SILICA_ENABLE_CONTAINERS_COPY_CONSTRUCTOR be defined, \c SILICA_ENABLE_CONTAINERS_COPY_CONSTRUCTOR takes precedence.

    */
#define SILICA_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
#endif


#ifndef DOXYGEN
    #ifndef SILICA_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
        #ifndef SILICA_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
            #define SILICA_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
        #endif
    #endif
#endif

// ----------------------------------------------------------------
// MODIFIERS

    /*! This macro can be used to make data members volatile. For example
    ```cpp
    class Foo
    {
    public:
        SILICA_VOLATILE int sharedBetweenISRAndMaincode;
    };
    ```

    Exactely how \c SILICA_VOLATILE expands is dependent on the target platform.
    */
#define SILICA_VOLATILE //Figure out how to make things volatile without breaking const correctness.... Shit fuck!
