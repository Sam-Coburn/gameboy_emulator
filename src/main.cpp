#include <testromrunner.hpp>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <test_rom.gb>" << endl;
        return 1;
    }

    string output = TestRomRunner::run(argv[1], TestRomRunner::DEFAULT_MAX_CYCLES, true);
    cout << endl;

    // Exit code lets scripts tell whether the test ROM passed
    return output.find("Passed") != string::npos ? 0 : 1;
}
