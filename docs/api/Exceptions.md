# Exceptions and Runtime Assertions

HolorLib implements many checks at compile time, using concepts and static assertions. However, some checks (e.g., verifying if an argument is within a correct range) can be performed only at runtime. In order to allow a certain fleibility with these runtime checks, the library implements a system that assigns different levels of assertions so that only the checks below a level that is set during compilation may be enabled (refer to [Bjarne Stroustrup's *The C++ Programming Language*](https://www.stroustrup.com/4th.html)  for more details).
HolorLib defines some exceptions that may be thrown by these runtime assertions. 


## Exceptions
Defined in header  `common/exceptions.h`, within the `#!cpp namespace holor::exception`.

### HolorInvalidArgument
##### signature
``` cpp
    struct HolorInvalidArgument : public std::invalid_argument
```
##### constructor
``` cpp
    HolorInvalidArgument(const std::string& p) : std::invalid_argument(p){};
```
The argument `p` is a message.



### HolorRuntimeError
##### signature
``` cpp
    struct HolorRuntimeError : public std::runtime_error
```
##### constructor
``` cpp
    HolorRuntimeError(const std::string& p) : std::runtime_error(p){};
```
The argument `p` is a message.



## Runtime Assertions
Defined in header  `common/runtime_assertions.h`, within the `#!cpp namespace holor::assert`.


HolorLib introduces three assertion levels for runtime assertions. These levels can be assigned to runtime assertions in order to control which assertions may or may not be enabled.

The three levels are:
``` cpp
    enum AssertionLevel{
        no_checks = 0,
        release = 1,
        debug = 2
    };
```

To control which level of assertions is used in an application, the Library may be installed using Cmake with the option `DEFINE_HOLOR_ASSERT_LEVEL`, for example setting `-DDEFINE_HOLOR_ASSERT_LEVEL=AssertionLevel::no_checks`.