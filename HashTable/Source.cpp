#include "Index.h"
#include <iostream> //cout
#include <memory> //shared pointer
#include <vector> //vector
#include <string> //string
#include <sstream> //stringstream
#include <random> //random numbers
#include <thread> //timeout thread
//high resolution timers
#include <ctime>
#include <ratio>
#include <chrono>
#define SEPARATOR '|'
bool test1(bool show) {
    Index i;
    std::shared_ptr<std::string> simple = std::make_shared<std::string>("Hello");
    i.Insert(simple, 77);
    int result = i.Find(simple);
    return result == 77;
}
bool test2(bool show) {
    Index index;
    for (int i = 0; i < 10000; i++) {
        std::stringstream s;
        s << i;
        std::shared_ptr<std::string> simple =
            std::make_shared<std::string>(s.str());
        index.Insert(simple, i);
    }
    std::shared_ptr<std::string> search = std::make_shared<std::string>("7341");
    int result = index.Find(search);
    return result == 7341;
}
bool test3(bool show) {
    Index index;
    std::random_device rd;
    std::mt19937 generator(rd()); //Mersenne Twister
    std::uniform_int_distribution<int> uniform_distribution(
        0,
        200000);
    int unique = 0;
    for (int i = 0; i < 100000; i++) {
        std::stringstream s;
        int val = uniform_distribution(generator);
        s << val;
        std::shared_ptr<std::string> key = std::make_shared<std::string>(s.str());
        if (index.Find(key) < 0) {
            index.Insert(key, val);
            unique++;
        }
    }
    int found = 0;
    for (int i = 0; i < 200000; i++) {
        std::stringstream s;
        s << i;
        std::shared_ptr<std::string> key = std::make_shared<std::string>(s.str());
        int val = index.Find(key);
        if (val == i) {
            found++;
        }
    }
    return found > 0;
}
bool test4(bool show) {
    Index index;
    for (int i = 0; i < 100000; i++) {
        std::stringstream s;
        s << i;
        std::shared_ptr<std::string> simple =
            std::make_shared<std::string>(s.str());
        index.Insert(simple, i);
    }
    std::shared_ptr<std::string> search = std::make_shared<std::string>("77341");
    index.Remove(search);
    int result = index.Find(search);
    return result == -1;
}
bool test5(bool show) {
    Index index;
    std::random_device rd;
    std::mt19937 generator(rd()); //Mersenne Twister
    std::uniform_int_distribution<int> uniform_distribution(
        0,
        2000000);
    std::vector<std::shared_ptr<std::string>> keys;
    for (int i = 0; i < 100000; i++) {
        std::stringstream s;
        int val = uniform_distribution(generator);
        s << val;
        std::shared_ptr<std::string> key = std::make_shared<std::string>(s.str());
        keys.push_back(key);
        index.Insert(key, val);
    }
    for (std::shared_ptr<std::string>& s : keys) {
        if (index.Find(s) >= 0) {
            index.Remove(s);
        }
        if (index.Find(s) >= 0) {
            return false;
        }
    }
    return true;
}
void testCase(int testNumber, bool show) {
    std::cout << testNumber << SEPARATOR;
    auto begin = std::chrono::high_resolution_clock::now();
    bool passed = false;
    switch (testNumber) {
    case 1: passed = test1(show); break;
    case 2: passed = test2(show); break;
    case 3: passed = test3(show); break;
    case 4: passed = test4(show); break;
    case 5: passed = test5(show); break;
    default: std::cout << "Unknown test case.";
    }
    std::cout << ((passed) ? "passed" : "failed");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << SEPARATOR;
    std::chrono::duration<double> time_span = end - begin;
    std::cout << time_span.count();
}
int main(int argc, char** argv) {
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::seconds(60));
        std::cout << "Timer elapsed." << std::endl;
        exit(1);
        });
    bool showDebugOutput = true;
    testCase(1, showDebugOutput); // simple insert
    std::cout << SEPARATOR << std::flush;
    testCase(2, showDebugOutput); // simple find
    std::cout << SEPARATOR << std::flush;
    testCase(3, showDebugOutput); // simple remove
    std::cout << SEPARATOR << std::flush;
    testCase(4, showDebugOutput); // complex insert/find
    std::cout << SEPARATOR << std::flush;
    testCase(5, showDebugOutput); // complex insert/find/remove
    std::cout << std::endl;
    exit(0);
}

/*
#include "Index.h"

int main() {

	Index i; 
	shared_ptr<string> simple = make_shared<string>("Hello");
	i.Insert(simple, 77);
	shared_ptr<string> simple1 = make_shared<string>("Sarah");
	i.Insert(simple1, 60);
	shared_ptr<string> simple2 = make_shared<string>("Myra");
	i.Insert(simple2, 20);

}
*/