/**
  * Simple usage example
  *
  * @project: East Curl C++ Wrapper
  * @purpose: This library provides a wrapper of the cURL library in C++
  * @version: 1.7
  *
  *
  * @author: Mohamed Riyad
  * @created on: 13 Sep, 2018
  *
  * @url: http://ryadpasha.com
  * @email: me@ryadpasha.com
  * @license: GNU General Public License v3.0
  *
  * @see: https://github.com/ryadpasha/easycurl-cpp
  */

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
