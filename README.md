# KAMOME
## A header-only library for fixed-point formats in C++

Since KAMOME requires a inline static member, use C++17 or more.

## Examples

```c++
#include <cstdio>
#include "kamome/types.hpp"

using namespace kmm;

int main() {
  fixed32_t<0>::change_point(16);
  fixed32_t<0> a = 1433.1433;
  
  printf("%f\n", double(a)); // 1433.143295
  
  return 0;
}
```

