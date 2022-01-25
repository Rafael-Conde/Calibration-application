#include "Instrument.h"

Instrument::Instrument(char order):
order {order}
{

}

Instrument::Instrument(char order, entry* entry_t) : order{ order }, entry_t{entry_t}
{

}
