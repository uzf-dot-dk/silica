#define DISABLE_COPY(T) T(const T&) = delete; T&operator=(const T&) = delete;
#define DISABLE_MOVE(T) T(T&&) = delete; T&operator=(T&&) = delete;
