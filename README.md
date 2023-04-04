This repository contains examples of shared memory using both SystemV IPC 
and POSIX shared memory. There are two C programs included for each type 
of shared memory, a sender and a receiver.

* `receiver.c` is an example of receiving shared memory using SystemV 
IPC and Unix socket to share the descriptor.
* `sender.c` is an example of sending shared memory using SystemV IPC 
and Unix socket to share the descriptor.
* `pr.c` is an example of receiving shared memory using POSIX shared 
memory.
* `ps.c` is an example of sending shared memory using POSIX shared 
memory.

There are also two Dockerfiles, one for the sender and one for the receiver. 
These Dockerfiles can be used to build and run containers for the sender 
and receiver programs.

To build the receiver container, navigate to the `receiver` directory and 
copy the `pr.c` file from the root directory to the `receiver` directory. 
Then, run the following command:

```
podman build -t pr .
```

To build the sender container, navigate to the `sender` directory and copy 
the `ps.c` file from the root directory to the `sender` directory. 
Then, run the following command:

```
podman build -t ps .
```

To run the sender and receiver containers, use the following commands:

```
podman run -it --rm --ipc=host ps
podman run -it --rm --ipc=host pr
```

Currently, the containers run on the host's IPC namespace, but there are 
other options available for isolating the IPC namespace if needed.
