#include <kjut/Array.h>

// Function taking reference to Array<T>
template <typename T>
void foo(Kjut::Array<T>& l) {
    //l.dump();
}

int main() {
    Kjut::Array<int, 15> fifteenFixed;
    Kjut::Array<int> dynamic; // List<int, 0>
    for (int i = 0; i < 17; ++i) {
        fifteenFixed.append(i);
        dynamic.append(i);
    }

    foo(fifteenFixed);
    foo(dynamic);

    return 0;
}

