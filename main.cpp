#include <kjut/Array.h>

int main() {
    Kjut::Array<int> a;
    a.append(11);
    a.append(22);
    a.append(33);
    a.append(44);

    std::cout << a[2] << std::endl;
    a[2] = 66;
    std::cout << a[2] << std::endl;
    return 0;
}

