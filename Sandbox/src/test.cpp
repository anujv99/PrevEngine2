
#include "prev.h"

using namespace prev;

class Testapp : public Application {

};

Application * CreateApplication() {
	return new Testapp();
}
