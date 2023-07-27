# Example PEP 684 – A Per-Interpreter GIL

## How to build and test PEP

1. Build docker `docker build --build-arg THREADS=$(nproc) -t pymulti .`
2. Runs
    - python11 `docker run --rm pymulti bash -c "time /app/build/py11/pymulti"`
    - python12 `docker run --rm pymulti bash -c "time /app/build/py12/pymulti"`

## Result on my machine
- python11
```
real    0m26.531s
user    0m26.690s
sys     0m0.008s
```
- python12
```
real    0m4.297s
user    0m33.928s
sys     0m0.035s
```
I think it is awesome ^_^
