#include <iomanip>
#include <sstream>
#include <zlib.h>
#include <vector>

#include "fingerprint.h"
#include "helpers.h"
#include "identify.h"
#include "selection.h"
// #include "verification.h"
// #include "identification.h"
// #include "enrollment.h"

#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>

using namespace v8;
using namespace std;
using v8::FunctionTemplate;

int initalized = -1;

NAN_METHOD(init)
{
    info.GetReturnValue().Set(initalized == 0);
    return;
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan);
    NAN_EXPORT(target, selectfpd);
}

NODE_MODULE(fingerprint, module_init)