#define ContainerWarning(message, ...) createLogEntry(__LINE__, __FILE__, message, ##__VA_ARGS__)

#define KJUT_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR

#ifndef KJUT_ARRAY_INITIAL_CAPACITY
    #define KJUT_ARRAY_INITIAL_CAPACITY 10
#endif

#ifndef KJUT_ENABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
    #ifndef KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
        #define KJUT_DISABLE_CONTAINERS_INITIALIZER_LIST_CONSTRUCTOR
    #endif
#endif

#ifndef KJUT_ENABLE_CONTAINERS_COPY_CONSTRUCTOR
    #ifndef KJUT_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
        #define KJUT_DISABLE_CONTAINERS_COPY_CONSTRUCTOR
    #endif
#endif

#define KJUT_VOLATILE //Figure out how to make things volatile without breaking const correctness.... Shit fuck!
