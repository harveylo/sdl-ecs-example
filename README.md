# An Entity-Component-System Example With SDL

This is an example of an entity component system, on the first draft implemented as prototype by using unordered maps and on the current state I'm using EnTT. Find the corresponding blog posts here: 

- Naive prototype for ECS: https://www.codingwiththomas.com/blog/an-entity-component-system-from-scratch
- EnTT: https://www.codingwiththomas.com/blog/use-entt-when-you-need-an-ecs
## Build

Use conan to build this example. If you haven't install conan, use `pip` to install: 
```
pip install conan==1.60.0
```
And then run the build commands. Note: I know there is already conan 2.0, but I haven't found time yet to check it out, so please stick to a version below 2.0

***CAUTION: if you are using vscode cmake plugin, the COMPILER KIT must be set as GCC, not clang or any other compilers***
```
conan install . -if ./build --build missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
cmake -S . -B ./build
cmake --build ./build
```