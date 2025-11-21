#include <iostream>
#include "../include/FixedMapMemoryResource.h"
#include "../include/DoublyLinkedList.h"

struct Point {
    int x = 0;
    int y = 0;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << "," << p.y << ")";
    }
};

int main() {

    FixedMapMemoryResource mr_int(sizeof(int) + 32, 50);
    FixedMapMemoryResource mr_point(sizeof(Point) + 32, 50);

    std::cout << "=== int ===" << std::endl;
    {
        DoublyLinkedList<int> list(&mr_int);

        list.push_back(1);
        list.push_back(2);
        list.push_front(0);

        for (int x : list)
            std::cout << x << " ";

        std::cout << "\nSize: " << list.size() << "\n\n";
    }

    std::cout << "=== Point ===" << std::endl;
    {
        DoublyLinkedList<Point> list(&mr_point);

        list.push_back(Point(1, 1));
        list.push_back(Point(2, 2));
        list.push_front(Point(0, 0));

        for (const auto& p : list)
            std::cout << p << " ";

        std::cout << "\nSize: " << list.size() << "\n";
    }

    return 0;
}
