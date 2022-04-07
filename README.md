# Game

To compile / build

Make sure you have Cmake

## To Run

```shell
./run.sh
```

If you don't have enough permissions:

```shell
chmod +x ./run.sh
```

## Manual Build / Run

```shell
cmake -S . -B build
cmake --build build
```

To run, make sure you're inside of the build directory.

```shell
./build/Game
```
