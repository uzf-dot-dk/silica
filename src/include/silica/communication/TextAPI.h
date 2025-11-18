#include <cstring>
#include <functional>
#include <string>
#include <cstdio>
#include <charconv>

#ifndef SILICA_TEXT_API_H
#define SILICA_TEXT_API_H

#ifndef TEXT_API_MAX_TOKENS
#define TEXT_API_MAX_TOKENS 50
#endif //TEXT_API_MAX_TOKENS

#ifndef TEXT_API_STRING_RESPONSE_MAX_LENGTH
#define TEXT_API_STRING_RESPONSE_MAX_LENGTH 100
#endif

#ifndef TEXT_API_STRING_MAX_EXPOSED_METHODS
#define TEXT_API_STRING_MAX_EXPOSED_METHODS 50
#endif




#ifndef TEXT_API_MAP_TYPE
#include <silica/Map.h>
#define TEXT_API_MAP_TYPE \
Kjut::Map< \
           std::string_view, \
           std::function<Kjut::APICallResult<MaximumStringlengthOfTextResponse>(C*, const VectorOfTokens &)>, \
           TEXT_API_STRING_MAX_EXPOSED_METHODS \
           >

#define TEXT_API_MAP_INSERT(map, key, value) map.insert(key, value);
#endif // TEXT_API_MAP_TYPE

#ifndef TEXT_API_VECTOR_TYPE
#include <silica/Array.h>

#define TEXT_API_VECTOR_TYPE Kjut::Array<Kjut::Token,TEXT_API_MAX_TOKENS>
#ifdef TEXT_API_VECTOR_TYPE_APPEND
#undef TEXT_API_VECTOR_TYPE_APPEND
#endif
#define TEXT_API_VECTOR_TYPE_APPEND append

#ifdef TEXT_API_VECTOR_TYPE_REMOVE_FIRST
#undef TEXT_API_VECTOR_TYPE_REMOVE_FIRST
#endif

#define TEXT_API_VECTOR_TYPE_REMOVE_FIRST(tokens) tokens.remove(0)

#endif // TEXT_API_VECTOR_TYPE


namespace Kjut
{

typedef struct  {
    const char *p;
    size_t len;
} Token;


class ConversionError
{
public:
    ConversionError() {}
    ~ConversionError() {}
    ConversionError(const Token &t, const char *type) : value(t.p, t.len), typeNameWithIndefiniteArticle(type) {}
    std::string_view value;
    std::string_view typeNameWithIndefiniteArticle;
};


class ParameterCountError
{
public:
    ParameterCountError() {}
    ~ParameterCountError() {}
    ParameterCountError(size_t expectedCount, size_t actualCount) : expectedCount(expectedCount), actualCount(actualCount) {}
    size_t expectedCount;
    size_t actualCount;
};




template <typename VectorOfTokens>
bool tokenize(const char *src, VectorOfTokens &tokens)
{
    if (!src) return false;

    const char *p = src;

    while (*p) {
        // Skip whitespace
        while (std::isspace(static_cast<unsigned char>(*p))) ++p;
        if (!*p) break;

        const char *start = nullptr;

        if (*p == '"') {
            // Quoted token
            ++p; // skip opening quote
            start = p;

            const char *q = p;
            while (*q) {
                if (*q == '\\' && *(q + 1) == '"') {
                    q += 2; // skip escaped quote
                    continue;
                }
                if (*q == '"') break;
                ++q;
            }

            tokens.TEXT_API_VECTOR_TYPE_APPEND({ start, static_cast<size_t>(q - start) });

            if (*q == '"') q++; // skip closing quote
            p = q;
        } else {
            // Unquoted token
            start = p;
            while (*p && !std::isspace(static_cast<unsigned char>(*p))) ++p;
            tokens.TEXT_API_VECTOR_TYPE_APPEND({ start, static_cast<size_t>(p - start) });
        }
    }

    return true;
}

template <typename T>
T fromToken(const Token &t);

template <typename T>
void toString(const T& value, char * destination, size_t destinationSize);


template <>
void toString<int>(const int& value, char * destination, size_t destinationSize)
{
    snprintf(destination, destinationSize-1, "%d", value);
}


template <>
void toString<double>(const double& value, char * destination, size_t destinationSize)
{
    snprintf(destination, destinationSize-1, "%e", value);
}

template <>
inline std::string_view fromToken<std::string_view>(const Token &t) {
    return std::string_view(t.p, t.len);
}

template <>
inline bool fromToken<bool>(const Token &t) {

    auto areStringsCaseInsensitiveEqual = +[](const char* a, const char* b, size_t lena, size_t lenb) -> bool {
        if(lena != lenb)
        {
            return false;
        }
        for (size_t i = 0; i < lena; ++i) {
            if (std::tolower(static_cast<unsigned char>(a[i])) !=
                std::tolower(static_cast<unsigned char>(b[i]))) {
                return false;
            }
        }
        return true;
    };

    const char * trueOptions[] =
        {
            "true", "yes", "on", "high", "1", 0
        };
    const char * falseOptions[] =
        {
            "false", "no", "off", "low", "0", 0
        };
    const char * current;
    size_t i = 0;

    for(i = 0; trueOptions[i]; i++)
    {
        if( areStringsCaseInsensitiveEqual(t.p, trueOptions[i], t.len, strlen(trueOptions[i])) )
        {
            return true;
        }
    }

    for(i = 0; falseOptions[i]; i++)
    {
        if( areStringsCaseInsensitiveEqual(t.p, falseOptions[i], t.len, strlen(falseOptions[i])) )
        {
            return false;
        }
    }
    throw ConversionError(t, "a boolean");
}




template <>
inline int fromToken<int>(const Token &t) {
    const char* begin = t.p;

    const char* end = t.p + t.len;

    int value;
    int base = 10;

    if(t.len > 2 and t.p[0] == '0' and t.p[1] == 'x')
    {
        base = 16;
        begin += 2;
    }
    else if(t.len > 2 and t.p[0] == '0' and t.p[1] == 'b')
    {
        base = 2;
        begin += 2;
    }
    else
    {
        base = 10;
    }

    auto result = std::from_chars(begin, end, value, base);

    if (result.ec != std::errc{} || result.ptr != end) {
        throw ConversionError(t ,"an integer");
    }

    return value;
}

template <>
inline double fromToken<double>(const Token &t)
{
    const char* begin = t.p;
    const char* end = t.p + t.len;

    double value;

    auto result = std::from_chars(begin, end, value);

    if (result.ec != std::errc{} || result.ptr != end) {
        throw ConversionError(t ,"a double");
    }

    return value;
}



template <typename VectorOfTokens, typename... Ts, size_t... Is>
std::tuple<Ts...> convertParametersToTypesImpl(const VectorOfTokens & params, std::index_sequence<Is...>) {
    return std::make_tuple(fromToken<Ts>(params[Is])...);
}



template <typename VectorOfTokens, typename... Ts>
std::tuple<Ts...> convertParametersToTypes(const VectorOfTokens & params) {
    if (params.size() != sizeof...(Ts))
    {
        throw ParameterCountError(sizeof...(Ts), params.size());
    }
    return convertParametersToTypesImpl<VectorOfTokens, Ts...>(params, std::index_sequence_for<Ts...>{});
}



template <size_t S = 100>
class APICallResult
{
public:

    APICallResult() { output[0] = 0; returnCode = 0;};
    APICallResult(int v) : returnCode(v) { output[0] = 0; }
    ~APICallResult() = default;

    char output[S+1];
    const size_t maxSize = S;
    int returnCode;
};



template <typename C, typename Func>
struct NamedMemberFunc {
    std::string_view displayName;
    Func functionPointer;
};

// Immutable compile-time array of NamedMemberFunc
template <typename C, typename... Entries>
struct MemberFuncArray {
    std::tuple<Entries...> entries;

    // default constructor
    constexpr MemberFuncArray() : entries{} {}

    // constructor from a tuple
    constexpr MemberFuncArray(std::tuple<Entries...> t) : entries(t) {}


    // push returns a new MemberFuncArray with the new entry appended
    template <typename Func>
    constexpr auto push(std::string_view name, Func f) const {
        return MemberFuncArray<C, Entries..., NamedMemberFunc<C, Func>>{
            std::tuple_cat(entries, std::make_tuple(NamedMemberFunc<C, Func>{name, f}))
        };
    }

    // get entry by index
    template <size_t I>
    constexpr auto get() const {
        return std::get<I>(entries);
    }

    constexpr size_t size() const { return sizeof...(Entries); }

    // iterate over all entries and call a lambda
    template <typename Lambda>
    void for_each(Lambda&& lambda) const {
        std::apply([&](auto const&... e) { ((lambda(e)), ...); }, entries);
    }
};

template <std::size_t N, typename F>
constexpr void constexpr_for(F&& f) {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (f(std::integral_constant<std::size_t, Is>{}), ...);
    }(std::make_index_sequence<N>{});
}



/**
\brief TextBasedApi is a class that, when properly subclasses, accepts a string and executes the method with the right parameters provided in the string.

An example is

```c++
class MyApi: public Kjut::TextBasedAPI<MyApi>
{

public:

    MyApi() : Kjut::TextBasedAPI<MyApi>()
    {
        registerCommand("addNumbers", &MyApi::addNumbers)
    }

    int addNumbers(int a, int b) { return a + b; }
};

...

MyApi api;
std::cout << api.execute("addNumbers 2 4").output << "\n";  // Prints 6

```


To use other containers, for example the std::vector and std::map, define the following prior to include

```c++


#include <map>
#define TEXT_API_MAP_TYPE \
std::map< \
          std::string_view, \
          std::function<APICallResult<MaximumStringlengthOfTextResponse>(C*, const VectorOfTokens &)> \
    >
#define TEXT_API_MAP_INSERT(map, key, value) map[key] = value;

#include <vector>
#define TEXT_API_VECTOR_TYPE std::vector<Kjut::Token>
#define TEXT_API_VECTOR_TYPE_APPEND push_back
#define TEXT_API_VECTOR_TYPE_REMOVE_FIRST(tokens) tokens.erase(tokens.begin(), tokens.begin()+1);

```


*/



template <typename C,
         size_t MaximumStringlengthOfTextResponse = TEXT_API_STRING_RESPONSE_MAX_LENGTH,
         typename VectorOfTokens = TEXT_API_VECTOR_TYPE,
         typename MapOfWrappers = TEXT_API_MAP_TYPE>
class TextBasedAPI
{
public:

    enum class Error
    {
        NoSuchMethod = 130,
        ConversionError = 131,
        ParameterCountMismatch = 132,
        MethodThrewException = 133
    };

protected:
    constexpr auto createArrayOfMemberFunctionsToRegister()
    {
        const int SOME_VALUE_TO_MAKE_THE_COMPILER_HAPPY = 1;
        return SOME_VALUE_TO_MAKE_THE_COMPILER_HAPPY;

// START -- Reenable this block, when reflection becomes available
#ifdef REFLECTION_FINALLY_GOT_HERE

        Populate this list by reflection

            auto arr1 = arr0.push("chant", &C::chant);
        auto arr2 = arr1.push("storeTruth", &C::storeTruth);
        auto arr3 = arr2.push("doStuff", &C::doStuff);
        auto arr4 = arr3.push("addStuff", &C::addStuff);
        auto arr5 = arr4.push("doNuffin", &C::doNuffin);
        auto arr0 = MemberFuncArray<C>{};
        return arr5;
#endif \
    // END -- Reenable this block, when reflection becomes available
    }

private:

    template <typename R, typename ...Ts>
    R callByMagic(C* instance, R (C::*methodPointer)(Ts...), std::tuple<Ts...>& args) {
        return std::apply([&](auto&&... unpacked) {
            return (instance->*methodPointer)(std::forward<decltype(unpacked)>(unpacked)...);
        }, args);
    }

    int exitCode;


protected:
    void setExitCode(int newExitCode) { this->exitCode = newExitCode; }

public:

    TextBasedAPI()
    {
        auto registered =  createArrayOfMemberFunctionsToRegister();
// START -- Reenable this block, when reflection becomes available
#ifdef REFLECTION_FINALLY_GOT_HERE
        registered.for_each([this](auto const& entry){
            registerCommand(entry.displayName.data(), entry.functionPointer);
        });
#endif
        // END -- Reenable this block, when reflection becomes available

    }
    virtual ~TextBasedAPI() = default;



    template <typename R, typename ...Ts>
    void registerCommand(const char *name, R (C::*methodPointer)(Ts...))
    {
        auto wrapper = [=](C *instance, const VectorOfTokens & parametersAsText)-> APICallResult<MaximumStringlengthOfTextResponse>
        {
            std::tuple<Ts...> parameters = convertParametersToTypes<VectorOfTokens, Ts...>(parametersAsText);

            APICallResult<MaximumStringlengthOfTextResponse> result;
            if constexpr (!std::is_void_v<R>)
            {
                R returnValue =
                    std::apply([&](auto&&... parameters) {
                        return (instance->*methodPointer)(std::forward<decltype(parameters)>(parameters)...);
                    }, parameters);

                toString(returnValue, result.output, result.maxSize);
            }
            else
            {
                std::apply([&](auto&&... parameters) {
                    return (instance->*methodPointer)(std::forward<decltype(parameters)>(parameters)...);
                }, parameters);
            }

            return result;
        };

        TEXT_API_MAP_INSERT(wrappers, std::string_view(name), wrapper);

    }

    APICallResult<MaximumStringlengthOfTextResponse> executeImpl(Token commandName, const VectorOfTokens & parametersAsText = {})
    {
        setExitCode(0);
        if (!wrappers.contains(std::string_view(commandName.p, commandName.len))) {
            APICallResult<MaximumStringlengthOfTextResponse> returnValue;
            returnValue.returnCode = static_cast<int>(Error::NoSuchMethod);
            snprintf(returnValue.output,
                     returnValue.maxSize,
                     "#:! Unknown command: %.*s",
                     static_cast<int>(commandName.len), commandName.p);
            return returnValue;
        }

        std::function<APICallResult<MaximumStringlengthOfTextResponse>(C*, const VectorOfTokens &)> & wrapper = wrappers[std::string_view(commandName.p, commandName.len)];

        try
        {
            return wrapper(reinterpret_cast<C*>(this), parametersAsText);
        }
        catch(const ConversionError &ce)
        {
            APICallResult<MaximumStringlengthOfTextResponse> returnValue;
            returnValue.returnCode = static_cast<int>(Error::ConversionError);
            snprintf(returnValue.output,
                     returnValue.maxSize,
                     "#:! Could not convert %.*s to %.*s",
                     static_cast<int>(ce.value.length()), ce.value.data(),
                     static_cast<int>(ce.typeNameWithIndefiniteArticle.length()), ce.typeNameWithIndefiniteArticle.data()
                     );
            return returnValue;
        }
        catch(const ParameterCountError &pce)
        {
            APICallResult<MaximumStringlengthOfTextResponse> returnValue;
            returnValue.returnCode = static_cast<int>(Error::ParameterCountMismatch);
            snprintf(returnValue.output,
                     returnValue.maxSize,
                     "#:! Wrong number of parameters proviced; %.*s expects %zu but %zu was given.",
                     static_cast<int>(commandName.len), commandName.p,
                     pce.expectedCount,
                     pce.actualCount
                     );
            return returnValue;
        }
        catch( ... )
        {
            APICallResult<MaximumStringlengthOfTextResponse> returnValue;
            returnValue.returnCode = static_cast<int>(Error::MethodThrewException);
            snprintf(returnValue.output,
                     returnValue.maxSize,
                     "#:! Somthing unknown went wrong while calling %.*s.",
                     static_cast<int>(commandName.len), commandName.p
                     );
            return returnValue;
        }
    }

    APICallResult<MaximumStringlengthOfTextResponse> execute(const char *commandline)
    {
        VectorOfTokens tokens;
        tokenize<VectorOfTokens>(commandline, tokens);

        if(tokens.size() < 1)
        {
            APICallResult<MaximumStringlengthOfTextResponse> returnValue;
            returnValue.returnCode = 0;
            return returnValue;
        }
        Token name = tokens[0];
        TEXT_API_VECTOR_TYPE_REMOVE_FIRST(tokens);
        auto r = executeImpl(name, tokens);
        return r;

    }

    MapOfWrappers wrappers;

};

} //namespace Kjut


#endif // SILICA_TEXT_API_H
