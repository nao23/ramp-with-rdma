#include "TrxQueue.h"


void TrxQueue::gen_test_trxs() {

    Config& config = Config::get_config();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> key_dice(1, config.data_num);
    std::uniform_int_distribution<int> value_dice(0, 9);
    std::vector<Transaction*> trxs;

    Map<Field, Value> values;
   
    char* val_str = new char[config.value_size];
    for (int i = 0; i < config.value_size; ++i) {
	snprintf(val_str + i, config.value_size, "%d", value_dice(mt));
    }

    values[Field("f")] = Value(val_str, config.value_size);

    // generate write trxs
    for (int i = 0; i < config.write_trx_num; ++i) {
	Transaction* trx = new Transaction();
	for (int j = 0; j < config.trx_len; ++j) {
	    trx->add_write(std::to_string(key_dice(mt)), values);
	}
	trxs.push_back(trx);
    }
    

    Set<Field> fields;
    fields.insert(Field("f"));

    // generate read trxs
    for (int i = 0; i < config.read_trx_num; ++i) {
	Transaction* trx = new Transaction();
	for (int j = 0; j < config.trx_len; ++j) {
	    trx->add_read(std::to_string(key_dice(mt)), fields);
	}
	trxs.push_back(trx);
    }

    // shuffle queue
    std::random_shuffle(trxs.begin(), trxs.end());
    
    // push trxs to queue
    for (auto& trx : trxs) {
	this->queue.push(trx);
    }
}

bool TrxQueue::pop(Transaction*& trx) {
    return (this->queue.try_pop(trx));
}
