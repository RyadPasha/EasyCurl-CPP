## Easy Curl C++ wrapper
This library provides a wrapper of the C++ libCurl.

If you want to know a bit more about cURL and libCurl, you should go on the official website http://curl.haxx.se

## Simple usage example
```
#include <iostream>
#include "easycurl.h"

int main(int argc, char **argv){
   try{
      EasyCurl ob("http://api.ryadpasha.com/");
      std::cout << ob.fetch();
      std::cout << std::endl;
   }

   catch (const EasyCurl::Exception &e){
      std::cerr << "Exception thrown: " << e.what() << std::endl;
   }

   return 0;
}
```

## Discussion
If you have questions or problems with installation or usage [create an Issue](https://github.com/ryadpasha/easycurl-cpp).

For any queries contact me at: **me@ryadpasha.com**
