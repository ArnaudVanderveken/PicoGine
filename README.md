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
I started with DirectX 11, as it was the graphics API I was the most accustomed to from my graphics courses. During these, we were using the Effects Framework as well, so the entire shader is written in a single .fx file, and it also simplifies data transfer to the GPU. In this projects, I decided then to not use that framework to be fully accustomed to DirectX11.

### DirectX 12
After I had a simple imlpementation of DirectX11, I wanted to experiment with DirectX 12 to be more in line with todays technologies used in the studios. This is still work in progress, as there are many more steps to be done before being able to render even a simple test triangle. Here are some elements already implemented: Commands, Command frames, descriptor heap allocators. Note these couldn't be properly tested, as the rest isn't setup yet.

### Consequences of using Pimpl for the renderer
Other elements in the engine needs to be "pimpl'ed" too, if they are lined to the renderer in any way. For example, materials fall in that category, as they store shader data and are thus implementation dependant. So every kind of material implemented also needs to be piml'ed. In otrder to ensure that all these classes uses the correct implementation, they rely on a single common game parameter to prevent stupid bugs from happening.
