#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <cstdlib>

std::string generateName()
{
  std::string temp(8, ' ');
  for(unsigned int i = 0; i != 8; ++i)
    temp[i] = alphaNum[rand() / (RAND_MAX / sizeof(alphaNum))];

  return temp;
}
