#include <Worker/Worker.h>

int main()
{
  Worker x("text.in", "text.out");
  x.start();
  return 0;
}