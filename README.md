# error-code
![example workflow](https://github.com/vasil-pashov/error-code/actions/workflows/cmake_ci.yml/badge.svg)

Small library used to wrap up status codes and messages from functions. Used as substitute for C++ exceptions.

The struct `EC::ErrorCode` has two main components the status of the error and an optional message which describes the error. Status code with value 0 indicates that there are no errors and `EC::ErrorCode::hasError()` will return false.

# Example usage
```c++
EC::ErrorCode foo(int n) {
  if(n < 0) {
    // Set the status code to 1, which indicates an error. Add message with string formating. 
    return EC::ErrorCode(1, "An error has occured. Expected number larger than 0, got %d.", n);
  } else if(n > 100) {
    // If the status code was not set explicitly in the constructor it will take the value of -1.
    // This is equivalent to EC::ErrorCode(-1, "Another error has occured");
    return EC::ErrorCode("Another error has occured");
  } else if(n > 50) {
    // Call a function which returns an error code, check the status and if it indicates an error
    // return from the function.
    const EC::ErrorCode err = bar(n);
    if(err.hasError()) {
      return err;
    }
  } else if(n > 25) {
    // Equivalent to the previous case, but the checking of the status and the return are wrapped
    // in a macro for convenience.
    RETURN_ON_ERROR_CODE(baz(n));
  }
  // The default constructor initializes object with status code 0, i.e. no error. EC::ErrorCode::hasError()
  // will return false.
  return EC::ErrorCode();
}
```
