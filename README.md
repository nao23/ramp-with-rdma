# ramp-with-rdma
This repository contains the prototype distributed in-memory key-value store evaluated in our paper. 
The prototype contains several different implementations of distributed transactions which are as follows.
- RAMP-Fast
- Three Locking Algorithm
- No concurrency control
- RAMP-Fast with RDMA (Our proposal)

The prototype also supports several different communication methods which are as follows.
- TCP/IP over Ethernet
- IP over InfiniBand
- Send/Recv Verbs
- RDMA Write (Our proposal)
- RDMA Write with Immediate 
- RDMA Read (Our proposal)

## Requirements
- GCC 4.9.3
- Boost 1.59.0
- Intel TBB 4.4
- MessagePack for C/C++ 1.2.0
- RDMA Communicaton Manager

## Build
`make -j8 all`

## Run
### Server
`./server -t TRX_TYPE -c COM_TYPE`

### Client
`./client -t TRX_TYPE -c COM_TYPE -w WRITE_TRX_NUM -r READ_TRX_NUM`