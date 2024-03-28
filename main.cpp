#include "Editor.h"

int main() {
    std::string filename = "test.txt";
    Editor editor(filename);
    editor.run();
    return 0;
}