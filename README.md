# ramp-with-rdma
This repository contains the prototype distributed in-memory key-value store evaluated in our [paper](http://id.nii.ac.jp/1001/00161818/). 
The prototype contains several different implementations of distributed transactions and communication methods which are as follows.

Distributed transactions:
- RAMP-Fast
- Three locking algorithms
- No concurrency control
- RAMP-Fast with RDMA (Our proposal)

Communication methods:
- TCP/IP over Ethernet
- IP over InfiniBand
- Send/Recv Verbs
- RDMA Write 
- RDMA Write with Immediate 
- RDMA Read

## Requirements
- GCC 4.9.3
- Boost 1.59.0
- Intel TBB 4.4
- MessagePack for C/C++ 1.2.0
- RDMA Communicaton Manager

## Build
`make -j8 all`

## Setup
Edit `server_lists/tcp` and `server_lists/ib` for specifying server's hostname and port.

## Run
### Server
```
$ ./server -t TRX_TYPE -c COM_TYPE
```
TRX_TYPE is the parameter for specifying transaction algorithms. Supporting parameters are as follows.
- ramp_f (RAMP-Fast)
- rr, rc, ru (Locking algorithms)
- no_cc (No concurrency control)
- ac_ramp_f (RAMP-Fast with RDMA)

COM_TYPE is the parameter for specifying communication method. Supporting parameters are as follows.
- tcp (TCP/IP over Ethernet)
- ipoib (IP over InfiniBand)
- send_recv (Send/Recv Verbs)
- rdma_write (RDMA Write)
- rdma_write_imm (RDMA Write with Immediate)

### Client
```
$ ./client -t TRX_TYPE -c COM_TYPE -w WRITE_TRX_NUM -r READ_TRX_NUM
```

WRITE_TRX_NUM and READ_TRX_NUM are the parameter for specifying the number of write transaction and read transaction respectively.