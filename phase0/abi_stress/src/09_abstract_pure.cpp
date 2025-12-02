#include "09_abstract_pure.h"

void PureCaller::CallPure(IPure* p) {
    if (p) {
        p->PureMethod(); // should resolve to _purecall if not implemented/instantiated
    }
}
