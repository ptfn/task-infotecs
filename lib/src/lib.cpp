#include "../include/lib.hpp"

namespace library {

void f1(std::string& str) {
   std::sort(str.begin(), str.end(), std::greater<char>());
   
   std::string result;

   for (size_t i = 0; i < str.size(); i++)
      if (i % 2 == 0)
         result += "KV";
      else
         result += str[i];

   str = result;
}

size_t f2(std::string str) {
   size_t result = 0;

   for (size_t i = 0; i < str.size(); i++)
      if (std::isdigit(str[i]))
         result += static_cast<size_t>(str[i] - '0');

   return result;
}

bool f3(std::string str) {
  return str.size() > 2 && str.size() % 32 ? true : false; 
}

}
