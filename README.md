# PicoGine

## The Concept behind it
PicoGine is a 3D game engine written in c++ that is focused on structure and modulatiry. This project allows me to summarize all the knowledge acquired throughout my studies, and combine them in a coherent project.
To implement that structure and modularity, several programming patterns are used. A good example is "Pointer to Implementation" aka pimpl. This pattern moves the actual implementation of a class to it's cpp file, and the header only consists of an abstracted interfae, that forwards the function calls to te used implementation. This allows for multiple implementations, all following the same interface and thus remain transparent to the code that uses it. It also contains the include dependencies local to the implementations'compilation unit instead of propagating them to the rest of the project.

## Main implemented features
### CMake build system
Made in a separate build folder to avoid cluttering the source tree, the CMake build system allows for an easy build of both the engine's library and the game, as well as creating an installer, in case you want to publish your project or share it to friends for testing. It also allows to easily swap for a static library to a DLL, in case multiple projects share the same engine library.

### Win32
Win32 is used to create the window(s) required for the game. It is also used to process device inputs for the keyboard and mouse and forwards them to the input manager. With this, no intermediate layer like SDL is required to interface with the operating system.

### DirectX 11

### DirectX 12
