# stupid-swarm

Playing with some swarm concepts.

# Quick Start

1. Clone this repo:

```bash
git clone https://github.com/idiasdas/stupid-swarm.git
```

2. Initialize the submodules:

```bash
git submodule update --init
```

3. Configure CMake:

```bash
# You can also use -DCMAKE_BUILD_TYPE=Release to have more performance
# You can specify a directory different than "build" if you want it for some reason
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

4. Build the project:

```bash
# In the directory build/
cd build/
make
```

5. Run executable:

```bash
./stupid-swarm
```


# TODO

1. Moving Goal Simulation
- [x] Particles color picking
- [x] Start/Pause button
- [x] Reset button
- [ ] Goal follow mouse
- [x] FPS plot
- [ ] LOG window
- [x] Particles number slider

2. Eletric forces simulation
