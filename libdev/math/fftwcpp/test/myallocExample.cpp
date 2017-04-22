#include <vector>
#include "myalloc.hpp"

int main()
{
    // create a vector, using MyAlloc<> as allocator
    std::vector<int,MyLib::MyAlloc<int> > v;
// "allocate" und "deallocate" müsst du dein "alloc" und dein "free" einsetzen
    // insert elements
    // - causes reallocations
    v.push_back(42);
    v.push_back(56);
    v.push_back(11);
    v.push_back(22);
    v.push_back(33);
    v.push_back(44);
}
