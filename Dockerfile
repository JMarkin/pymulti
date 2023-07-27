FROM alpine as builder

ENV PYENV_ROOT /opt/pyenv
ENV PATH $PYENV_ROOT/shims:$PYENV_ROOT/bin:$PATH


# install pyenv for pythons
RUN apk add --no-cache git bash build-base libffi-dev openssl-dev bzip2-dev zlib-dev xz-dev readline-dev sqlite-dev tk-dev curl && \
    curl https://pyenv.run | bash


# install 3.11.4 3.12-dev
RUN env PYTHON_CONFIGURE_OPTS='--enable-optimizations --with-lto' PYTHON_CFLAGS='-march=native -mtune=native' pyenv install 3.11.4 3.12-dev

WORKDIR /app

RUN apk add --no-cache cmake

COPY . .

ARG THREADS 4
ENV THREADS ${THREADS}

RUN cmake --preset py11 && cmake --build build/py11 \
    && cmake --preset py12 && cmake --build build/py12

ENV PYTHONPATH /app
