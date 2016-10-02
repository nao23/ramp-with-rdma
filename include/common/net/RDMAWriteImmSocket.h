#pragma once

#include "RDMAWriteSocket.h"
#include "Config.h"
#include "Item.h"


class RDMAWriteImmSocket : public RDMAWriteSocket {

private:
    static std::shared_ptr<spdlog::logger> class_logger;  // for this class
    Buffer msg_buf;

    void post_write_imm(const Buffer& buf, const RemoteKeyAndAddr& rka);        

public:
    RDMAWriteImmSocket(struct rdma_cm_id* client_id) : RDMAWriteSocket(client_id) {}
    ~RDMAWriteImmSocket() = default;
    static RDMAWriteImmSocket* connect(const HostAndPort& hp);
    
    void send_msg(MessageHeader header, char* body);
    void recv_header(MessageHeader* header);
    char* get_body(size_t body_size);
    void clear_msg_buf();
    void send_close();
};

static inline int
rdma_post_write_imm(struct rdma_cm_id* id, void* context, void* addr,
                    size_t length, struct ibv_mr* mr, int flags,
                    uint64_t remote_addr, uint32_t rkey)
{
    struct ibv_sge sge;

    sge.addr = (uint64_t) (uintptr_t) addr;
    sge.length = (uint32_t) length;
    sge.lkey = mr ? mr->lkey : 0;

    struct ibv_send_wr wr, *bad;

    wr.wr_id = (uintptr_t) context;
    wr.next = NULL;
    wr.sg_list = &sge;
    wr.num_sge = 1;
    wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
    wr.send_flags = flags;
    wr.wr.rdma.remote_addr = remote_addr;
    wr.wr.rdma.rkey = rkey;
    wr.imm_data = 1855;

    return rdma_seterrno(ibv_post_send(id->qp, &wr, &bad));
}
